/*
 * opencog/attention/WARentCollectionAgent.h
 *
 * Written by Misgana Bayetta
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <algorithm>
#include <math.h>
#include <time.h>
#include <chrono>
#include <fstream>

#include <opencog/util/Config.h>
#include <opencog/util/mt19937ar.h>
#include <opencog/attention/atom_types.h>

#define DEPRECATED_ATOMSPACE_CALLS
#include <opencog/atomspace/AtomSpace.h>

#include "RentCollectionBaseAgent.h"

//#define DEBUG

using namespace opencog;


RentCollectionBaseAgent::RentCollectionBaseAgent(CogServer& cs) :
    Agent(cs)  
{
    as = &cs.getAtomSpace();

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

RentCollectionBaseAgent::~RentCollectionBaseAgent()
{
    if (log)
        delete log;
}

void RentCollectionBaseAgent::setLogger(Logger* _log)
{
    if (log)
        delete log;
    log = _log;
}

Logger* RentCollectionBaseAgent::getLogger()
{
    return log;
}


void RentCollectionBaseAgent::run()
{

       while (true) {
        HandleSeq targetSet;
        selectTargets(targetSet);

        if (targetSet.size() == 0)
            continue;

        for (Handle& h : targetSet) {
            int sti = h->getAttentionValue()->getSTI();
            int lti = h->getAttentionValue()->getLTI();
            int stiRent = calculate_STI_Rent();
            int ltiRent = calculate_LTI_Rent();

            if (stiRent > sti)
                stiRent = sti;

            if (ltiRent > lti)
                ltiRent = lti;
            
            if(stiRent > 0)
              log_rent(h, stiRent);
            
            h->setSTI(sti - stiRent);
            h->setLTI(lti - ltiRent);
        }

        std::cout << "[DEBUG] [WARentCollectionAgent] sleeping for " << get_sleep_time() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(get_sleep_time()));
    }
}

int RentCollectionBaseAgent::calculate_STI_Rent()
{
    int funds = as->get_STI_funds();
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

int RentCollectionBaseAgent::calculate_LTI_Rent()
{
    int funds = as->get_LTI_funds();
    float diff  = targetLTI - funds;
    float ndiff = diff / ltiFundsBuffer;
    ndiff = std::min(ndiff,1.0f);
    ndiff = std::max(ndiff,-1.0f);

    return LTIAtomRent + (LTIAtomRent * ndiff);
}

using  sys_clock = std::chrono::system_clock;

void RentCollectionBaseAgent::log_rent(const Handle& h, AttentionValue::sti_t charged)
{  
    auto in_time_t = sys_clock::to_time_t(sys_clock::now());
    static bool first_time = true;
    char buff[31];
    strftime(buff, 30, "%H:%M:%S", std::localtime(&in_time_t));
    std::string ts(buff);

   // mtx.lock();
    std::ofstream outf("rent.data", std::ofstream::out | std::ofstream::app);
    if(first_time){
        outf << "uuid,sti,charged,af_boundary,time\n";
        first_time = false;
    }

    outf << h.value() << "," << h->getSTI() << "," << charged << ","
        << as->get_attentional_focus_boundary() << "," << ts << "\n";
    outf.flush();
    outf.close();  
    //mtx.unlock();
}
