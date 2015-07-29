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
#include <unordered_set>
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
struct OverPlaneFilter {
    using CellId = ::CellId<DIMS>;
    dim_t val;
    size_t dim;
    OverPlaneFilter(const CellId& plane){
        FOR(i, DIMS) {
            if(plane.getFrom()[i] == plane.getTo()[i]) {
                this->dim = i;
                this->val = plane.getFrom()[i];
                break;
            }
        }
    }
    bool operator()(const CellId& ct) const {
        return ct.getFrom()[this->dim] >= val;
    }
};


template<int DIMS>
std::vector<CellId<DIMS> > getSeparatorPlanes(const CellIdSet<DIMS>& set) {
    using CellId = ::CellId<DIMS>;
    CellId bounds = set.getBounds();
    
    std::unordered_set<CellId> candidates;

    for(auto& cid: set) {
        FOR(dim, DIMS) {
            auto val = cid.getFrom()[dim];
            candidates.push_back(bounds.withDimension(dim, val));
        }
    }
    
    std::vector<CellId> ret;
    for(auto& cid: candidates) {
        if(set.canBeSplitByHiperplane(cid)) {
            ret.push_back(cid);
        }
    }
    
    return ret;
}


template<int DIMS>
struct PlaneDivisorsGenerator {
    using IdSet = CellIdSet<DIMS>;
    std::vector<OverPlaneFilter<DIMS> > separators;
    PlaneDivisorsGenerator(const IdSet & set) {
        auto planes = getSeparatorPlanes(set);
        for(auto& plane: planes) {
            separators.emplace_back(plane);
        }
    }
    
    std::vector<OverPlaneFilter<DIMS> > getDividers() {
        return separators;
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
