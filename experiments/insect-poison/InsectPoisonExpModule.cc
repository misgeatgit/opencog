/*
 * InsectPoisonExpModule.cc
 *
 *  Created on: Apr 19, 2015
 *      Author: misgana
 */

#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdio.h>

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/attentionbank/AttentionBank.h>
#include <opencog/guile/SchemeEval.h>

#include <opencog/attention/atom_types.h>

#include <opencog/cogserver/server/CogServer.h>
#include <opencog/cogserver/server/Module.h>
#include <opencog/util/Config.h>

#include <opencog/attention/AttentionStat.h>

#include "InsectPoisonExpModule.h"

using namespace opencog;

DECLARE_MODULE(InsectPoisonExpModule);

InsectPoisonExpModule::InsectPoisonExpModule(CogServer& cs) :
    Module(cs), _cs(cs)
{
    _as = &_cs.getAtomSpace();
    _bank = &attentionbank(_as);
    _stimulus_rec = &(_bank->stimulusRec);
    
    // file_name = std::string(PROJECT_SOURCE_DIR)
    //             + "/experiments/attention/dump";
    registerAgentRequests();
}

InsectPoisonExpModule::~InsectPoisonExpModule()
{
    unregisterAgentRequests();
}

void InsectPoisonExpModule::registerAgentRequests()
{
    do_dump_af_stat_register();
    do_set_topic_switched_register();
    do_dump_af_size_register();
    do_start_logger_register();
    do_parse_sent_register();
}

void InsectPoisonExpModule::unregisterAgentRequests()
{
    do_dump_af_stat_unregister();
    do_set_topic_switched_unregister();
    do_dump_af_size_unregister();
    do_start_logger_unregister();
    do_parse_sent_unregister();
}

#define LOGGER_AGENT_PTR(AGENT_PTR) (dynamic_cast<LoggerAgent*>(AGENT_PTR.get()))

void InsectPoisonExpModule::init(void)
{
    registerAgentRequests();
    _cs.registerAgent(LoggerAgent::info().id,
            &loggerAgentFactory);
    _logger_agentptr = _cs.createAgent(
            LoggerAgent::info().id, false);
    _logger_agent = LOGGER_AGENT_PTR(_logger_agentptr);
}

std::string InsectPoisonExpModule::do_dump_af_stat(Request *req,
        std::list<std::string> args)
{
    std::string file_name = args.front(); //get file name if provided.

    // case when dump-af-stat is called with no argument  
    if(file_name.empty()){
        file_name = "afstat.data";
    } else {
        file_name += ".data";
    }

    std::ofstream outf(file_name, std::ofstream::trunc);
   
    outf << "TOTAL_STIMULUS_REC: " << _stimulus_rec->size() << "\n";
    outf << "NLP_parse_percentage_in_AF: "
        << (_logger_agent)->nlp_parse_percentage_in_af <<"\n"; 

    outf << "Logged_at: "
        << print_timept((_logger_agent)->last_probing_time) <<"\n"; 

    if(LoggerAgent::topic_changed){
        outf << "Prev_topic_percentage_in_af: " 
            << (_logger_agent)->prev_topic_percentage << "\n"; 
        outf << "Current_topic_percentage_in_af: "
            << (_logger_agent)->current_topic_percentage << "\n"; 
    }

    outf  << "Atom(uuid)" << ", Atom(name)" << ", EnteredAt" << ", LastSeenAt" << ", STI" <<  ", DurationInAF" 
        << ", IsNLPParseOutput" << ", DirectSTI"<< ", GainFromSpreading"<< "\n";

    for(auto p : (_logger_agent)->handle_atomstat_map)
    {
        LoggerAgent::AtomStat ast = p.second;
        outf << ast.h.value() << ", ";
        if(ast.h->is_node()){
            outf << ast.h->get_name();
        }else{
            //convert the link to one line
            std::string str = ast.h->to_short_string();
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
            outf <<  nameserver().getTypeName(ast.h->get_type()) << "["
                 << str << "]";
            //outf <<  nameserver().getTypeName(ast.h->get_type()) << "["
            //     << ast.h.value() << "]";
        }

        outf <<", "<< print_timept(ast.entered_at) 
            << ", " << print_timept(ast.last_active)
            << ", " << get_sti(ast.h)
            << ", " << ast.dr.count()  << ", "
            << ast.is_nlp_parse_output<< ", ";
        if(atom_avstat.find(ast.h) != atom_avstat.end()){ 
            //outf<< atom_avstat[ast.h].direct_sti_gain << ","
            if(_stimulus_rec->find(ast.h) != _stimulus_rec->end()){
                outf<< (*_stimulus_rec)[ast.h] << ", ";
            } else{
                outf << " - , ";
            }
            outf<< (atom_avstat[ast.h].heblink_sti_gain + atom_avstat[ast.h].link_sti_gain)
                << '\n';
        }else{
            outf<< " - , "
                <<  " - "
                << '\n';
        }
    }
    outf.flush();
    outf.close();
    return "Dumped it to " + file_name + "\n";
}

std::string InsectPoisonExpModule::do_dump_af_size(Request *req,
        std::list<std::string> args)
{
    std::string file_name = args.front();
    std::ofstream outf(file_name + "-af.data", std::ofstream::trunc);
    //outf << "AF_szie" <<"     at_time(sec)" <<"     nlp_parse_percentage" << "\n";
    outf << "at_time(sec)" <<", nonNLP_percentage"<<", insect_percentage" << ", poison_percentage" <<", insecticide_percentage" << "\n";
    //for (const auto afs : (_logger_agent)->af_size_stat){
    for (const auto afs : (_logger_agent)->insect_poison_percentage){
        outf << afs[0] << ", "  <<  afs[1]<<", " << afs[2] << ", " << afs[3] << ", "<< afs[4] << "\n";
    }
    outf.flush();
    outf.close();
    return "Dumped it to " + file_name + "\n";
}

std::string InsectPoisonExpModule::do_set_topic_switched(Request *req,
        std::list<std::string> args) {
    LoggerAgent::topic_changed = true;
    return "Topic changed flag is set.\n";
}

std::string InsectPoisonExpModule::do_start_logger(Request* req, std::list<std::string> args){
    _cs.startAgent(_logger_agentptr,false, "AFLogger");
    return "LoggerAgent started\n";
}

std::string InsectPoisonExpModule::do_parse_sent(Request* req, std::list<std::string> args){
    auto it = args.begin();
    std::string sentence_file = *it;
    ++it;
    int stimulus_amount = stoi(*it);
    ++it;
    int topic_switched = stoi(*it);

    SchemeEval _scm_eval(_as);
    _scm_eval.eval_h("(use-modules (opencog )) \
            (use-modules (opencog nlp relex2logic)) \
            (use-modules (opencog nlp chatbot))");

    std::string cmd1 = "(nlp-start-stimulation"+ std::to_string(stimulus_amount) +")";
    _scm_eval.eval_h(cmd1.c_str());  

    std::ifstream ifs;
    std::string line;
    ifs.open(sentence_file, std::ifstream::in);

    LoggerAgent::topic_changed = topic_switched;

    for (std::string line; getline( ifs, line ); ){
        std::cout << "Parsing " << line << '\n';
        std::string cmd2 = "(nlp-parse \""+ line +"\")";
        _scm_eval.eval_h(cmd1.c_str());  

    }    

    return "LoggerAgent started\n";
}
