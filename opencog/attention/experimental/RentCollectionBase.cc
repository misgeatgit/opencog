#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>
#include <fstream>

#include <opencog/util/Config.h>
#include <opencog/util/mt19937ar.h>
#include <opencog/attention/atom_types.h>

#define DEPRECATED_ATOMSPACE_CALLS
#include <opencog/atomspace/AtomSpace.h>

#include "RentCollectionBase.h"

//#define DEBUG

using namespace opencog;

RentCollectionBase::RentCollectionBase(CogServer& cs) 
{
    a = &cs.getAtomSpace();
    
    // init starting wages/rents. these should quickly change and reach
    // stable values, which adapt to the system dynamics
    STIAtomRent = config().get_int("ECAN_STARTING_ATOM_STI_RENT");
    LTIAtomRent = config().get_int("ECAN_STARTING_ATOM_LTI_RENT");

    targetSTI = config().get_int("TARGET_STI_FUNDS");
    stiFundsBuffer = config().get_int("STI_FUNDS_BUFFER");
    targetLTI = config().get_int("TARGET_LTI_FUNDS");
    ltiFundsBuffer = config().get_int("LTI_FUNDS_BUFFER");

    // Provide a logger
    log = NULL;
    setLogger(new opencog::Logger("RentCollectionAgent.log", Logger::FINE,
    true));
}

RentCollectionBase::~RentCollectionBase()
{
    if (log)
        delete log;
}

void RentCollectionBase::setLogger(Logger* _log)
{
    if (log)
        delete log;
    log = _log;
}

Logger* RentCollectionBase::getLogger()
{
    return log;
}

int RentCollectionBase::calculate_STI_Rent()
{
    int funds = a->get_STI_funds();
    float diff  = targetSTI - funds;
    float ndiff = diff / stiFundsBuffer;
    ndiff = std::min(ndiff,1.0f);
    ndiff = std::max(ndiff,-0.99f);
    //printf("ndiff: %f   ",ndiff);
    //
    float res = STIAtomRent + (STIAtomRent * ndiff);

    if (res < 1)
        if ((rand() % 100) > (100 * res))
            res = 1;

    return floor(res);
}

int RentCollectionBase::calculate_LTI_Rent()
{
    int funds = a->get_LTI_funds();
    float diff  = targetLTI - funds;
    float ndiff = diff / ltiFundsBuffer;
    ndiff = std::min(ndiff,1.0f);
    ndiff = std::max(ndiff,-1.0f);

    return LTIAtomRent + (LTIAtomRent * ndiff);
}

using  sys_clock = std::chrono::system_clock;

void RentCollectionBase::log_rent(const Handle& h, AttentionValue::sti_t charged)
{  
    auto in_time_t = sys_clock::to_time_t(sys_clock::now());
    static bool first_time = true;
    std::ofstream outf("rent.data", std::ofstream::out | std::ofstream::app);
    
    if(first_time){
       outf << "uuid  sti  charged  af_boundary  time\n";
       first_time = false;
    }
    // src_uuid  src_sti  trgt_uuid trgt_sti  time amount af_sti

    char buff[31];
    strftime(buff, 30, "%H:%M:%S", std::localtime(&in_time_t));
    std::string ts(buff);

   outf << h.value() << "  " << h->getSTI() << "  " << charged << "  "
    << a->get_attentional_focus_boundary() << "  " << ts << "\n";

    outf.flush();
    outf.close();  
}
