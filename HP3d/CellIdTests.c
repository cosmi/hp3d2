//
//  CellIdTests.c
//  HP3d
//
//  Created by Marcin on 03.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#include "CellIdTests.h"

void testCellIdOperations() {
    CHECK_EQ("Valid half", CellId<2>::getForSize({4, 4}), CellId<2>::getForSize({4, 8}).getHalf());
    CHECK_EQ("Valid half", CellId<2>::getForSize({4, 4}), CellId<2>::getForSize({8, 4}).getHalf());
}

void testCellIdRelations() {
    
}


void runCellIdTests() {
    TEST("Test CellId Operations", testCellIdOperations());
    TEST("Test CellId Relations", testCellIdRelations());
    
    
}