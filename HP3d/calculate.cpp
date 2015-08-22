//
//  calculate.cpp
//  HP3d
//
//  Created by Marcin on 05.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#include "calculate.h"


#include <cassert>
#include <iostream>
#include "Point.h"
#include "CellId.h"
#include "CellNodeSpace.h"
#include "CellDrawer.h"
#include "CellIdSet.h"
#include "GridBuilder.h"
#include "CellSeparators.h"
#include "DivisionStrategy.h"
#include "CellDividerGenerators.h"
#include "TestUtils.h"

#include <cmath>

struct result_record {
    int dims, sing, lvl;
    size_t nodes, elements;
    result_t cost;
};

std::ostream& operator<<(std::ostream& os, const result_record& res) {
    os << res.dims << '\t' << res.sing << '\t' << res.lvl << '\t' << res.elements << '\t' << res.nodes <<'\t' << res.cost;
    return os;
}



template<int DIMS>
result_record calculateResult(int sing, int lvl) {
    assert(sing <= DIMS);
    auto grid = initSingularity<DIMS, CellNodeSpace<DIMS> >(sing, lvl);
    
    MemoizingOptimizedDivisionStrategy<DIMS, HalfDividersGenerator<DIMS> > strat(grid);
    auto result = strat.AbstractStrategy<DIMS>::calculateStrategy();
    
    return result_record({DIMS, sing, lvl, grid.getFreeNodes().size(), grid.getLeavesIdSet().size(), result->getCost()});
}

result_record calculateResult(int dims, int sing, int lvl) {
    switch(dims) {
        case 1: return calculateResult<1>(sing, lvl);
        case 2: return calculateResult<2>(sing, lvl);
        case 3: return calculateResult<3>(sing, lvl);
        case 4: return calculateResult<4>(sing, lvl);
        case 5: return calculateResult<5>(sing, lvl);
        case 6: return calculateResult<6>(sing, lvl);
        case 7: return calculateResult<7>(sing, lvl);
        case 8: return calculateResult<8>(sing, lvl);
    }
    return result_record();
}



int main(int argc, char** argv) {
    for(int dims = 1; dims<=5; dims++) {
        FOR(sing, dims) {
            for(int lvl = 0; lvl < 20; lvl++) {
                double limit = lvl * dims * pow(pow(2, sing), lvl);;
                if (limit > 50000) continue;
                std::cout << limit << "\t";
                std::cout << calculateResult(dims, sing, lvl) << std::endl;
            }
        }
    }
    
}


