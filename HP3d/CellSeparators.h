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

template<int DIMS>
struct NestedBisectionSeparator {
    using IdSet = CellIdSet<DIMS>;
    std::pair<IdSet, IdSet> operator()(const IdSet& ids) const {
        auto bounds = CellId<DIMS>::getBounds(ids.begin(), ids.end());
        auto half = bounds.getHalf();
        return ids.splitBy(InBoundsFilter<DIMS>(half));
    }
};




template<int DIMS>
struct ArbitrarySeparator {
    struct ArbitraryFilter {
        using CellId = ::CellId<DIMS>;
        int a;
        ArbitraryFilter():a(0){}
        bool operator()(const CellId& ct) const {
            (*const_cast<int*>(&a))++;
            return (a&1);
        }
    };
    using IdSet = CellIdSet<DIMS>;
    std::pair<IdSet, IdSet> operator()(const IdSet& ids) const {
        return ids.splitBy(ArbitraryFilter());
    }
};
//
//
//template <int DIMS>
//std::pair<CellIdSet<DIMS>, CellIdSet<DIMS> > splitSet(const CellIdSet<DIMS>& cs, const CellId<DIMS>& bounds) {
//    std::pair<CellIdSet<DIMS>, CellIdSet<DIMS> > ret;
//    
//    for(auto & id : cs.getIds()) {
//        if(bounds.covers(id)) {
//            ret.first.addId(id);
//        } else {
//            ret.second.addId(id);
//        }
//    }
//    
//    return ret;
//}


#endif /* defined(__HP3d__CellSeparators__) */
