/*
 * ForwardChainerCallBack.h
 *
 *  Created on: 14 Jan, 2015
 *      Author: misgana
 */

#ifndef FORWARDCHAINERCALLBACK_H_
#define FORWARDCHAINERCALLBACK_H_

#include "PLNCommons.h"

#include <opencog/reasoning/RuleEngine/rule-engine-src/Rule.h>

class ForwardChainerCallBack {
protected:
	AtomSpace * as_;
	vector<Rule*> rules_;
	HandleSeq target_list_;
	PLNCommons commons_;
public:
	ForwardChainerCallBack(AtomSpace* as, vector<Rule*> rules, HandleSeq tlist);
	;
	virtual ~ForwardChainerCallBack() {
		delete commons_;
	}
	;
	/**
	 * given a set of rules,choose next rule to be applied based
	 * target,fitness evaluation,weight of rule in the current context...etc
	 */
	virtual Rule& choose_rule(Handle target) = 0;
	/**
	 * Choose additional premises to the chainer based on fitness
	 * evaluation in the focus set.
	 */
	virtual void choose_input(Handle target) = 0;
	/**
	 * choose next target from the target list
	 */
	virtual Handle choose_next_target(void) = 0;

};

inline ForwardChainerCallBack::ForwardChainerCallBack(AtomSpace* as,
		vector<Rule*> rules, HandleSeq tlist) :
		as_(as), rules_(rules), target_list_(tlist) {
	commons_ = new PLNCommons(as_);
}

#endif /* FORWARDCHAINERCALLBACK_H_ */
