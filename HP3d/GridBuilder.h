//
//  GridBuilder.h
//  HP3d
//
//  Created by Marcin on 06.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__GridBuilder__
#define __HP3d__GridBuilder__

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <numeric>

#include "defs.h"
#include "Cell.h"
#include "CellSpace.h"

template<int DIMS, class Cell = ::Cell<DIMS> >
void buildSingularityHelper(CellSpace<DIMS, Cell>& cs, int dims, int dim, CellId<DIMS> id) {
    if(dim == dims) {
        cs.splitTo(id);
    } else {
        auto bounds = cs.getBounds();
        while(bounds.covers(id)) {
            buildSingularityHelper(cs, dims, dim+1, id);
            id = id.move(dim, 1);
        }
    }
}

template<int DIMS, class Cell = ::Cell<DIMS> >
void buildSingularity(CellSpace<DIMS, Cell>& cs, int dims) {
    buildSingularityHelper(cs, dims, 0, CellId<DIMS>::unit());
}


#endif /* defined(__HP3d__GridBuilder__) */
