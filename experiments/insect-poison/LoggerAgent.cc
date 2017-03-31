#include "LoggerAgent.h"

using namespace opencog;
using namespace std::chrono;

#define MAX_SAMPLES 30000
bool LoggerAgent::topic_changed = false;

LoggerAgent::~LoggerAgent(){
}

const ClassInfo& LoggerAgent::classinfo() const
{
    return info();
}

const ClassInfo& LoggerAgent::info()
{
    static const ClassInfo _ci("opencog::LoggerAgent");
    return _ci;
}

LoggerAgent::LoggerAgent(CogServer& cs) : Agent(cs), _start(system_clock::now()){
    _as = &cs.getAtomSpace();
    _bank = &attentionbank(_as);
    _start = system_clock::now();
    _atomAddedSignalConnection = _as->addAtomSignal(
            boost::bind(&LoggerAgent::atomAddedListener, this, _1));

    af_size_stat.reserve(MAX_SAMPLES); //allocate 50K sample holding space.

}


void LoggerAgent::atomAddedListener(const Handle& h){
    //if(h->getType() == WORD_NODE){
        corpus_wordnodes.push_back(h);
        if(topic_changed){
            switched_topic_words.push_back(h); 
        }
    //}
}

void LoggerAgent::run(void){
    HandleSeq afset;
    _bank->get_handle_set_in_attentional_focus(std::back_inserter(afset));
    
    if(afset.size() <= 0 ) return;
    
    static bool first_time = true;
    last_probing_time = system_clock::now(); //reference time to deduce whether atom is still in AF or not.
    static auto last_run = system_clock::now();

    if(topic_changed && first_time){
        // task-4 calculate percentage of AF content from prev topic
        // and current topic 
        HandleSeq poison_wnodes_in_AF; 
        HandleSeq insect_wnodes_in_AF; 

        std::sort(switched_topic_words.begin(), switched_topic_words.end());
        std::sort(corpus_wordnodes.begin(), corpus_wordnodes.end());
        std::sort(afset.begin(), afset.end());

        // Get poison wnodes in the AF  
        std::set_intersection(switched_topic_words.begin(), switched_topic_words.end(),
                afset.begin(), afset.end(),
                std::back_inserter(poison_wnodes_in_AF));

        // Get insect wnodes in the AF 
        HandleSeq insect_wnodes; 
        std::set_difference(switched_topic_words.begin(), switched_topic_words.end(),
                corpus_wordnodes.begin(), corpus_wordnodes.end(),
                std::back_inserter(insect_wnodes));
        std::sort(insect_wnodes.begin(), insect_wnodes.end());
        std::set_intersection(insect_wnodes.begin(), insect_wnodes.end(),
                afset.begin(), afset.end(),
                std::back_inserter(insect_wnodes_in_AF));

        prev_topic_percentage = (static_cast<float>(insect_wnodes_in_AF.size())/ static_cast<float>(afset.size()))*100;
        current_topic_percentage = (static_cast<float>(poison_wnodes_in_AF.size())/static_cast<float>(afset.size()))*100;
        first_time = false;
    }

    int count = 0;
    for(const Handle& h : afset){
        bool is_nlp_parse_output = false;
        auto it = std::find(corpus_wordnodes.begin(), corpus_wordnodes.end(), h);
        if(it != corpus_wordnodes.end()){
            count ++;
            is_nlp_parse_output = true;
        }                 

        //task 2 - query af and fill atomstat
        auto iter = handle_atomstat_map.find(h);
        auto time = system_clock::now();
        if (iter == handle_atomstat_map.end()){
            AtomStat ast;
            ast.h = h;
            ast.last_active = time;
            ast.entered_at = time;
            ast.is_nlp_parse_output = is_nlp_parse_output;
            ast.dr = duration<double>(0);
            // ast.percentage_sti_gain_from_spreading =
            handle_atomstat_map[h] = ast;
        }
        else{
            (iter->second).last_active = time;
            (iter->second).dr = system_clock::now() - (iter->second).entered_at;
        }
    }

    // task 1- Update constituency percentage.
    if(afset.size() > 0 )
        nlp_parse_percentage_in_af = (static_cast<float>(count)/static_cast<float>(afset.size()))*100;


    // task 3 query afsize and store it in af_size_at_time_t ds
    // this a memory intensive operation so do it every half a second and also
    // clear memory when it reaches 50K
    duration<double> dr = system_clock::now() - last_run;
    if(dr.count() >= 0.1 ){
        if(af_size_stat.size() > MAX_SAMPLES){
         std::cout << "MAX sample reached. Removing the fist 1K samples \n";
         af_size_stat.erase(std::begin(af_size_stat),af_size_stat.begin() + 1000);
        }
        AFSizeStat afs;
        afs.at_time = system_clock::now();
        afs.af_size = afset.size();
        afs.nlp_parse_percentage = nlp_parse_percentage_in_af;
        af_size_stat.push_back(afs);
    }

    last_run = system_clock::now();
}




