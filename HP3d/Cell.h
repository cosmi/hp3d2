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

#include "defs.h"
#include "CellId.h"

template<int DIMS>
class Cell {
public:
    using CellId = CellId<DIMS>;
    using Point = Point<DIMS>;
protected:
    CellId id;
    Cell * children;
    Cell() {}
public:
    Cell(const CellId& id):id(id), children(nullptr) {
        assert(id.isCube());
        assert(id.isAligned());
    }

    const CellId& getId() const {
        return id;
    }
    
    
    void split() const {
        children = new Cell[1<<DIMS];
        FOR(i, 1<<DIMS) {
            
        }
    }
    
};


#endif /* defined(__HP3d__Cell__) */
