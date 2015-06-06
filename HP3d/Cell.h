//
//  Cell.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__Cell__
#define __HP3d__Cell__
#include <algorithm>
#include <iostream>
#include <array>

#include "defs.h"
#include "CellId.h"

template<int DIMS>
class Cell {
public:
    using CellId = ::CellId<DIMS>;
    using Point = ::Point<DIMS>;
    using ChildrenArray = std::array<Cell, 1<<DIMS>;
protected:
    CellId id;
    ChildrenArray* children;
    
    Cell():id(CellId::null()) {}
    friend class std::array<Cell, 1<<DIMS>;
public:
    Cell(const CellId& id):id(id), children(nullptr) {
        assert(id.isCube());
        assert(id.isAligned());
        std::cout << ">>" << this->getId() << std::endl;
    }
    ~Cell() {
        delete [] children;
    }

    const CellId& getId() const {
        return id;
    }
    
    bool isLeaf() const {
        return children == nullptr;
    }
    
    bool split() {
        if(children != nullptr) return false;
        children = new ChildrenArray;
        FOR(i, 1<<DIMS) {
            (*children)[i] = Cell(id.getChild(i));
        }
        return true;
    }
    
    ChildrenArray& getChildren() {
        return *children;
    }
    const ChildrenArray& getChildren() const {
        return *children;
    }
};

template<int DIMS>
using DefaultCell = Cell<DIMS>;

#endif /* defined(__HP3d__Cell__) */
