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
#include "CellSeparators.h"
#include "CostFunction.h"


template<int DIMS>
class DivisionTree {
public:
    using ptr = std::shared_ptr<DivisionTree>;
protected:
    CellIdSet<DIMS> ids;
    ptr ch[2];
    result_t cost;

    DivisionTree(CellIdSet<DIMS> ids, const ptr& a, const ptr& b):ids(ids), cost(0) {
        ch[0] = a; ch[1] = b;
    }
    DivisionTree(CellIdSet<DIMS> ids):ids(ids), cost(0) {
        assert(ids.size() == 1);
    }
public:
    void setCost(result_t c) {
        cost = c;
    }
    const result_t& getCost() const {
        return cost;
    }
    static ptr build(CellIdSet<DIMS> ids, const ptr& a, const ptr& b) {
        return ptr(new DivisionTree(ids, a, b));
    }
    
    static ptr build(CellIdSet<DIMS> ids) {
        return ptr(new DivisionTree(ids));
    }
    void printToStream(std::ostream& os, int depth = 0) const {
        FOR(i, depth) os << "  ";
        if(ids.size() > 1) {
            os << '(' << ids.size() << ')' << " = " << cost << std::endl;
        } else {
            os << '(' << *ids.begin() << ')' << " = " << cost << std::endl;
        }
        if(ch[0] != nullptr) ch[0]->printToStream(os, depth+1);
        if(ch[1] != nullptr) ch[1]->printToStream(os, depth+1);
    }
};

template<int DIMS, class CostFunction = FlopsFunction, class CellType = Cell<DIMS>, class CellSpace = CellNodeSpace<DIMS, CellType> >
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
    using result = typename DivisionTree::ptr;
    using result_pair = std::pair<result, NodeCounter>;
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
    virtual result_pair calculateStrategy(const IdSet& ids) {
        return this->calculateStrategyImpl(ids);
    };
    virtual result_pair calculateStrategyImpl(const IdSet& ids) = 0;
    virtual ~AbstractStrategy(){};
    
    virtual result calculateStrategy() {
        auto leaves = cs.getLeavesIds();
        auto res = calculateStrategy(IdSet(leaves.begin(), leaves.end()));
        assert(res.second.empty());
        return res.first;
    }
    
    
};

template<int DIMS,
         class DivisionFunction = NestedDissectionSeparator<DIMS>,
         class CostFunction = FlopsFunction,
         class CellType = Cell<DIMS>,
         class CellSpace = CellNodeSpace<DIMS, CellType> >
class NestedDivisionStrategy : public AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> {
    protected:
    using Strategy = ::AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> ;
    DivisionFunction divider;
    
    using DivisionTree = typename Strategy::DivisionTree;
    using CellId = ::CellId<DIMS>;
    using result = typename Strategy::result;
    using result_pair = typename Strategy::result_pair;
    using IdSet = typename Strategy::IdSet;
    
    virtual result_pair calculateStrategyImpl(const IdSet& ids) {
        assert(ids.size() > 0);
        result_pair res;
        result_t innerCost = 0;
        if(ids.size() == 1) {
            auto& cid = *(ids.begin());
            res = result_pair(DivisionTree::build(ids),
                              this->nodesForSingleCell(cid));
        } else {
            auto subsets = this->divider(ids);
            
            auto r1 = this->calculateStrategy(subsets.first);
            auto r2 = this->calculateStrategy(subsets.second);
            
            innerCost += r1.first->getCost() + r2.first->getCost();
            res = result_pair(DivisionTree::build(ids, r1.first, r2.first),
                              this->combine(r1.second, r2.second));
        }
        size_t all = res.second.size();
        size_t reduced = this->reduce(res.second);
        res.first->setCost(this->calculateStepCost(all, reduced) + innerCost);
        
        return res;
    }
    
public:
    NestedDivisionStrategy(const CellSpace& cs,
                           const DivisionFunction& divider = DivisionFunction(),
                           const CostFunction& cf = CostFunction())
    : Strategy(cs, cf), divider(divider) {}

};

#endif /* defined(__HP3d__DivisionStrategy__) */
