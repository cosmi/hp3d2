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

template<int DIMS, class Cell = ::Cell<DIMS> >
void enforceTauRuleHelper(CellSpace<DIMS, Cell>& cs, const CellId<DIMS> & id) {
    cs.splitTo(id);
    FOR(dim, DIMS) {
        auto p1 = id.move(dim, -1);
        if(cs.covers(p1)) enforceTauRuleHelper(cs, p1.getAlignedParent());
        auto p2 = id.move(dim, +1);
        if(cs.covers(p2)) enforceTauRuleHelper(cs, p2.getAlignedParent());
    }
}
template<int DIMS, class Cell = ::Cell<DIMS> >
void enforceTauRule(CellSpace<DIMS, Cell>& cs) {
    std::vector<CellId<DIMS> > v;
    v.reserve(cs.countCells());
    const auto& ids = cs.getIds();
    v.insert(v.begin(), ids.begin(), ids.end());
    for(auto & i : v) {
        enforceTauRuleHelper(cs, i);
    }
}


#endif /* defined(__HP3d__GridBuilder__) */
