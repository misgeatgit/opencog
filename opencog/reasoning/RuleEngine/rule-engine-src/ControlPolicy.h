/*
 * ControlPolicy.h
 *
 * Copyright (C) 2014 Misgana Bayetta
 *
 * Author: Misgana Bayetta <misgana.bayetta@gmail.com>  Sept 2014
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
#ifndef CONTROL_POLICY_
#define CONTROL_POLICY_
/*what do we need? load stuffs. what kind of stuffs.
 -(load-pln with this config file)
 -then what rules use
 -what are rules are mutually exclusive
 */
#include "Rule.h"

using namespace opencog;
using namespace std;

class ControlPolicy {
private:
	//list of control policy parameters
	vector<Rule*> rules_;
	vector<vector<Rule*>> mutex_sets_;
	map<string, Rule*> strname_rule_map_;
	int max_iter_;
	bool attention_alloc_;
	AtomSpace * as_;
	void load_chaining_rules();
	void load_mutexes();
	void load_single_val_params();
public:
	void load_config(string& conf_path);
	ControlPolicy(AtomSpace* as, string conf_path);
	~ControlPolicy();
	int get_max_iter(void);
	bool get_attention_alloc(void);
	vector<vector<Rule*>> get_mutex_sets(void);
	vector<Rule*> get_rules(void);
};

#endif

