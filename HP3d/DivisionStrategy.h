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
#include "CellNodeSpace.h"


template<int DIMS>
class DivisionTree {
public:
    using ptr = std::shared_ptr<DivisionTree>;
protected:
    CellIdSet<DIMS> ids;
    ptr ch[2];

    DivisionTree(CellIdSet<DIMS> ids, const ptr& a, const ptr& b):ids(ids) {
        ch[0] = a; ch[1] = b;
    }
    DivisionTree(CellIdSet<DIMS> ids):ids(ids) {
        assert(ids.size() == 1);
    }
public:
    static ptr build(CellIdSet<DIMS> ids, const ptr& a, const ptr& b) {
        return ptr(new DivisionTree(ids, a, b));
    }
    
    static ptr build(CellIdSet<DIMS> ids) {
        return ptr(new DivisionTree(ids));
    }
    
    
    
};

template<int DIMS>
struct CellIdInBounds {
    CellId<DIMS> bounds;
    CellIdInBounds(const CellId<DIMS>& bounds):bounds(bounds){}
    bool operator()(const CellId<DIMS>& cell) const {
        return bounds.covers(cell);
    }
};


template<int DIMS, class CostFunction, class CellType = Cell<DIMS>, class CellSpace = CellNodeSpace<DIMS, CellType> >
class AbstractStrategy {
protected:
    const CellSpace& cs;
    const CellId<DIMS> bounds;
    const CostFunction cf;
    AbstractStrategy(const CellSpace& cs, const CostFunction& cf ):cs(cs), bounds(cs.getBounds()), cf(cf) {}
    
    const CellId<DIMS>& getBounds() const {
        return bounds;
    }
    
    using DivisionTree = ::DivisionTree<DIMS>;
    using CellId = ::CellId<DIMS>;
    
    using NodeCounter = std::unordered_map<CellId, size_t>;
    using result_pair = std::pair<typename DivisionTree::ptr, result_t>;
    using result_triple = std::pair<result_pair, NodeCounter>;
    using IdSet = ::CellIdSet<DIMS>;
    
    NodeCounter nodesForSingleCell(const CellId& cid) {
        NodeCounter nc;
        for(auto& node: cs.getNodesForCell(cid)) {
            nc[node] = 1;
        }
        return nc;
    }
    
    NodeCounter combine(const NodeCounter& a, const NodeCounter& b) const {
        NodeCounter nc(a);
        for(auto& pos: b) {
            nc[pos.first]+=pos.second;
        }
        return nc;
    }
    
    size_t reduce(NodeCounter& a) const {
        size_t reduced = 0;
        for (typename NodeCounter::iterator it = a.begin(); it != a.end(); ) {
            auto reqNodes = cs.countCellsForNode(it->first);
            if(reqNodes == it->second) {
                it = a.erase(it);
                reduced++;
            } else {
                ++it;
            }
        }
        return reduced;
    }
    
    result_t calculateStepCost(size_t all, size_t reduced) const {
        return cf(reduced, all);
    }
public:
    virtual result_triple calculateStrategy(const IdSet&) = 0;
    virtual ~AbstractStrategy(){};
    
    virtual result_pair calculateStrategy() {
        auto leaves = cs.getLeavesIds();
        auto res = calculateStrategy(IdSet(leaves.begin(), leaves.end()));
        assert(res.second.empty());
        return res.first;
    }
    
    
};

template<int DIMS, class CostFunction, class CellType = Cell<DIMS>, class CellSpace = CellNodeSpace<DIMS, CellType> >
class NestedDissectionStrategy : public AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> {
public:
    using Strategy = ::AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> ;
    NestedDissectionStrategy(const CellSpace& cs, const CostFunction& cf = CostFunction() ):Strategy(cs, cf) {}
    
    using DivisionTree = typename Strategy::DivisionTree;
    using CellId = ::CellId<DIMS>;
    using result_pair = typename Strategy::result_pair;
    using result_triple = typename Strategy::result_triple;
    using IdSet = typename Strategy::IdSet;
    
    virtual result_triple calculateStrategy(const IdSet& ids) {
        assert(ids.size() > 0);
        result_triple res;
        if(ids.size() == 1) {
            auto& cid = *(ids.begin());
            res = result_triple(result_pair(DivisionTree::build(ids), 0),
                                this->nodesForSingleCell(cid));
        } else {
            auto bounds = CellId::getBounds(ids.getIds().begin(), ids.getIds().end());
            auto half = bounds.getHalf();
            
            auto subsets = ids.splitBy(CellIdInBounds<DIMS>(half));
            
            auto r1 = calculateStrategy(subsets.first);
            auto r2 = calculateStrategy(subsets.second);
            
            res = result_triple(result_pair(DivisionTree::build(ids, r1.first.first, r2.first.first), 0),
                                this->combine(r1.second, r2.second));
        }
        size_t all = res.second.size();
        size_t reduced = this->reduce(res.second);
        res.first.second = this->calculateStepCost(all, reduced);
        
        return res;
    }
};

#endif /* defined(__HP3d__DivisionStrategy__) */
