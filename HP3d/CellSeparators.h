//
//  CellSeparators.h
//  HP3d
//
//  Created by Marcin on 07.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellSeparators__
#define __HP3d__CellSeparators__

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <numeric>

#include "defs.h"
#include "Cell.h"
#include "CellSpace.h"
#include "CellIdSet.h"


template <int DIMS>
struct InBoundsFilter {
    using CellId = ::CellId<DIMS>;
    CellId bounds;
    InBoundsFilter(const CellId& cid) : bounds(cid){}
    bool operator()(const CellId& ct) const {
        return bounds.covers(ct);
    }
};

template <int DIMS>
std::pair<CellIdSet<DIMS>, CellIdSet<DIMS> > splitSet(const CellIdSet<DIMS>& cs, const CellId<DIMS>& bounds) {
    std::pair<CellIdSet<DIMS>, CellIdSet<DIMS> > ret;
    
    for(auto & id : cs.getIds()) {
        if(bounds.covers(id)) {
            ret.first.addId(id);
        } else {
            ret.second.addId(id);
        }
    }
    
    return ret;
}


#endif /* defined(__HP3d__CellSeparators__) */
