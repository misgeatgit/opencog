/*
 * FCMemory.h
 *
 *  Created on: 15 Jan, 2015
 *      Author: misgana
 */

#ifndef FCMEMORY_H_
#define FCMEMORY_H_

#include <opencog/reasoning/RuleEngine/rule-engine-src/Rule.h>
#include <opencog/atomspace/AtomSpace.h>

struct Inference {
	int step;
	Rule* applied_rule;
	HandleSeq inf_product;
};

using namespace opencog;

class ForwardChainer;
class FCMemory {
private:
	friend class ForwardChainer; //allow access to private
	bool _search_in_af;
	vector<Rule*> _rules; //loaded rules
	HandleSeq _target_list; //selected targets on each forward chaining steps
	Rule* _cur_rule;
	Handle _cur_target;
	vector<Inference> _inf_history; //inference history
	AtomSpace* _as;
	void update_target_list(HandleSeq input);
public:
	FCMemory(AtomSpace* as);
	~FCMemory();
	vector<Rule*>& get_rules(void) ;
	void set_target(Handle target);
	HandleSeq get_target_list(void);
	bool is_search_in_af(void);
	Rule* get_cur_rule(void);
	void set_cur_rule(Rule* r) ;
	void add_rules_product(HandleSeq product);
	Handle get_cur_target(void) ;
	bool isin_target_list(Handle h);
	HandleSeq get_result(void);
};

#endif /* FCMEMORY_H_ */
