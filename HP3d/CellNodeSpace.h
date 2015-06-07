//
//  CellNode.h
//  HP3d
//
//  Created by Marcin on 07.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellNodeSpace__
#define __HP3d__CellNodeSpace__
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <numeric>
#include <set>

#include "defs.h"
#include "Cell.h"
#include "CellSpace.h"


#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/indirected.hpp>


template<int DIMS>
void getInducedNodesOnCellIdHelper(const CellId<DIMS>& cid, int dim, std::list<CellId<DIMS> >& ret) {
    if(dim == DIMS) {
        ret.push_back(cid);
    } else {
        getInducedNodesOnCellIdHelper(cid, dim+1, ret);
        getInducedNodesOnCellIdHelper(cid.getReducedDimId(dim, false), dim+1, ret);
        getInducedNodesOnCellIdHelper(cid.getReducedDimId(dim, true), dim+1, ret);
    }
}

template<int DIMS>
std::list<CellId<DIMS> > getInducedNodesOnCellId(const CellId<DIMS>& cid) {
    std::list<CellId<DIMS> > ret;
    getInducedNodesOnCellIdHelper(cid, 0, ret);
    size_t exp_count = 1;
    FOR(i, DIMS) exp_count *= 3;
    assert(exp_count == ret.size());
    return ret;
}


template<int DIMS, class CellType = Cell<DIMS> >
class CellNodeSpace : public CellSpace<DIMS, CellType> {
public:
    using CellSpace = ::CellSpace<DIMS, CellType>;
    using CellId = ::CellId<DIMS>;

    using IdSet = std::unordered_set<CellId>;
    
    IdSet constrainedNodes;
    IdSet freeNodes;
public:
    bool isConstrainedNode(const CellId& node) {
        auto bounds = CellSpace::getBounds();
        auto boundDimsCount = node.countDimsOnBounds(bounds);
        
        auto cells = CellSpace::getCoveringCells(node);
        
        size_t dimensionality = node.countNonZeroDims();
        
        using namespace std;
        
        cout << " NODE: " << node << endl;
        for(auto& cellptr : cells) {
            cout << " -> " << cellptr->getId() << endl;
        }
        
        assert(cells.size() <= (1<<(DIMS - dimensionality - boundDimsCount)));
        
        size_t leavesCount = 0;
        
        size_t matchingNodeCount = 0;
        for(auto& cellptr : cells) {
            if(cellptr->isLeaf()) leavesCount++;
            auto& id = cellptr->getId();
            if(node.isNodeOnCell(id)) {
                matchingNodeCount++;
            }
        }
        assert(leavesCount > 0);
        return matchingNodeCount != cells.size();
    }
    
    const IdSet& getConstrainedNodes() const {
        return constrainedNodes;
    }
    const IdSet& getFreeNodes() const {
        return freeNodes;
    }
    void initNodes() {
        IdSet s;
        for(auto& cid : CellSpace::getLeavesIds()) {
            auto list = getInducedNodesOnCellId(cid);
            s.insert(list.begin(), list.end());
        }
        for(auto& node: s) {
            if(isConstrainedNode(node)) {
                constrainedNodes.insert(node);
            } else {
                freeNodes.insert(node);
            }
        }
        
    }
};



#endif /* defined(__HP3d__CellNodeSpace__) */
