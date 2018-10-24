/*
 * PLNDynamicsExpSetupModule.h
 *
 *  Created on: Apr 19, 2015
 *      Author: misgana
 */

#ifndef PLNDYNAMICSEXPSETUPMODULE_H_
#define PLNDYNAMICSEXPSETUPMODULE_H_

#include "LoggerAgent.h"

#include <opencog/cogserver/server/Agent.h>
#include <opencog/cogserver/server/Factory.h>
#include <opencog/attention/AttentionModule.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <opencog/attentionbank/AttentionBank.h>

namespace opencog
{

class AtomSpace;
class CogServer;
class SchemeEval;
class Module;

//namespace ECANExperiment {

//extern int current_group;
using namespace std::chrono;

class InsectPoisonExpModule: public Module
{
private:
    AgentPtr _logger_agentptr;
    LoggerAgent* _logger_agent;

    AttentionBank * _bank;

    Factory<LoggerAgent, Agent> loggerAgentFactory;

    AtomSpace * _as;
    CogServer& _cs;

    std::map<Handle, AttentionValue::sti_t> * _stimulus_rec;

    //Load word dict.
    DECLARE_CMD_REQUEST(InsectPoisonExpModule, "dump-af-stat", do_dump_af_stat,
                        "Dumps AF stastics to a file",
                        "Usage: dump-af-stat <FILE_NAME>\n",
                        false, true)

    DECLARE_CMD_REQUEST(InsectPoisonExpModule, "start-logger", do_start_logger,
                        "Starts logger agent",
                        "Usage: start-logger\n",
                        false, true)

    void registerAgentRequests();
    void unregisterAgentRequests();

    std::string print_timept(time_point<system_clock> tpt)
    {
        auto ms = duration_cast<milliseconds>(tpt.time_since_epoch()) % 1000;
        std::time_t t = system_clock::to_time_t(tpt);
        std::stringstream ss;
        std::tm tm = *std::localtime(&t);
        ss << std::put_time(&tm, "%H:%M:%S");
        ss << ':' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    };


public:
    InsectPoisonExpModule(CogServer&);
    virtual ~InsectPoisonExpModule();
    static inline const char* id();
    virtual void init(void);
};
//}

} /* namespace opencog */

#endif /* EXPERIMENTSETUPMODULE_H_ */
