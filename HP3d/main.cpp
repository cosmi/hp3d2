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
#include "CellDrawer.h"
#include "GridBuilder.h"
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...


    auto cs = CellSpace<2>();
    cs.initWithOneCell(4);
//    cs.splitTo(CellId<2>::unit().move(0,7).move(1,7));
    buildSingularity(cs,0);
    cs.enforceTauRule();
    CellDrawer<2> cd(cs.getBounds(),50);
//    cd.draw(cid.getChildId(3));
//    cd.draw(cid.getChildId(3).getChildId(3).getAlignedParent());
//    cd.draw(cid.getChild(2));
//    cd.draw(cid.getChild(2).move(0, 2).move(1,1).getChild(0));
    cd.draw(cs);
//    cd.draw(p2);
//    cd.save("/Users/cosmi/phd/output.bmp");
    cd.open();
    
    return 0;
}
