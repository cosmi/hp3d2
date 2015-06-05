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
#include "CellDrawer.h"
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    auto p = Point<2>({0,0});
    auto p2 = Point<2>({4,4});

    CellDrawer<2> cd(0, 10, 0, 10, 10);
    auto cid = CellId<2>(p, p2);
//    cd.draw(cid.getChildId(3));
//    cd.draw(cid.getChildId(3).getChildId(3).getAlignedParent());
    cd.draw(cid.getChildId(2).getAlignedParent());
    cd.draw(p2);
//    cd.draw(p2);
//    cd.save("/Users/cosmi/phd/output.bmp");
    cd.open();
    return 0;
}
