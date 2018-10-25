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
    do_start_logger_register();
}

void InsectPoisonExpModule::unregisterAgentRequests()
{
    do_dump_af_stat_unregister();
    do_start_logger_unregister();
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


    std::vector< std::pair<time_point<system_clock>, std::vector<LoggerAgent::AtomAFStat>>>& stat = _logger_agent->af_duration_stat;


    struct LogData {
        Handle h;
        time_point<system_clock> t1;
        time_point<system_clock> t2;
        double duration;
        AttentionValue::sti_t sti1;
        AttentionValue::sti_t sti2;
        double sti_change;
    };

    std::vector<LogData> logdata;

    for(int i=0; i < (int)stat.size() ; i++) {
        auto& tp_vec = stat[i];

        for(LoggerAgent::AtomAFStat& afs : tp_vec.second){
            Handle& h = afs.h;
            bool exists = true;

            time_point<system_clock> t1 = tp_vec.first;
            time_point<system_clock> t2 = tp_vec.first;
            AttentionValue::sti_t sti1 = afs.total_sti;
            AttentionValue::sti_t sti2 = afs.total_sti;

            // update t2 and sti2
            while(exists) {
                // look in the next time bin.
                for(int j=i+1; j < (int)stat.size(); j++){
                    auto& afsvec = stat[j].second;
                    auto it = std::find_if(afsvec.begin(), afsvec.end(),[h](LoggerAgent::AtomAFStat& afs){
                            return afs.h == h;
                            });
                    if(it == afsvec.end()){
                        exists = false;
                        break;
                    } else { 
                        t2 =stat[j].first;
                        sti2 = (*it).total_sti;
                        // erase it so that won't be checked in the next
                        // round.
                        afsvec.erase(it);
                    }
                }
            }

            // Create LogData
            LogData log;
            log.h = h;
            log.t1 = t1;
            log.t2 = t2;
            duration<double> dr = t2 - t1;
            log.duration = dr.count();
            log.sti1 = sti1;
            log.sti2 = sti2;
            log.sti_change = sti2 - sti1;

            logdata.push_back(log);
        }


    }


    std::string file_name = args.front(); //get file name if provided.

    // case when dump-af-stat is called with no argument
    if (file_name.empty()) {
        file_name = "afstat.data";
    } else {
        file_name += ".data";
    }

    std::ofstream outf(file_name, std::ofstream::trunc);

    outf << "TOTAL_STIMULUS_REC: " << _stimulus_rec->size() << "\n";

    outf << "Logged_at: "
        << print_timept((_logger_agent)->last_probing_time) << "\n";

    outf  << "Atom(uuid)" << ", Atom(name)" << ", t1(H:m:s:ms)," <<" t2(H:m:s:ms)" <<" STI_1" <<", STI_2" <<", duration" <<", sti_diff"  << "\n";

    for (auto p : logdata) {
        Handle h = p.h;
        outf << h.value() <<", ";
        if (h->is_node()) {
            outf << h->get_name();
        } else {
            //convert the link to one line
            std::string str = h->to_short_string();
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
            outf <<  nameserver().getTypeName(h->get_type()) << "["
                << "-" << "]";
        }

        outf << ", " << print_timept(p.t1)
             << ", " << print_timept(p.t2)
            << ", " << p.sti1
            << ", " << p.sti2
            << ", " << p.duration
            << ", " << p.sti_change
            << "\n ";
    }
    outf.flush();
    outf.close();
    return "Dumped it to " + file_name + "\n";
}

std::string InsectPoisonExpModule::do_start_logger(Request* req,
        std::list<std::string> args)
{
    _cs.startAgent(_logger_agentptr, false, "AFLogger");
    return "LoggerAgent started\n";
}


