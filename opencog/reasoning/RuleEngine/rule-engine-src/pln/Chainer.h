/*
 * Chainer.h
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
#ifndef CHAINER_H_
#define CHAINER_H_

#include <opencog/query/PatternMatch.h>

using namespace opencog;

/**
 * Abstract class for forward and backward chaining
 */
class Chainer {
private:
protected:
	AtomSpace * _main_atom_space; // knowledge base atomspace
	AtomSpace * _target_list_atom_space; //supposed to store selected premises
	Handle _initial_target;
	PatternMatch _pattern_matcher;
    std::string _conf_path ="default_cpolicy.json";
public:
	Chainer(AtomSpace *);
	virtual ~Chainer();
	void set_htarget(Handle& h);
};
#endif /* CHAINER_H_ */
