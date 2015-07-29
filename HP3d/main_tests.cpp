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
#define CHECK(msg, code) if(!(code)) { std::cerr << "CHECK failed: " << msg << endl; err_count++;}
#define ASSERT(msg, code) if(!(code)) { std::cerr << "ASSERT failed: " << msg << endl; err_count++; return; }


template<int DIMS>
void initializeCellSpace(int size) {
    auto cs = CellNodeSpace<DIMS>();
    cs.initWithOneCell(size);
    CHECK("Has only one cell", cs.countCells() == 1);
    auto bounds = cs.getBounds();
    CHECK("Is cube", bounds.isCube());
    CHECK("Has correct size", bounds.getSide() == 1<<size);
}


void runAllTests() {
    TEST("Cell space initializes correctly 2,4", initializeCellSpace<2>(4));
    TEST("Cell space initializes correctly 4,7", initializeCellSpace<4>(7));
    TEST("Cell space initializes correctly 2,0", initializeCellSpace<2>(0));
}



int main(int argc, const char * argv[]) {
    runAllTests();
}