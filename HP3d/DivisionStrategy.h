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

template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const DivisionTree<DIMS>& tree) {
    tree.printToStream(os);
    return os;
}




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
    
    NodeCounter nodesForSingleCell(const CellId& cid) const {
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
    
    
    result_pair calculateStrategyForSingleId(const IdSet& ids) {
        assert(ids.size() == 1);
        auto& cid = *(ids.begin());
        result_pair res = result_pair(DivisionTree::build(ids),
                          this->nodesForSingleCell(cid));
        
        size_t all = res.second.size();
        size_t reduced = this->reduce(res.second);
        res.first->setCost(this->calculateStepCost(all, reduced));
        return res;
    }
    result_pair calculateStrategyForSplitSets(const IdSet& ids,
                                              const IdSet& set1,
                                              const IdSet& set2) {
        assert(set1.size() != 0 && set2.size() != 0);
        auto r1 = this->calculateStrategy(set1);
        auto r2 = this->calculateStrategy(set2);
        
        result_t innerCost = r1.first->getCost() + r2.first->getCost();
        auto res = result_pair(DivisionTree::build(ids, r1.first, r2.first),
                               this->combine(r1.second, r2.second));
        size_t all = res.second.size();
        size_t reduced = this->reduce(res.second);
        res.first->setCost(this->calculateStepCost(all, reduced) + innerCost);
        
        return res;
    }


    virtual result_pair calculateStrategy(const IdSet& ids) {
        return this->calculateStrategyImpl(ids);
    };
    virtual result_pair calculateStrategyImpl(const IdSet& ids) = 0;
public:
    virtual ~AbstractStrategy(){};
    
    virtual result calculateStrategy() {
        auto leaves = cs.getLeavesIds();
        auto res = calculateStrategy(IdSet(leaves.begin(), leaves.end()));
        assert(res.second.empty());
        return res.first;
    }
    
    
    
};

template<int DIMS, class CostFunction = FlopsFunction, class CellType = Cell<DIMS>, class CellSpace = CellNodeSpace<DIMS, CellType> >
class MemoizingStrategy: public virtual AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> {
protected:
    using Strategy = ::AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> ;
//    MemoizingStrategy(const CellSpace& cs, const CostFunction& cf ):Strategy(cs, cf) {}

    
    using result_pair = typename Strategy::result_pair;
    using IdSet = ::CellIdSet<DIMS>;

    std::unordered_map<IdSet, result_pair> cache;

    virtual result_pair calculateStrategy(const IdSet& ids) {
        auto it = cache.find(ids);
        if(it == cache.end()) {
            auto ret = this->calculateStrategyImpl(ids);
            return cache[ids] = ret;
        } else {
            return it->second;
        }

    };
public:
    MemoizingStrategy(){}
    size_t getMemoizedCount() const {
        return cache.size();
    }
};


template<int DIMS,
         class DivisionFunction = NestedBisectionSeparator<DIMS>,
         class CostFunction = FlopsFunction,
         class CellType = Cell<DIMS>,
         class CellSpace = CellNodeSpace<DIMS, CellType> >
class NestedDivisionStrategy : public virtual AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> {
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
        if(ids.size() == 1) {
            return this->calculateStrategyForSingleId(ids);
        }
        result_pair res;


        auto subsets = this->divider(ids);
        return this->calculateStrategyForSplitSets(ids, subsets.first, subsets.second);
    }
    
public:
    NestedDivisionStrategy(const CellSpace& cs,
                           const DivisionFunction& divider = DivisionFunction(),
                           const CostFunction& cf = CostFunction())
    : Strategy(cs, cf), divider(divider) {}

};


template<int DIMS,
class DividersGenerator,
class CostFunction = FlopsFunction,
class CellType = Cell<DIMS>,
class CellSpace = CellNodeSpace<DIMS, CellType> >
class OptimizedDivisionStrategy : public virtual AbstractStrategy<DIMS, CostFunction, CellType, CellSpace> {
protected:
    using Strategy = ::AbstractStrategy<DIMS, CostFunction, CellType, CellSpace>;
    
    using DivisionTree = typename Strategy::DivisionTree;
    using CellId = ::CellId<DIMS>;
    using result = typename Strategy::result;
    using result_pair = typename Strategy::result_pair;
    using IdSet = typename Strategy::IdSet;
    
    virtual result_pair calculateStrategyImpl(const IdSet& ids) {
        assert(ids.size() > 0);
        if(ids.size() == 1) {
            return this->calculateStrategyForSingleId(ids);
        }
        
        DividersGenerator gen(ids);
        result_pair bestResult;
        bool anyResult = false;
        for(auto& divider: gen.getDividers()) {
            std::pair<IdSet, IdSet> subsets = divider(ids);
            
            if(subsets.first.size()==0 || subsets.second.size() == 0) continue;
            
            
            result_pair res = this->calculateStrategyForSplitSets(ids, subsets.first, subsets.second);
            
            if(!anyResult || res.first->getCost() < bestResult.first->getCost()) {
                swap(res, bestResult);
                anyResult = true;
            }
            
            
        }
        assert(anyResult);
        
        return bestResult;
    }
    
public:
    OptimizedDivisionStrategy(const CellSpace& cs,
                           const CostFunction& cf = CostFunction())
    : Strategy(cs, cf) {}
    
};

template<int DIMS,
class DividersGenerator,
class CostFunction = FlopsFunction,
class CellType = Cell<DIMS>,
class CellSpace = CellNodeSpace<DIMS, CellType> >
class MemoizingOptimizedDivisionStrategy
:   public virtual MemoizingStrategy<DIMS, CostFunction, CellType, CellSpace>,
    public virtual OptimizedDivisionStrategy<DIMS, DividersGenerator, CostFunction, CellType, CellSpace>
{
    using MemoizingStrategy = ::MemoizingStrategy<DIMS, CostFunction, CellType, CellSpace>;
    using AbstractStrategy = ::AbstractStrategy<DIMS, CostFunction, CellType, CellSpace>;
    using OptimizedDivisionStrategy = ::OptimizedDivisionStrategy<DIMS, DividersGenerator, CostFunction, CellType, CellSpace>;
public:
        
    MemoizingOptimizedDivisionStrategy(
                                     const CellSpace& cs,
                                     const CostFunction& cf = CostFunction())
    : OptimizedDivisionStrategy(cs, cf), MemoizingStrategy(), AbstractStrategy(cs, cf) {
        
    }
};



#endif /* defined(__HP3d__DivisionStrategy__) */
