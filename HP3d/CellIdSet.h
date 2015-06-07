//
//  CellIdSet.h
//  HP3d
//
//  Created by Marcin on 06.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellIdSet__
#define __HP3d__CellIdSet__

#include <algorithm>
#include <iostream>
#include <set>

#include "defs.h"
#include "CellId.h"
#include "CellSpace.h"

template<int DIMS>
struct CellIdComparator: public std::binary_function<CellId<DIMS>, CellId<DIMS>, bool> {
    size_t operator()(const CellId<DIMS>& id1, const CellId<DIMS>& id2) const {
        auto sizeDiff = id2.getSide() - id1.getSide();
        if(sizeDiff != 0) return 0 < sizeDiff;
        auto& from1 = id1.getFrom(), &from2 = id2.getFrom();
        FOR(i, DIMS) {
            if(from1[i] != from2[i]) return from1[i] < from2[i];
        }
        return false;
    }
};

template<int DIMS>
class CellIdSet {
    using SetType = std::set<CellId<DIMS>, CellIdComparator<DIMS> >;
    using CellId = ::CellId<DIMS>;
protected:
    SetType ids;
public:
    const SetType& getIds() const {
        return ids;
    }
    
    bool addId(const CellId& cid) {
        return ids.insert(cid).second;
    }
    template<class CellType>
    CellIdSet(const CellSpace<DIMS, CellType>& cs) {
        auto leaves = cs.getLeavesIds();
        ids.insert(leaves.begin(), leaves.end());
    }
    CellIdSet(){}
};

namespace std {
    template<int DIMS>
    struct hash<CellIdSet<DIMS> >: public unary_function<CellIdSet<DIMS>, size_t> {
        static const hash<CellId<DIMS> > subHash;
        size_t operator()(const CellIdSet<DIMS>& ids) const {
            size_t ret = 0;
            const size_t PRIME = 512927377;
            for(auto & id : ids.getIds()) {
                ret*=PRIME;
                ret+=subHash(id);
            }
            return ret;
        }
    };
}



#endif /* defined(__HP3d__CellIdSet__) */
