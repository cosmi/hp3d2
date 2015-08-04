//
//  CellDividerGenerators.h
//  HP3d
//
//  Created by Marcin on 03.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellDividerGenerators__
#define __HP3d__CellDividerGenerators__


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
#include "CellSeparators.h"




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

template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const OverPlaneFilter<DIMS>& filter) {
    return os << "OverPlaneFilter[" << filter.dim << ':' << filter.val << ']';
}


template<int DIMS>
std::vector<CellId<DIMS> > getSeparatorPlanes(const CellIdSet<DIMS>& set) {
    using CellId = ::CellId<DIMS>;
    CellId bounds = set.getBounds();
    
    std::unordered_set<CellId> candidates;
    
    for(auto& cid: set) {
        FOR(dim, DIMS) {
            auto from = cid.getFrom()[dim];
            auto to = cid.getTo()[dim];
            candidates.insert(bounds.withDimension(dim, from, from));
            candidates.insert(bounds.withDimension(dim, to, to));
        }
    }
    
    std::vector<CellId> ret;
    for(auto& cid: candidates) {
        
//        std::cout << "CAND: " << cid << std::endl;
        if(!cid.isSideOf(bounds) &&
           set.canBeSplitByHiperplane(cid)) {
            ret.push_back(cid);
        }
    }
    
//    std::cout << "TEST "<< set << std::endl;
    
    return ret;
}


template<int DIMS>
struct PlaneDividersGenerator {
    using IdSet = CellIdSet<DIMS>;
    using Filter = OverPlaneFilter<DIMS>;
    using Separator = FilterSeparator<DIMS, Filter>;
    std::vector<Separator> separators;
    PlaneDividersGenerator(const IdSet & set) {
        auto planes = getSeparatorPlanes(set);
        for(auto& plane: planes) {
            separators.emplace_back(Separator(Filter(plane)));
        }
    }
    
    std::vector<Separator> getDividers() {
        return separators;
    }
};


#endif /* defined(__HP3d__CellDividerGenerators__) */
