/*
 * RelexToLogicCB.h
 *
 *  Created on: 19 Jan, 2015
 *      Author: misgana
 */

#ifndef RELEXTOLOGICCB_H_
#define RELEXTOLOGICCB_H_

#include "DefaultForwardChainerCB.h"

using namespace opencog;

class RelexToLogicCB :public virtual DefaultForwardChainerCB {
public:
	RelexToLogicCB(AtomSpace* as);
	virtual ~RelexToLogicCB();
};

#endif /* RLEXTOLOGICCB_H_ */
