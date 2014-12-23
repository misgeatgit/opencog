/*
 * ControlPolicy.cc
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
#include "ControlPolicy.h"

#include <opencog/guile/load-file.h>
#include <opencog/util/misc.h>
#include <opencog/util/Config.h>
#include <opencog/guile/SchemeEval.h>

ControlPolicy::ControlPolicy(AtomSpace * as) :
		as_(as) {
}

ControlPolicy::~ControlPolicy() {
}

void ControlPolicy::load_config(std::string& conf_path) {
	try {
		config().load(conf_path.c_str());
	} catch (RuntimeException &e) {
		std::cerr << e.getMessage() << std::endl;
	}
	vector<string> rules_str;
	//FCHAIN_RULES= "[blink-var1,blink-var1,...]:rule_path1","[blink-var2]:rule_path2",...
	tokenize(config()["FCHAIN_RULES"], back_inserter(rules_str), ", ");
	if (!rules_str.empty()) {
		for (string rule : rules_str) {
			auto it = remove_if(rule.begin(), rule.end(),
					[](char c) {return (c==']' or c=='[' or c=='"');});
			rule.erase(it, rule.end());

			vector<string> varlist_rule;
			tokenize(rule, back_inserter(varlist_rule), ":");
			assert(varlist_rule.size() == 2);
			load_scm_file_relative(*as_, varlist_rule[1], vector<string>(0)); // load rules to the chaining processor atomspace (i.e target_list_atom_space)
			string vars = varlist_rule[0];
			istringstream is(vars);
			string var_name;
			vector<string> rule_name;
			map<string, Rule*> strname_rule_map;
			while (getline(is, var_name, ',')) {
				SchemeEval scm_eval = new SchemeEval(as_);
				rule_name.push_back(var_name);
				Rule r = Rule(scm_eval->eval_h(var_name));
				rules_.push_back(r);
				strname_rule_map[var_name] = &r;
			}

			//MORE CONFIG PARAM LOADING ...
			max_iter_ = config().get_int("ITERATION_SIZE");
			attention_alloc_ = config().get_bool(
					"ATTENTION_ALLOCATION_ENABLED"); //informs the callbacks to look for atoms only on the attentional focus

			vector<string> str_mutex_rules;
			//FCHAIN_RULES= "[blink-var1,blink-var1,...]:rule_path1","[blink-var2]:rule_path2",...
			tokenize(config()["MUTEX"], back_inserter(str_mutex_rules), ", ");


			for (string r : str_mutex_rules) {
				auto it = remove_if(r.begin(), r.end(),
						[](char c) {return (c==']' or c=='[' or c=='"');});
				rule.erase(it, rule.end());
				istringstream is(vars);
				string var_name;
				vector<Rule> mutexes;
				//make sure the mutexes are already declared in FCHAIN_RULES param
				while (getline(is, var_name, ','))
					if (find(rule_name.begin(), rule_name.end(), var_name)
							== rule_name.end())
						throw(InvalidParamException);
					else
						mutexes.push_back(strname_rule_map[var_name]);
				mutex_rules.push_back(mutexes);
			}

		}
	}
}
