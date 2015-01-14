/*
 * Rule.h
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

#ifndef RULE_H_
#define RULE_H_

/*#include <string>
#include <vector>*/
#include <opencog/atomspace/Handle.h>

using namespace opencog;
using namespace std;
/**
 * A wrapper class for rule Handles
 */
class Rule {
private:
	Handle rule_handle_;
	string name_;
	string category_;
	int priority_ = -1;
	vector<Rule*> mutex_rules_;
public:
	Handle get_handle();
	/**
	 * Validates whether  the Handle is of type ImplicationLink or BindLink and then sets @param rule_
	 */
	Rule(Handle rule);
	Rule();
	void set_rule_handle(Handle h) throw (exception);
	void set_name(string name);
	string get_name();
	void set_category(string name);
	string& get_category();
	void set_priority(int p);
	int get_priority();
	void add_mutex_rule(Rule* r);
	vector<Rule*> get_mutex_rules(void);
	virtual ~Rule();
};

#endif /* RULE_H_ */
