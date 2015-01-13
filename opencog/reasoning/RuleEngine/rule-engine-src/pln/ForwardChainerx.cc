/*
 * ForwardChainerx.cc
 *
 *  Created on: 14 Jan, 2015
 *      Author: misgana
 */

#include "ForwardChainerx.h"

ForwardChainerx::ForwardChainerx(AtomSpace * as, Implicator* input_match_cb,
		Implicator* pm_cb, string conf_path /*=""*/) {
	if (conf_path != "")
		conf_path_ = conf_path;

	//set to default ones
	fcim_ = new ForwardChainInputMatchCB(main_atom_space_,
			target_list_atom_space_, this); //fetching from main and copying it to target_list_atom_space
	fcpm_ = new ForwardChainPatternMatchCB(target_list_atom_space_, this); // chaining PLN rules are applied on the target_list_atom_space
	init();
}

ForwardChainerx::~ForwardChainerx() {

}

void ForwardChainerx::init() {
	cpolicy_loader_ = new JsonicControlPolicyLoader(main_atom_space_,
			conf_path_); //new ControlPolicyLoader(main_atom_space, conf_path);
	search_in_af = cpolicy_loader_->get_attention_alloc();
	rules_ = cpolicy_loader_->get_rules();
	cur_rule_ = Handle::UNDEFINED;
}
void ForwardChainerx::do_chain(ForwardChainerCallBack* fcb, Handle htarget) { //set rules and premise the fcb first
	fcb_ = fcb;
	Handle hcurrent_target;
	int steps = 0;
	auto max_iter = cpolicy_loader_->get_max_iter();
	//bool terminate = false;
	while (steps <= max_iter /*or !terminate*/) {
		if (steps == 0) {
			if (htarget == Handle::UNDEFINED)
				hcurrent_target = choose_target_from_atomspace(
						main_atom_space_); //start FC on a random target
			else
				hcurrent_target = htarget;
		} else {
			if (!target_list_.empty())
				hcurrent_target = fcb->choose_next_target()(target_list_);
		}
		fcb->choose_input(hcurrent_target); //add more premise via pattern matching of related atoms to hcurrent_target
		Rule& rule = fcb->choose_rule(hcurrent_target);
		cur_rule_ = rule.get_handle();
		chaining_pm_.do_bindlink(cur_rule_, *fcpm_); //xxx guide matching to search only the target list
		steps++;
		//TODO implement termination criteria
	}
}

Handle ForwardChainerx::choose_target_from_atomspace(AtomSpace * as) {
	HandleSeq hs;
	as->getHandlesByType(back_inserter(hs), ATOM, true); //xxx experimental must be replaced by atoms in AF
	for (Handle h : hs)
		add_to_target_list(h);
	return fcb_->choose_next_target();
}

/**TODO
 * USE CALLBACKS FOR  ForwardChaining,InputMathcing and Searching.
 * implement mutual exclusions
 */
