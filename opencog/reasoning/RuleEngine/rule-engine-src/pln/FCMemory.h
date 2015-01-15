/*
 * FCMemory.h
 *
 *  Created on: 15 Jan, 2015
 *      Author: misgana
 */

#ifndef FCMEMORY_H_
#define FCMEMORY_H_

#include "ForwardChainer.h"
#include <opencog/reasoning/RuleEngine/rule-engine-src/Rule.h>

using namespace opencog;

struct inference {
	int step;
	Rule* applied_rule;
	HandleSeq inf_product;
};

class FCMemory {
private:
	friend class ForwardChainer; //allow access to private
	bool _search_in_af;
	vector<Rule*> _rules; //loaded rules
	HandleSeq _target_list; //selected targets on each forward chaining steps
	Rule* _cur_rule;
	Handle _cur_target;
	vector<inference> _inf_history; //inference history

	void expand_target_list(HandleSeq input) {
		_target_list.insert(_target_list.end(), input.begin(), input.end());
	}
public:
	FCMemory();
	~FCMemory();
	vector<Rule*>& get_rules(void) {
		return _rules;
	}
	HandleSeq get_target_list(void) {
		return _target_list;
	}
	bool is_search_in_af(void) {
		return _search_in_af;
	}
	Rule* get_cur_rule(void) {
		return _cur_rule;
	}
	void set_cur_rule(Rule* r) {
		_cur_rule = r;
	}
	void add_currule_product(HandleSeq product) {
		for (Handle p : product) {
			inference inf;
			inf.applied_rule = _cur_rule;
			inf.inf_product.push_back(p);
			_inf_history.push_back(inf);
		}
	}
	Handle get_cur_target(void) {
		return _cur_target;
	}
};

#endif /* FCMEMORY_H_ */
