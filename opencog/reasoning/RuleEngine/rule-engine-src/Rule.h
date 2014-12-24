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

using namespace opencog;

/**
 * A wrapper class for Handles of rule
 */
class Rule {
private:
	Handle rule_handle_;
	int priority_;
public:
	Handle get_rule_handle();
	int get_priority();
	/**
	 * Validates whether  the Handle is of type ImplicationLink or BindLink and then sets @param rule_
	 */
	Rule(Handle rule) throw(InvalidParamException);
	virtual ~Rule();
};

#endif /* RULE_H_ */
