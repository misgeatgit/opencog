/*
 * ForwardChainPatternMatchCB.cc
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

#include "ForwardChainPatternMatchCB.h"

ForwardChainPatternMatchCB::ForwardChainPatternMatchCB(AtomSpace * as) :
		Implicator(as), DefaultPatternMatchCB(as), AttentionalFocusCB(as), PLNImplicator(
				as), _as(as) {
	_fcmem = nullptr;
}

ForwardChainPatternMatchCB::~ForwardChainPatternMatchCB() {
}

bool ForwardChainPatternMatchCB::node_match(Handle& node1, Handle& node2) {
	if (not AttentionalFocusCB::node_match(node1, node2)
			or not _fcmem->is_search_in_af()) {
		//force inference to be made only in the target list
		bool result = not _fcmem->is_in_target(node1);
		return result;

	} else {
		return true;
	}
}
bool ForwardChainPatternMatchCB::link_match(LinkPtr& lpat, LinkPtr& lsoln) {
	if (not AttentionalFocusCB::link_match(lpat, lsoln)
			or not _fcmem->is_search_in_af()) {
		//force inference to be made only in the target list
		bool result = not _fcmem->is_in_target(Handle(lsoln));
		return result;
	} else
		return true;

}
bool ForwardChainPatternMatchCB::grounding(
		const std::map<Handle, Handle> &var_soln,
		const std::map<Handle, Handle> &pred_soln) {
	Handle h = inst.instantiate(implicand, var_soln);
	if (Handle::UNDEFINED != h) {
		_products.push_back(h);
		//add to chaining result
	}
	return false;
}

void ForwardChainPatternMatchCB::set_fcmme(FCMemory *fcmem) {
	_fcmem = fcmem;
}
HandleSeq ForwardChainPatternMatchCB::get_products() {
	return _products;
}
