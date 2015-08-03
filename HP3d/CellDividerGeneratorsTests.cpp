//
//  CellDividerGeneratorsTests.cpp
//  HP3d
//
//  Created by Marcin on 03.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#include "CellDividerGeneratorsTests.h"
#include "TestUtils.h"



template<int DIMS>
size_t countDivisionsForSingularity(int dims, int lvl) {
    auto ids = initSingularity<DIMS, CellSpace<DIMS> >(dims, lvl).getLeavesIdSet();
    
    
    auto generator = PlaneDividersGenerator<DIMS>(ids);
    return generator.getDividers().size();
}

void testPlaneDividersGenerator() {
    CHECK_EQ("Has correct no of divisions 2/0/2", 2, countDivisionsForSingularity<2>(0, 2));
    CHECK_EQ("Has correct no of divisions 2/0/3", 2, countDivisionsForSingularity<2>(0, 3));
    CHECK_EQ("Has correct no of divisions 2/1/2", 3, countDivisionsForSingularity<2>(1, 3));
}

void runCellDividerGeneratorsTests() {
    
    TEST("Test PlaneDividersGenerator", testPlaneDividersGenerator());
    
    
}