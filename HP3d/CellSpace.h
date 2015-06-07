//
//  CellSpace.h
//  HP3d
//
//  Created by Marcin on 06.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellSpace__
#define __HP3d__CellSpace__
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <numeric>
#include <type_traits>
#include "defs.h"
#include "Cell.h"

#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/indirected.hpp>


template <class CellType>
struct LeafFilter {
    bool operator()(const CellType& ct) const {
        return ct.isLeaf();
    }
};

template <class IdType, class CellType>
struct ToIdTransform {
    const IdType& operator()(const CellType& ct) const {
        return ct.getId();
    }
};

template<int DIMS, class Cell = ::Cell<DIMS> >
class CellSpace {
public:
    using Point = ::Point<DIMS>;
    using PointDifference = ::PointDifference<DIMS>;
    using CellType = Cell;
    using CellId = ::CellId<DIMS>;
protected:
    std::vector<Cell*> roots;
    std::unordered_map<CellId, Cell*> dict;
    void makeSplit(const CellId& parent) {
        if(dict.count(parent)==0) {
            makeSplit(parent.getAlignedParent());
        }
        split(parent);
    }
public:
    CellSpace() {}
    
    void initWithOneCell(int lvls) {
        assert(roots.empty());
        auto id = CellId(Point::origin(), PointDifference::cube(dim_t(1)<<dim_t(lvls)));
        auto cell = new Cell(id);
        roots.push_back(cell);
        dict[cell->getId()] = cell;
    }
    
    const auto getCells() const {
        using namespace boost::adaptors;
        return dict | map_values | indirected;
    }
    
    const CellType& getCell(const CellId& cid) const {
        return *dict.find(cid);
    }
    
    std::list<const CellType*> getCoveringCells(const CellId& cid) const {
        std::list<const CellType*> cl;
        for(auto cellptr : roots) {

            cl.splice(cl.end(), cellptr->getCoveringCells(cid));
        }
        return cl;
    }
    
    size_t countCells() const {
        return dict.size();
    }
    
    const auto getLeaves() const {
         using namespace boost::adaptors;
        return getCells() | filtered(LeafFilter<CellType>());
    }
    

    auto getIds() const -> decltype(dict | boost::adaptors::map_keys){
        using namespace boost::adaptors;
        return dict | map_keys;
    }
    
    auto getLeavesIds() const -> decltype(CellSpace::getLeaves() | boost::adaptors::transformed(ToIdTransform<CellId, CellType>())) {
        using namespace boost::adaptors;
        return getLeaves() | transformed(ToIdTransform<CellId, CellType>());
    }
    
    void split(const CellId& id) {

        assert(dict.count(id) > 0);
        Cell* c = dict[id];
        if(c->split()) {
            for(auto& child: c->getChildren()) {
                dict[child.getId()] = &child;
            }
        }
    }
    
    void splitTo(const CellId& id) {
        assert(covers(id));
        if(dict.count(id)>0) {
            return;
        }
        makeSplit(id.getAlignedParent());
    }
    
    CellId getBounds() const {
        CellId cid = roots[0]->getId();
        for(auto& i : roots) {
            cid = CellId::getBounds(cid, i->getId());
        }
        return cid;
    }
    
    bool covers(const CellId & cid) const {
        for(auto& i : roots) {
            if(i->getId().covers(cid)) return true;
        }
        return false;
    }
};


#endif /* defined(__HP3d__CellSpace__) */
