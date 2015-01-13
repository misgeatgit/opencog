/*
 * DefaultForwardChainerCB.h
 *
 *  Created on: 14 Jan, 2015
 *      Author: misgana
 */

#ifndef DEFAULTFORWARDCHAINERCB_H_
#define DEFAULTFORWARDCHAINERCB_H_

#include "ForwardChainerCallBack.h"

class DefaultForwardChainerCB: public virtual ForwardChainerCallBack {
private:
	map<Handle, string> choose_variable(Handle h);
	Handle target_to_pmimplicant(Handle htarget,	map<Handle, string> hnode_vname_map);
	void add_to_target_list(Handle h);
	Handle tournament_select(map<Handle, float> hfitnes_map)
public:
	DefaultForwardChainerCB(AtomSpace* as, vector<Rule*> rules,
			HandleSeq premise);
	virtual ~DefaultForwardChainerCB();

	virtual Rule& choose_rule(Handle target);
	virtual void choose_input(Handle target);
	virtual Handle choose_next_target();
};

#endif /* DEFAULTFORWARDCHAINERCB_H_ */
