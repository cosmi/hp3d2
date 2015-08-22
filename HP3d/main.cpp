//
//  main.cpp
//  HP3d
//
//  Created by Marcin Skotniczny on 05.06.2015.
//  Copyright (c) 2015 Marcin Skotniczny. All rights reserved.
//

#include <cassert>
#include <iostream>
#include "Point.h"
#include "CellId.h"

#include "CellSpace.h"
#include "CellNodeSpace.h"
#include "CellDrawer.h"
#include "CellIdSet.h"
#include "GridBuilder.h"
#include "CellSeparators.h"
#include "CellDividerGenerators.h"
#include "DivisionStrategy.h"
#include "CostFunction.h"
using namespace std;





int main(int argc, const char * argv[]) {
    const int DIMS = 2;
    
    using namespace std;
    auto cs = CellNodeSpace<DIMS>();
    cs.initWithOneCell(3);
//    std::cout << cs.getBounds() << " " << cs.getBounds().getHalf() << std::endl;
//    cs.splitTo(CellId<DIMS>::unit());
///    cs.splitTo(CellId<DIMS>::unit().move(0,7).move(1,7));
//    cs.splitTo(CellId<2>::unit().move(0,23).move(1,23));
    buildSingularity(cs,1);
    enforceExtendedTauRule(cs,1);

//    CellIdSet<2> cis(cs);
//    cd.draw(splitSet(cis, cs.getBounds().getHalf()).first);
    cs.initNodes();

    CellDrawer<DIMS> cd0(cs.getBounds(),50);
    CellDrawer<DIMS> cd1(cs.getBounds(),50);
    CellSpace<DIMS>& cs1 = cs;
    cd0.draw(cs1);
    cd1.draw(cs1);
    
//        NestedDivisionStrategy<DIMS, ArbitrarySeparator<DIMS> > strat(cs);
//    NestedDivisionStrategy<DIMS> strat(cs);
    MemoizingOptimizedDivisionStrategy<DIMS, PlaneDividersGenerator<DIMS> > strat1(cs);
    AbstractStrategy<2>& strat = strat1;
    auto ret = strat.calculateStrategy();
//auto ret = strat.AbstractStrategy<2, FlopsFunction, Cell<2>, CellNodeSpace<2, Cell<2> > >::calculateStrategy();
//    cd0.draw(*ret, false);
    cd1.draw(*ret, true);
    cd0.open(false);
    cd1.open(false);
//    ret->printToStream(cout);
    cout << "RES " << ret->getCost() << endl;
//    std::system("sleep ");
    return 0;
}
