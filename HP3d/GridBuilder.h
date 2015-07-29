//
//  GridBuilder.h
//  HP3d
//
//  Created by Marcin on 06.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__GridBuilder__
#define __HP3d__GridBuilder__

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <numeric>

#include "defs.h"
#include "Cell.h"
#include "CellSpace.h"

template<int DIMS, class Cell = ::Cell<DIMS> >
void buildSingularityHelper(CellSpace<DIMS, Cell>& cs, int dims, int dim, CellId<DIMS> id) {
    if(dim == dims) {
        cs.splitTo(id);
    } else {
        auto bounds = cs.getBounds();
        while(bounds.covers(id)) {
            buildSingularityHelper(cs, dims, dim+1, id);
            id = id.move(dim, 1);
        }
    }
}

template<int DIMS, class Cell = ::Cell<DIMS> >
void buildSingularity(CellSpace<DIMS, Cell>& cs, int dims) {
    buildSingularityHelper(cs, dims, 0, CellId<DIMS>::unit());
}


//TODO: tau rule should be enforced also across corners
template<int DIMS, class Cell = ::Cell<DIMS> >
void enforceExtendedTauRuleHelper(CellSpace<DIMS, Cell>& cs, const CellId<DIMS> & id, int commonDims) {
    if(!cs.hasCell(id)){
        auto parent = id.getAlignedParent();
        enforceExtendedTauRuleHelper(cs, parent, commonDims);
        cs.splitTo(id);
        FOR(i, id.getChildrenCount()) {
            auto cid = parent.getChild(i);
            if(cid == id) continue;
            enforceExtendedTauRuleHelper(cs, cid, commonDims);
        }
    }


//            std::cout << "NOOODE " << id << std::endl;
//    if(id.getTo() == Point<DIMS>({8,8}) && id.getSide() == 2) {
//
//    }
    auto neighbors = id.getSameSizeNeighbors();
    { // JUST ASSERTION:
        int q = 1;
        FOR(i, DIMS) q*=3;
        assert(neighbors.size() == q);
    }
    auto bounds = CellId<DIMS>::getBounds(neighbors.begin(), neighbors.end());
    using namespace std;
//    cout << "FOR " << id << endl;
//    for(auto cid : neighbors) cout << "> " << cid << endl;
    assert(id.getSize()*3 == bounds.getSize());
    
    for(CellId<DIMS> & cid : neighbors) {
        
        auto commons = CellId<DIMS>::intersection(id, cid);
        if(commons.getDimensionality() < commonDims) continue;
        
        auto parent = cid.getAlignedParent();
        
//        cout << "P: " << parent << " C: " << cid << endl;
        if(cs.hasCell(parent)) continue;
        if(cs.covers(parent)) {
            enforceExtendedTauRuleHelper(cs, parent, commonDims);
        }
    }
//    FOR(dim, DIMS) {
//        auto p1 = id.move(dim, -1);
//        if(cs.covers(p1)) enforceExtendedTauRuleHelper(cs, p1.getAlignedParent());
//        auto p2 = id.move(dim, +1);
//        if(cs.covers(p2)) enforceExtendedTauRuleHelper(cs, p2.getAlignedParent());
//    }
}

template<int DIMS, class Cell = ::Cell<DIMS> >
void enforceExtendedTauRule(CellSpace<DIMS, Cell>& cs, int commonDims = 0) {
    std::vector<CellId<DIMS> > v;
    v.reserve(cs.countCells());
    const auto& ids = cs.getLeavesIds();
    v.insert(v.begin(), ids.begin(), ids.end());
    for(auto & i : v) {
        enforceExtendedTauRuleHelper(cs, i, commonDims);
//        std:: cout << "==========" << std::endl;
    }
}


#endif /* defined(__HP3d__GridBuilder__) */
