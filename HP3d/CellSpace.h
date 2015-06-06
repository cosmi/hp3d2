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

#include "defs.h"
#include "Cell.h"

#include <boost/range/adaptor/map.hpp>

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
        std::cout << "SPL " << parent <<std::endl;
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
    
    auto getCells() const {
        return dict | boost::adaptors::map_values;
    }
    
    void split(const CellId& id) {
        std::cout << dict.size() << std::endl;
        for(auto it : dict) {
            std::cout << it.first << std::endl;
        }

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
    
    void enforceTauRule(const CellId & id) {
        splitTo(id);
        FOR(dim, DIMS) {
            auto p1 = id.move(dim, -1);
            if(covers(p1)) enforceTauRule(p1.getAlignedParent());
            auto p2 = id.move(dim, +1);
            if(covers(p2)) enforceTauRule(p2.getAlignedParent());
        }
    }
    
    void enforceTauRule() {
        std::vector<CellId> v;
        v.reserve(dict.size());
        for(auto & i : dict) {
            v.push_back(i.first);
        }
        for(auto & i : v) {
            enforceTauRule(i);
        }
    }
};


#endif /* defined(__HP3d__CellSpace__) */
