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

#include <opencog/util/Config.h>
#include <opencog/util/mt19937ar.h>
#include <opencog/attention/atom_types.h>

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/attentionbank/AttentionBank.h>
#include <opencog/cogserver/server/CogServer.h>

#include "RentCollectionBaseAgent.h"

//#define DEBUG
#ifdef DEBUG
#undef DEBUG
#endif

using namespace opencog;

RentCollectionBaseAgent::RentCollectionBaseAgent(CogServer& cs) :
    Agent(cs), _atq(&cs.getAtomSpace())
{
    _bank = &attentionbank(_as);
    load_params();
    // Provide a logger
    setLogger(new opencog::Logger("RentCollectionAgent.log", Logger::FINE, true));
}

void RentCollectionBaseAgent::run()
{
    load_params();
    HandleSeq targetSet;
    selectTargets(targetSet);

    if (targetSet.size() == 0) return;

    collectRent(targetSet);
}

void RentCollectionBaseAgent::load_params(void)
{
    STIAtomRent = std::stod(_atq.get_param_value(AttentionParamQuery::rent_starting_sti_rent));
    LTIAtomRent = std::stod(_atq.get_param_value(AttentionParamQuery::rent_starting_lti_rent));
}

AttentionValue::sti_t RentCollectionBaseAgent::calculate_STI_Rent()
{
    double funds = _bank->getSTIFunds();
    double diff  = _bank->getTargetSTI() - funds;

    if(diff <= 0)
        return 0;

    double ndiff = diff / funds;
    ndiff = std::min(ndiff, 1.0);
    ndiff = std::max(ndiff, -0.99);
    AttentionValue::sti_t res = STIAtomRent + (STIAtomRent * ndiff);

    return res;
}

AttentionValue::sti_t RentCollectionBaseAgent::calculate_LTI_Rent()
{
    double funds = _bank->getLTIFunds();
    double diff  = _bank->getTargetLTI() - funds;
    double ndiff = diff / funds;
    ndiff = std::min(ndiff, 1.0);
    ndiff = std::max(ndiff, -1.0);

    return LTIAtomRent + (LTIAtomRent * ndiff);
}
