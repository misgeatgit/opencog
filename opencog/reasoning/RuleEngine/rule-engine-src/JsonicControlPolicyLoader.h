/*
 * JsonicControlPolicyLoader.h
 *
 *  Created on: 29 Dec, 2014
 *      Author: misgana
 */

#ifndef JSONICCONTROLPOLICYLOADER_H_
#define JSONICCONTROLPOLICYLOADER_H_

#include "ControlPolicyLoader.h"
#include "Rule.h"

#include <lib/json_spirit/json_spirit.h>

using namespace opencog;
using namespace json_spirit;

class JsonicControlPolicyLoader:public virtual ControlPolicyLoader {
private:
	Rule* cur_read_rule_ = NULL;
	map<Rule*,vector<string>> rule_mutex_map_;
	void read_json(const Value &v, int level = -1);
	void read_array(const Value &v, int level);
	void read_obj(const Value &v, int level);
	void read_null(const Value &v, int level);
	template <typename T> void read_primitive(const Value &v,int level);
	/**
	 * sets the mutually exclusive rules
	 */
	void set_mutex_rules(void);
	Rule* get_rule(string& name);
public:
	JsonicControlPolicyLoader(AtomSpace* as, string conf_path);
	virtual ~JsonicControlPolicyLoader();
	virtual void load_config();
};

#endif /* JSONICCONTROLPOLICYLOADER_H_ */
