//
//  main_tests.cpp
//  HP3d
//
//  Created by Marcin on 29.07.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#include "main_tests.h"




template<int DIMS>
void initializeCellSpace(int size) {
    auto cs = CellNodeSpace<DIMS>();
    cs.initWithOneCell(size);
    CHECK("Has only one cell", cs.countCells() == 1);
    auto bounds = cs.getBounds();
    CHECK("Is cube", bounds.isCube());
    CHECK("Has correct size", bounds.getSide() == 1<<size);
}


template<int DIMS>
result_t calculateByBisectionStrategy(int sing, int lvl) {
    
    auto cs = CellNodeSpace<DIMS>();
    cs.initWithOneCell(lvl);
    buildSingularity(cs,sing);
    enforceExtendedTauRule(cs,1);
    cs.initNodes();
    
    NestedDivisionStrategy<DIMS> strat(cs);
    auto ret = strat.AbstractStrategy<DIMS>::calculateStrategy();
    
    return ret->getCost();
}

void testBisectionStrategy() {
    CHECK_EQ("Valid result", 118, calculateByBisectionStrategy<1>(0, 8));
    CHECK_EQ("Valid result", 4529, calculateByBisectionStrategy<2>(0, 5))
    CHECK_EQ("Valid result", 9963, calculateByBisectionStrategy<2>(1, 3))
    CHECK_EQ("Valid result", 7590013, calculateByBisectionStrategy<3>(2, 3))
    CHECK_EQ("Valid result", 222635, calculateByBisectionStrategy<3>(0, 4))
    
    CHECK_EQ("Valid result", 60114821, calculateByBisectionStrategy<4>(1, 3));
    CHECK_EQ("Valid result", 1015924531, calculateByBisectionStrategy<5>(0, 3));
}


template<int DIMS>
result_t calculateByOptimizedPlanesStrategy(int sing, int lvl) {
    
    auto cs = CellNodeSpace<DIMS>();
    cs.initWithOneCell(lvl);
    buildSingularity(cs,sing);
    enforceExtendedTauRule(cs,1);
    cs.initNodes();
    
    OptimizedDivisionStrategy<DIMS, PlaneDividersGenerator<DIMS> > strat(cs);
    auto ret = strat.calculateStrategy();
    
    return ret->getCost();
}


void testOptimizedPlanesStrategy() {
    CHECK_EQ("Valid result", 118, calculateByOptimizedPlanesStrategy<1>(0, 3));
    CHECK_EQ("Valid result", 4529, calculateByOptimizedPlanesStrategy<2>(0, 3))
    CHECK_EQ("Valid result", 9963, calculateByOptimizedPlanesStrategy<2>(1, 3))
}



void runAllTests() {
    runCellIdTests();
    runCellDividerGeneratorsTests();
    TEST("Cell space initializes correctly 2,4", initializeCellSpace<2>(4));
    TEST("Cell space initializes correctly 4,7", initializeCellSpace<4>(7));
    TEST("Cell space initializes correctly 2,0", initializeCellSpace<2>(0));
    TEST("Test Bisection Strategy", testBisectionStrategy());
//    TEST("Test OptimizedPlanes Strategy", testOptimizedPlanesStrategy());

}



int main(int argc, const char * argv[]) {
    runAllTests();
}