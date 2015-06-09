//
//  DivisionStrategy.h
//  HP3d
//
//  Created by Marcin on 09.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__DivisionStrategy__
#define __HP3d__DivisionStrategy__

#include <algorithm>
#include <memory>
#include "defs.h"
#include "CellIdSet.h"
#include "Cell.h"


template<int DIMS>
class DivisionTree {
public:
    using ptr = std::shared_ptr<DivisionTree>;
protected:
    CellIdSet<DIMS> ids;
    ptr ch[2];
public:
    DivisionTree() {
    }
    DivisionTree(CellIdSet<DIMS> ids, const ptr& a, const ptr& b):ids(ids) {
        ch[0] = a; ch[1] = b;
    }
};

template<int DIMS, class CellSpace, class CellType = Cell<DIMS> >
class AbstractStrategy {
protected:
    const CellSpace& cs;
    AbstractStrategy(const CellSpace& cs):cs(cs){}
    using DivisionTree = ::DivisionTree<DIMS>;
    using result_pair = std::pair<typename DivisionTree::ptr, result_t>;
    using IdSet = ::CellIdSet<DIMS>;
    result_pair calculateDivisionStrategy() {
        return calculateDivisionStrategy(IdSet(cs));
    }
};

template<int DIMS, class CellSpace, class CellType = Cell<DIMS> >
class NestedDissectionStrategy : public AbstractStrategy<DIMS, CellSpace, CellType> {
public:
    using AbstractStrategy = ::AbstractStrategy<DIMS, CellSpace, CellType>;
    NestedDissectionStrategy(const CellSpace& cs):AbstractStrategy(cs) {}
    
    using DivisionTree = ::DivisionTree<DIMS>;
    using result_pair = std::pair<typename DivisionTree::ptr, result_t>;
    using CellId = ::CellId<DIMS>;
    using IdSet = ::CellIdSet<DIMS>;
    result_pair calculateDivisionStrategy(const IdSet& ids) {
        auto bounds = CellId::getBounds(ids.getIds().begin(), ids.getIds().end());
        
    }
};

#endif /* defined(__HP3d__DivisionStrategy__) */
