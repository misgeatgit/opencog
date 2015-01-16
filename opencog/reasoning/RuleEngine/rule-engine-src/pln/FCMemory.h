/*
 * FCMemory.h
 *
 *  Created on: 15 Jan, 2015
 *      Author: misgana
 */

#ifndef FCMEMORY_H_
#define FCMEMORY_H_

#include <opencog/reasoning/RuleEngine/rule-engine-src/Rule.h>

using namespace opencog;

struct inference {
	int step;
	Rule* applied_rule;
	HandleSeq inf_product;
};

class ForwardChainer;
class FCMemory {
private:
	friend class ForwardChainer; //allow access to private
	bool _search_in_af;
	vector<Rule*> _rules; //loaded rules
	HandleSeq _target_list; //selected targets on each forward chaining steps
	Rule* _cur_rule;
	Handle _cur_target;
	vector<inference> _inf_history; //inference history
	void expand_target_list(HandleSeq input);
public:
	FCMemory();
	~FCMemory();
	vector<Rule*>& get_rules(void) ;
	HandleSeq get_target_list(void);
	bool is_search_in_af(void);
	Rule* get_cur_rule(void);
	void set_cur_rule(Rule* r) ;
	void add_rules_product(HandleSeq product);
	Handle get_cur_target(void) ;
	bool is_in_target(Handle h);
};

#endif /* FCMEMORY_H_ */
