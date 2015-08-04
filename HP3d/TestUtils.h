//
//  TestUtils.h
//  HP3d
//
//  Created by Marcin on 03.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef HP3d_TestUtils_h
#define HP3d_TestUtils_h

#include "CellNodeSpace.h"
#include "GridBuilder.h"

template<int DIMS, class CellSpace = CellNodeSpace<DIMS> >
CellSpace initSingularity(int dims, int lvl) {
    auto cs = CellSpace();
    cs.initWithOneCell(lvl);
    buildSingularity(cs, dims);
    enforceExtendedTauRule(cs,1);
    return cs;
}




#endif
