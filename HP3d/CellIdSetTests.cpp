//
//  CellIdSetTests.cpp
//  HP3d
//
//  Created by Marcin on 21.11.2015.
//  Copyright © 2015 Marcin. All rights reserved.
//

#include "CellIdSetTests.h"
#include "TestCommons.h"

void testCellIdOperations() {
}

void runCellIdSetTests() {
    TEST("Test CellIdSet Division", testCellIdOperations());
    TEST("Test CellId Relations", testCellIdRelations());
    
    
}