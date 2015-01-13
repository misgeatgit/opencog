/*
 * ForwardChainerx.h
 *
 *  Created on: 14 Jan, 2015
 *      Author: misgana
 */

#ifndef FORWARDCHAINERX_H_
#define FORWARDCHAINERX_H_

#include "Chainer.h"
#include "PLNCommons.h"
#include "ForwardChainerCallBack.h"
#include "ForwardChainInputMatchCB.h"
#include "ForwardChainPatternMatchCB.h"

#include <opencog/query/DefaultImplicator.h>
#include <opencog/reasoning/RuleEngine/rule-engine-src/ControlPolicyLoader.h>

class ForwardChainerx: public virtual Chainer {
private:
	/**
	 * choose a random target to start forward chaining with. This is useful when there is no target
	 * specified ahead to the forward chaining process.
	 * @param as - the atomspace instance from which target is selected
	 */
	Handle choose_target_from_atomspace(AtomSpace *);

	Implicator * fcim_; //xxx why doesn't the compiler like changing the type to PatternMatchCallBack
	Implicator * fcpm_;
	bool search_in_af;

	ControlPolicyLoader * cpolicy_loader_;
	HandleSeq target_list_; /*potential target list*/
	HandleSeq chaining_results;

	vector<Rule*> rules_; //rules loaded from control policy
	Handle cur_rule_; // = Handle::UNDEFINED; //Handle to chosen BindLink on a praticular step of forward chaining
	ForwardChainerCallBack* fcb_;
public:
	ForwardChainerx(AtomSpace * as, Implicator* input_match_cb,
			Implicator* pm_cb, string conf_path = "");
	virtual ~ForwardChainerx();
	void do_chain(ForwardChainerCallBack* fcb,Handle htarget);
	void init();
};

#endif /* FORWARDCHAINERX_H_ */
