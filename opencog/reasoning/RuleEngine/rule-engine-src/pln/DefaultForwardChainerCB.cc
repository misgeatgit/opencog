/*
 * DefaultForwardChainerCB.cc
 *
 *  Created on: 14 Jan, 2015
 *      Author: misgana
 */

#include "DefaultForwardChainerCB.h"

DefaultForwardChainerCB::DefaultForwardChainerCB(AtomSpace* as,
		vector<Rule*> rules, HandleSeq premise) :
		ForwardChainerCallBack(as, rules, premise) {

}

DefaultForwardChainerCB::~DefaultForwardChainerCB() {
}

//-----------------callbacks-------------------------------------------------------------------------------//
Rule& DefaultForwardChainerCB::choose_rule(Handle target) {
	//TODO choose rule via stochastic selection or fitness selection
	//TODO handle mutual exclusions
	Rule *choosen_rule = rules_[random() % rules_.size()];
	return choosen_rule;
	//auto mset = choosen_rule->get_mutex_rules();
}

void DefaultForwardChainerCB::choose_input(Handle htarget) {
	if (NodeCast(htarget)) {
		HandleSeq hs = as_->getIncoming(htarget);
		for (Handle h : hs)
			add_to_target_list(h); //add to potential target list
	}
	if (LinkCast(htarget)) {
		map<Handle, string> hnode_vname_map = choose_variable(htarget);
		Handle implicant = target_to_pmimplicant(htarget, hnode_vname_map);
		Handle bindLink = commons_->create_bindLink(implicant);
		chaining_pm_.do_bindlink(bindLink, *fcim_); //result is added to target_list in fcim_'s grounding call back handler
	}
}

Handle DefaultForwardChainerCB::choose_next_target() {
	map<Handle, float> tournament_elem;
	for (Handle h : target_list_) {
		float fitness = target_tv_fitness(h);
		tournament_elem[h] = fitness;
	}
	return tournament_select(tournament_elem);
}

//-------------------private methods------------------------------------------------------------------------------//

map<Handle, string> DefaultForwardChainerCB::choose_variable(Handle htarget) {
	vector<Handle> candidates = commons_->get_nodes(htarget, vector<Type>());
	map<Handle, HandleSeq> node_iset_map;
//xxx don't choose two or more nodes linked by one Link( i.e choose only one whenever
//there are more than one node linked by the same link)
	for (auto it = candidates.begin(); it != candidates.end(); ++it) {
		HandleSeq hs = as_->getIncoming(*it);
		if (distance(candidates.begin(), it) == 0) {
			node_iset_map[*it] = hs;
		} else {
			bool has_same_link = false;
			for (auto i = node_iset_map.begin(); i != node_iset_map.end();
					++i) {
				HandleSeq tmp;
				sort(hs.begin(), hs.end());
				sort(i->second.begin(), i->second.end());
				set_intersection(hs.begin(), hs.end(), i->second.begin(),
						i->second.end(), back_inserter((tmp)));
				if (tmp.size() > 0) {
					has_same_link = true;
					break;
				}
			}
			if (!has_same_link)
				node_iset_map[*it] = hs;
		}
	}
}

Handle DefaultForwardChainerCB::target_to_pmimplicant(Handle htarget,
		map<Handle, string> hnode_vname_map) {
	Type link_type;
	HandleSeq hsvariablized;

	if (LinkCast(htarget)) {
		LinkPtr p_htarget = LinkCast(htarget);
		link_type = p_htarget->getType();
		HandleSeq hsoutgoing = as_->getOutgoing(htarget);
		for (auto i = hsoutgoing.begin(); i != hsoutgoing.end(); ++i) {
			Handle htmp = target_to_pmimplicant(*i, hnode_vname_map);
			hsvariablized.push_back(htmp);
		}
		return as_->addLink(link_type, hsvariablized, TruthValue::TRUE_TV());
	} else {
		if (NodeCast(htarget)) {
			auto it_var = hnode_vname_map.find(htarget); //TODO replace by find-if for linear complexity
			NodePtr p_htarget = NodeCast(htarget);
			if (it_var != hnode_vname_map.end())
				return as_->addNode(VARIABLE_NODE, it_var->second,
						TruthValue::TRUE_TV());
			else
				return htarget;
		}
	}
	return Handle::UNDEFINED; //unreachable?
}

void DefaultForwardChainerCB::add_to_target_list(Handle h) {
	bool found = (find(target_list_.begin(), target_list_.end(), h)
			!= target_list_.end());
	if (NodeCast(h)) {
		if (found)
			target_list_.push_back(h);
	}
	if (LinkCast(h)) {
		if (found)
			target_list_.push_back(h);
		HandleSeq hs = as_->getOutgoing(h);
		for (Handle hi : hs)
			if (find(target_list_.begin(), target_list_.end(), hi)
					== target_list_.end())
				add_to_target_list(hi);
	}
}


//-----------helper functions------------------------------------------------------//

Handle DefaultForwardChainerCB::tournament_select(map<Handle, float> hfitnes_map) {
	if (hfitnes_map.size() == 1) {
		return hfitnes_map.begin()->first;
	}

	map<Handle, float> winners;
	int size = hfitnes_map.size() / 2; //TODO change the way tournament size is calculated
	for (auto i = 0; i < size; i++) {
		int index = (random() % hfitnes_map.size());
		auto it = hfitnes_map.begin();
		advance(it, index);
		winners[it->first] = it->second;
	}
	auto it = winners.begin();
	Handle hbest = it->first;
	float max = it->second;
	for (; it != winners.end(); ++it) {
		if (it->second > max) {
			hbest = it->first;
			max = it->second;
		}
	}
	return hbest;
}
