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
#include <opencog/guile/SchemeSmob.h>
#include <opencog/util/Config.h>

ForwardChainer::ForwardChainer(AtomSpace * as, string conf_path /*=""*/) :
		Chainer(as) {
	_as = as;
	_fcmem = new FCMemory(_as);
	if (conf_path != "")
		_conf_path = conf_path;
	init();
}

ForwardChainer::~ForwardChainer() {
	delete _cpolicy_loader;
	delete _fcmem;
}

void ForwardChainer::init() {
	_cpolicy_loader = new JsonicControlPolicyLoader(_main_atom_space,
			_conf_path);
	_cpolicy_loader->load_config();
	_fcmem->_search_in_af = _cpolicy_loader->get_attention_alloc();
	_fcmem->_rules = _cpolicy_loader->get_rules();
	_fcmem->_cur_rule = nullptr;
}

void ForwardChainer::do_chain(ForwardChainerCallBack& fcb,
		Handle htarget/*=Handle::UNDEFINED*/) {
	int steps = 0;
	auto max_iter = _cpolicy_loader->get_max_iter();
	init_target(htarget);
	//TODO implement termination criteria
	while (steps < max_iter /*OR other termination criteria*/) {
		//add more premise to hcurrent_target by pattern matching
		cout << "choosen target is:" << SchemeSmob::to_string(_fcmem->get_cur_target()) << endl;
		cout << "CALLING choose_input" << endl;
		HandleSeq input = fcb.choose_input(*_fcmem);
		cout << "CALLING update_target list" << endl;
		_fcmem->update_target_list(input);
		//choose the best rule to apply
		cout << "CALLING choose_rule" << endl;
		Rule& r = fcb.choose_rule(*_fcmem);
		_fcmem->_cur_rule = &r;
		//apply rule
		cout << "CALLING apply_rule" << endl;
		HandleSeq product = fcb.apply_rule(*_fcmem);
		_fcmem->add_rules_product(product);
		_fcmem->update_target_list(product);
		//next target
		cout << "CALLING choose_next_target" << endl;
		_fcmem->set_target(fcb.choose_next_target(*_fcmem));
		steps++;
	}
}

void ForwardChainer::init_target(Handle htarget) {
	if (htarget == Handle::UNDEFINED)
		_fcmem->set_target(choose_random_target(_main_atom_space));	//start FC on a random target
	else
		_fcmem->set_target(htarget);
}


Handle ForwardChainer::choose_random_target(AtomSpace * as) {
	//choose a random atoms to start forward chaining with
	HandleSeq hs;
	if (_cpolicy_loader->get_attention_alloc())
		as->getHandleSetInAttentionalFocus(back_inserter(hs));
	else
		as->getHandlesByType(back_inserter(hs), ATOM, true);
	Handle rand_target;
	for (;;) {
		Handle h = hs[rand() % hs.size()];
		Type t= as->getType(h);
		if (t != VARIABLE_NODE and t != BIND_LINK and t!=IMPLICATION_LINK ) {
			rand_target = h;
			break;
		}
	}
	return rand_target;
}

HandleSeq ForwardChainer::get_chaining_result() {
	return _fcmem->get_result();
}
