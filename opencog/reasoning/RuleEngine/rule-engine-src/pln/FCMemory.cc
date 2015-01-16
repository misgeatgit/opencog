/*
 * FCMemory.cc
 *
 *  Created on: 16 Jan, 2015
 *      Author: misgana
 */

#include "FCMemory.h"

FCMemory::FCMemory() {
}

FCMemory::~FCMemory() {
}

void FCMemory::expand_target_list(HandleSeq input) {
	_target_list.insert(_target_list.end(), input.begin(), input.end());
}

vector<Rule*>& FCMemory::get_rules(void) {
	return _rules;
}
HandleSeq FCMemory::get_target_list(void) {
	return _target_list;
}
bool FCMemory::is_search_in_af(void) {
	return _search_in_af;
}
Rule* FCMemory::get_cur_rule(void) {
	return _cur_rule;
}
void FCMemory::set_cur_rule(Rule* r) {
	_cur_rule = r;
}
void FCMemory::add_rules_product(HandleSeq product) {
	for (Handle p : product) {
		inference inf;
		inf.applied_rule = _cur_rule;
		inf.inf_product.push_back(p);
		_inf_history.push_back(inf);
	}
}
Handle FCMemory::get_cur_target(void) {
	return _cur_target;
}

bool FCMemory::is_in_target(Handle h) {
	for (Handle hi : _target_list) {
		if (hi.value() == h.value())
			return true;
	}
	return false;
}
