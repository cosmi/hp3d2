//
//  CellIdTests.c
//  HP3d
//
//  Created by Marcin on 03.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#include "CellIdTests.h"
#include "TestCommons.h"

void testCellIdOperations() {
    CHECK_EQ("Valid half", CellId<2>::getForSize({4, 4}), CellId<2>::getForSize({4, 8}).getHalf());
    CHECK_EQ("Valid half", CellId<2>::getForSize({4, 4}), CellId<2>::getForSize({8, 4}).getHalf());
}

void testCellIdRelations() {
    CHECK("Is boundary 1", !CellId<2>({0,0}, {2,2}).isSideOf(CellId<2>({0,0}, {2,2})));
    CHECK("Is boundary 2", CellId<2>({0,2}, {2,2}).isSideOf(CellId<2>({0,0}, {2,2})));
    CHECK("Is boundary 3", !CellId<2>({0,0}, {3,2}).isSideOf(CellId<2>({0,0}, {2,2})));
}


void runCellIdTests() {
    TEST("Test CellId Operations", testCellIdOperations());
    TEST("Test CellId Relations", testCellIdRelations());
    
    
}