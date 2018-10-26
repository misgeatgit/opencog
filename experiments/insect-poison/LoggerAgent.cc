#include "LoggerAgent.h"

using namespace opencog;
using namespace std::chrono;
using namespace std::placeholders;

#define MAX_SAMPLES 50000

LoggerAgent::~LoggerAgent()
{
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

LoggerAgent::LoggerAgent(CogServer& cs) : Agent(cs), _start(system_clock::now())
{
    _as = &cs.getAtomSpace();
    _bank = &attentionbank(_as);
    _start = system_clock::now();
    _stimulus_rec = &(_bank->stimulusRec);
    //    _as->atomAddedSignal().connect(std::bind(&LoggerAgent::atomAddedListener, this,
    //                                   _1));
    //_bank->AddAFSignal().connect(std::bind(&LoggerAgent::atomAddedListener, this, _1));

    try {
        af_duration_stat.reserve(MAX_SAMPLES); //allocate 50K sample holding space.
    } catch (const std::exception& e) {
        std::cout << "Allocation Error: " << e.what() << '\n';
    }
}

// Listens to Atoms added to the AS.
// Assumption: if not topic_changed flag is set, atoms are stored in
// poison_atom vec. Else, they would be stored in insect_atoms vec.
void LoggerAgent::atomAddedListener(const Handle& h)
{
}

void LoggerAgent::run(void)
{
    HandleSeq afset;
    _bank->get_handle_set_in_attentional_focus(std::back_inserter(afset));
    if (afset.size() <= 0 )
        return;
   
    auto at_time = system_clock::now();

    std::vector<LogData> now;
    for(auto h : afset) {
       auto it = std::find_if(logdata.begin(), logdata.end(),[h](LogData& ld)
                             {return ld.h == h;});
       
       AttentionValue::sti_t direct_sti = 0.0f;
       if (_stimulus_rec->find(h) != _stimulus_rec->end())
           direct_sti = (*_stimulus_rec)[h];

       if(it != logdata.end()){
           (*it).sti2 = get_sti(h);
           (*it).direct_sti2 = direct_sti;
           (*it).t2 = at_time;
           duration<double> dr = (*it).t2 - (*it).t1;
           (*it).duration = dr.count();
           (*it).sti_change = (*it).sti2 - (*it).sti1;
           (*it).direct_sti_change = (*it).direct_sti1 - (*it).direct_sti2;
           // Add it to the newest list
           now.push_back(*it);
           // Remove it from the prev list.
           logdata.erase(it);
       } else {
           LogData ld;
           ld.h = h;
           ld.sti1 = get_sti(h);
           ld.direct_sti1 = direct_sti;
           ld.t1 = at_time;
           now.push_back(ld);
       }
    }
   
   // merge the previous and latest as the newest logdata.
   logdata.insert(logdata.end(), now.begin(), now.end());
}

