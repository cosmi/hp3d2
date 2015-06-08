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
using namespace std;

int main(int argc, const char * argv[]) {

    auto cs = CellNodeSpace<2>();
    cs.initWithOneCell(4);
//    std::cout << cs.getBounds() << " " << cs.getBounds().getHalf() << std::endl;
//    cs.splitTo(CellId<2>::unit());
    cs.splitTo(CellId<2>::unit().move(0,7).move(1,7));
//    buildSingularity(cs,0);
    enforceTauRule(cs);
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
    
    return 0;
}
