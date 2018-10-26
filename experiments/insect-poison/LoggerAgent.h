/*
   (_logger_agent)-> Requirements
 * -------------
 * -1 Distribution of residence-duration of nlp-parse atoms
 * - Percentage of atoms in AF directly from nlp-parse //XXX From scheme or C++?

 * -2 Distribution of residence-duration in the AF
 * -3 Log af size every few ms
 * - Log percentage of sti gain from spreading and direct stimulation
 * - Percentage of the AF consisting of words from the insect corpus vs poison
 *   right after the transition from one corpus to the other
 * /



 * Design
 * --------
 *  struct AtomStat { bool from_sentence; Handle h; update last_update time;}
 *  Dump Log at time t (using a command from the shell)
 *
 */

/*
 * LoggerAgent.h
 *
 */

#ifndef _LOGGERAGENT_H_
#define _LOGGERAGENT_H_

#include <opencog/guile/SchemeEval.h>
#include <opencog/cogserver/server/Agent.h>
#include <opencog/cogserver/server/CogServer.h>
#include <opencog/nlp/types/atom_types.h>
#include <opencog/attentionbank/AttentionBank.h>

#include <unordered_set>
#include <chrono>

using namespace std::chrono;

namespace opencog
{

class LoggerAgent: public Agent
{
private:
    SchemeEval * _scm_eval;
    AtomSpace * _as;
    AttentionBank * _bank;
    time_point<system_clock> _start;
    std::map<Handle, AttentionValue::sti_t>* _stimulus_rec;

public:
    virtual ~LoggerAgent();
    LoggerAgent(CogServer& cs);

    virtual const ClassInfo& classinfo() const;
    static const ClassInfo& info();

    struct AtomAFStat {
        Handle h;
        //time_point<system_clock> at_time;
        AttentionValue::sti_t total_sti;  
        AttentionValue::sti_t direct_sti_gain;  
    };

    struct LogData {
        Handle h;
        time_point<system_clock> t1;
        time_point<system_clock> t2;
        double duration;
        AttentionValue::sti_t sti1 = 0;
        AttentionValue::sti_t direct_sti1 = 0;
        AttentionValue::sti_t sti2 = -1;
        AttentionValue::sti_t direct_sti2 = -1;
        double sti_change = 0;
        double direct_sti_change = 0;
    };

    std::vector<LogData> logdata;
    /*
     * for(each atom in AF):
     *      
     *
     *
     */ 
    

    std::vector< std::pair<time_point<system_clock>, std::vector<AtomAFStat>>> af_duration_stat;

    std::vector<Handle> af_size_stat;

    std::vector< std::pair<std::chrono::system_clock::time_point, AtomAFStat>> afStat;

    time_point<system_clock> last_probing_time; //duration of stay in sec.

    // This is guarenteed to function properly only when you load the
    // agent before starting nlp parse and you won't be loading any more
    // from other sources after starting it. i.e, load external sources first,
    // start this agent, do nlp-parsing and make sure not to add any more word nodes
    // from other sources after these steps.
    void atomAddedListener(const Handle& h);
    void run(void);
};

}

#endif /* _SENTENCEGENAGENT_H_ */

