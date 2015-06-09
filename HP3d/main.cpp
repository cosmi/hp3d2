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
    using namespace std;
    auto cs = CellNodeSpace<2>();
    cs.initWithOneCell(5);
//    std::cout << cs.getBounds() << " " << cs.getBounds().getHalf() << std::endl;
    cs.splitTo(CellId<2>::unit());
//    cs.splitTo(CellId<2>::unit().move(0,7).move(1,7));
//    cs.splitTo(CellId<2>::unit().move(0,23).move(1,23));
//    buildSingularity(cs,0);
    enforceExtendedTauRule(cs);
    CellDrawer<2> cd(cs.getBounds(),50);
//    CellIdSet<2> cis(cs);
//    cd.draw(splitSet(cis, cs.getBounds().getHalf()).first);
    cs.initNodes();
    cd.draw(cs);
//    cd.draw(cid.getChildId(3));
//    cd.draw(cid.getChildId(3).getChildId(3).getAlignedParent());
//    cd.draw(cid.getChild(2));
//    cd.draw(cid.getChild(2).move(0, 2).move(1,1).getChild(0));
//    cd.draw(cs);
//    cd.draw(p2);
//    cd.save("/Users/cosmi/phd/output.bmp");
    cd.open(false);
    
    
    NestedDissectionStrategy<2, FlopsFunction> strat(cs);
    auto ret = strat.AbstractStrategy<2, FlopsFunction>::calculateStrategy();
    cout << "RES " << ret.second << endl;
    
    return 0;
}
