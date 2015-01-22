/*
 * FCMemory.cc
 *
 *  Created on: 16 Jan, 2015
 *      Author: misgana
 */

#include "FCMemory.h"

FCMemory::FCMemory(AtomSpace* as) {
	_as = as;
}

FCMemory::~FCMemory() {
}

void FCMemory::update_target_list(HandleSeq input) {
	for (Handle i : input) {
		if (find(_target_list.begin(), _target_list.end(), i)
				== _target_list.end())
			_target_list.push_back(i);
	}
}

vector<Rule*>& FCMemory::get_rules(void) {
	return _rules;
}
void FCMemory::set_target(Handle target){
  _cur_target = target;
  _target_list.push_back(_cur_target);
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
		Inference inf;
		inf.applied_rule = _cur_rule;
		inf.inf_product.push_back(p);
		_inf_history.push_back(inf);
	}
}
Handle FCMemory::get_cur_target(void) {
	return _cur_target;
}

bool FCMemory::isin_target_list(Handle h) {
	for (Handle hi : _target_list) {
		//recursive lookup
		/*if (LinkCast(hi)) {
		 HandleSeqSeq hseqs;
		 hseqs.push_back(_as->getOutgoing(hi));
		 HandleSeq iset;
		 do {
		 HandleSeq iset = hseqs[hseqs.size() - 1];
		 hseqs.pop_back();
		 for (Handle i : iset) {
		 if (LinkCast(i))
		 hseqs.push_back(_as->getOutgoing(i));
		 else if (i.value() == h.value())
		 return true;
		 }
		 } while (not hseqs.empty());
		 } else */
		if (hi.value() == h.value())
			return true;
	}
	return false;
}

HandleSeq FCMemory::get_result() {
	HandleSeq result;
	for (Inference i : _inf_history)
		result.insert(result.end(),i.inf_product.begin(), i.inf_product.end());
	return result;
}
