/*
 * DefaultForwardChainerCB.h
 *
 *  Created on: 14 Jan, 2015
 *      Author: misgana
 */

#ifndef DEFAULTFORWARDCHAINERCB_H_
#define DEFAULTFORWARDCHAINERCB_H_

#include "ForwardChainerCallBack.h"
#include "FCMemory.h"

class DefaultForwardChainerCB: public virtual ForwardChainerCallBack {
private:
	map<Handle, string> choose_variable(Handle h);
	Handle target_to_pmimplicant(Handle htarget,
			map<Handle, string> hnode_vname_map);
	void add_to_target_list(Handle h);
	Handle tournament_select(map<Handle, float> hfitnes_map);
public:
	DefaultForwardChainerCB(AtomSpace* as);
	virtual ~DefaultForwardChainerCB();

//callbacks
	virtual Rule& choose_rule(FCMemory& fcm);
	virtual HandleSeq choose_input(FCMemory& fcm);
	virtual Handle choose_next_target(FCMemory& fcm);
	virtual HandleSeq apply_rule(FCMemory& fcm);
};

#endif /* DEFAULTFORWARDCHAINERCB_H_ */
