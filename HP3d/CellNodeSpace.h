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
    using IdList = std::list<CellId>;
    using CellList = std::list<CellType*>;
    
    IdSet constrainedNodes;
    IdSet freeNodes;
    
    using NodeToCellsMap = std::unordered_map<CellId, IdList>;
    
    NodeToCellsMap nodeCells;
    
public:
    bool isConstrainedNode(const CellId& node) {
        auto bounds = CellSpace::getBounds();
        auto boundDimsCount = node.countDimsOnBounds(bounds);
        
        auto cells = CellSpace::getCoveringCells(node);
        
        size_t dimensionality = node.countNonZeroDims();
        
//        using namespace std;
//        
//        cout << " NODE: " << node << endl;
//        for(auto& cellptr : cells) {
//            cout << " -> " << cellptr->getId() << endl;
//        }
        
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
    
    IdList getCoveredCells(const CellId& node) const {
        
        using namespace std;
        // this is the count of non-zero dimensions of our node
        size_t nodeDims = node.getDimensionality();
        
        if(nodeDims == DIMS) {
            // THIS IS A SPECIAL CASE! Just return matching node
            assert(this->hasCell(node));
            return IdList(1, node);
        }
//        cout << "=== NODE: " << node << endl;
        
        // find all candidate cells that touch our node
        // there will be some overhead, esp. for high-dimensionality nodes
        // we could improve that, but it doesn't really matter
        auto touchingCells = this->getTouchingLeaves(node);
        IdSet touchingSides;
        
        for(auto& cellptr : touchingCells) {
            auto& cell = *cellptr;
            auto& id = cell.getId();
            // this will get all DIMS-1 sides of our cell, ie. for 3D - each face of the cube
            auto sides = id.getLowerDimensionalityIds();
            for(auto& side: sides) {
                auto isct = CellId::intersection(node, side);
                if(!isct.isValid()) {
                    // this side is not even touching the node
                    continue;
                }
                
                size_t dims = isct.getDimensionality();
                assert(dims <= nodeDims); //sanity check, intersection cannot have more dims than original node
                
                // if intersection has less dimensions than the node, ignore that side
                if(dims == nodeDims) {
                    
                    // some of the cells found can be 1 step smaller
                    // thus, the intersection should be exactly the original node or half of it
//                    assert(isct == node || isct.getSize()*2 == node.getSize());
       
                    
                    // all cells containing this side (or part of it) will be covered by the node
                    touchingSides.insert(side);
                }
            }
        }
        
//        auto candidateBounds = CellId::getBounds(touchingSides.begin(), touchingSides.end());
//        cout << "BOUNDS: " << candidateBounds << endl;
        IdSet candidateCells;
        for(auto& side: touchingSides) {
//            cout << "S: " << side << endl;
            auto cells = this->getTouchingLeaves(side);
            for(auto& cellptr: cells) {
                candidateCells.insert(cellptr->getId());
            }
        }
        
        
        IdList ret;
        // select all cells that have a side covered by one of the selected sides
        for(const CellId& cid : candidateCells) {
//            cout << "CAND: " << cid << endl;
            for(auto& side: touchingSides) {
                auto isct = CellId::intersection(side, cid);
                if(isct.isValid() && isct.getDimensionality() == DIMS-1) {
//                    cout << "I " << isct << " " << side << " " << cid << endl;

//                    assert(isct == side || isct.getSize()*2 == side.getSize());
                    ret.push_back(cid);
                    break;
                }
            }
        }
        
//        for(auto& cid: ret) {
//            cout << "C " << cid << endl;
//        }
        return ret;
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
        
        for(auto& node: freeNodes) {
            nodeCells[node] = getCoveredCells(node);
        }
        
    }
    
    const NodeToCellsMap& getNodeToCellsMapping() const {
        return nodeCells;
    }
};



#endif /* defined(__HP3d__CellNodeSpace__) */
