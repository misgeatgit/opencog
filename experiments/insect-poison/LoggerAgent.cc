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
    std::vector<AtomAFStat> stat;
    stat.reserve(afset.size());

    for(auto h : afset) {
        AttentionValue::sti_t direct_sti = 0.0f;
        if (_stimulus_rec->find(h) != _stimulus_rec->end())
            direct_sti = (*_stimulus_rec)[h];
        
        //AtomAFStat afstat {h, get_sti(h), direct_sti};
        stat.push_back(AtomAFStat{h,get_sti(h), direct_sti});
    }
 
     af_duration_stat.push_back(std::make_pair(at_time, stat));
}

