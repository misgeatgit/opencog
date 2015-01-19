/*
 * ForwardChainer.cc
 *
 * Copyright (C) 2014,2015 Misgana Bayetta
 *
 * Author: Misgana Bayetta <misgana.bayetta@gmail.com>
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

#include "ForwardChainer.h"
#include "ForwardChainerCallBack.h"
#include <opencog/reasoning/RuleEngine/rule-engine-src/JsonicControlPolicyLoader.h>

ForwardChainer::ForwardChainer(AtomSpace * as, string conf_path /*=""*/) :
		Chainer(as) {
	_as = as;
	_fcmem = new FCMemory(_as);
	if (conf_path != "")
		_conf_path = conf_path;
	init();
}

ForwardChainer::~ForwardChainer() {
	delete _fcmem;
}

void ForwardChainer::init() {
	_cpolicy_loader = new JsonicControlPolicyLoader(_main_atom_space,
			_conf_path);
	_fcmem->_search_in_af = _cpolicy_loader->get_attention_alloc();
	_fcmem->_rules = _cpolicy_loader->get_rules();
	_fcmem->_cur_rule = nullptr;
}

void ForwardChainer::do_chain(ForwardChainerCallBack& fcb,
		Handle htarget/*=Handle::UNDEFINED*/) {
	Handle hcur_target;
	int steps = 0;
	auto max_iter = _cpolicy_loader->get_max_iter();
	//TODO implement termination criteria
	while (steps < max_iter /*OR other termination criteria*/) {
		//set target
		if (htarget == Handle::UNDEFINED and steps == 0)
			hcur_target = choose_target_from_atomspace(_main_atom_space); //start FC on a random target
		else if (htarget != Handle::UNDEFINED and steps == 0)
			hcur_target = htarget;
		else
			hcur_target = fcb.choose_next_target(*_fcmem);
		//add more premise to hcurrent_target by pattern matching
		HandleSeq input = fcb.choose_input(*_fcmem);
		_fcmem->update_target_list(input);
		//choose the best rule to apply
		Rule& r = fcb.choose_rule(*_fcmem);
		_fcmem->_cur_rule = &r;
		//apply rule
		HandleSeq product = fcb.apply_rule(*_fcmem);
		_fcmem->add_rules_product(product);
		_fcmem->update_target_list(product);

		steps++;
	}
}

HandleSeq ForwardChainer::get_chaining_result() {
	return _fcmem->get_result();
}
