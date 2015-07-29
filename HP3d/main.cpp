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
#include "CellNodeSpace.h"
#include "CellDrawer.h"
#include "CellIdSet.h"
#include "GridBuilder.h"
#include "CellSeparators.h"
#include "DivisionStrategy.h"
#include "CostFunction.h"
using namespace std;

int main(int argc, const char * argv[]) {
    const int DIMS = 2;
    
    using namespace std;
    auto cs = CellNodeSpace<DIMS>();
    cs.initWithOneCell(4);
//    std::cout << cs.getBounds() << " " << cs.getBounds().getHalf() << std::endl;
//    cs.splitTo(CellId<DIMS>::unit());
//    cs.splitTo(CellId<2>::unit().move(0,7).move(1,7));
//    cs.splitTo(CellId<2>::unit().move(0,23).move(1,23));
    buildSingularity(cs,0);
    enforceExtendedTauRule(cs);

//    CellIdSet<2> cis(cs);
//    cd.draw(splitSet(cis, cs.getBounds().getHalf()).first);
    cs.initNodes();

    CellDrawer<DIMS> cd(cs.getBounds(),50);
    cd.draw(cs);
    cd.open(false);
    
//        NestedDivisionStrategy<DIMS, ArbitrarySeparator<DIMS> > strat(cs);
    NestedDivisionStrategy<DIMS> strat(cs);
    auto ret = strat.AbstractStrategy<DIMS>::calculateStrategy();
    
//    ret->printToStream(cout);
    cout << "RES " << ret->getCost() << endl;
    return 0;
}
