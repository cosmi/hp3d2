//
//  main_tests.cpp
//  HP3d
//
//  Created by Marcin on 29.07.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#include "main_tests.h"


int err_count = 0;
#define TEST(msg, code) {err_count = 0; std::cerr << "Testing: " << msg << "... " << std::endl; {code;} if(err_count == 0) {std::cerr << "SUCCESS" << std::endl;} else {std::cerr << "ERROR" << std::endl;} };
#define CHECK(msg, code) if(!(code)) { std::cerr << "CHECK failed: " << msg << std::endl; err_count++;}
#define ASSERT(msg, code) if(!(code)) { std::cerr << "ASSERT failed: " << msg << std::endl; err_count++; return; }
#define CHECK_EQ(msg, value, code) {auto __result = code; if(__result != value) { std::cerr << "CHECK failed: " << msg << " (should be: " << value << ", was: " << __result << std::endl; err_count++;}}

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


void testCellId() {
    
    CHECK_EQ("Valid half", CellId<2>::getForSize({4, 4}), CellId<2>::getForSize({4, 8}).getHalf());
    CHECK_EQ("Valid half", CellId<2>::getForSize({4, 4}), CellId<2>::getForSize({8, 4}).getHalf());
}



void runAllTests() {
    TEST("Test CellId", testCellId());
    TEST("Cell space initializes correctly 2,4", initializeCellSpace<2>(4));
    TEST("Cell space initializes correctly 4,7", initializeCellSpace<4>(7));
    TEST("Cell space initializes correctly 2,0", initializeCellSpace<2>(0));
    TEST("Test Bisection Strategy", testBisectionStrategy());
}



int main(int argc, const char * argv[]) {
    runAllTests();
}