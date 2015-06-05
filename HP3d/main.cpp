//
//  main.cpp
//  HP3d
//
//  Created by Marcin Skotniczny on 05.06.2015.
//  Copyright (c) 2015 Marcin Skotniczny. All rights reserved.
//

#include <iostream>
#include "Point.h"
#include "CellDrawer.h"
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    auto p = Point<2>({1,1});
    auto p2 = Point<2>({4,4});

    CellDrawer<2> cd(0, 10, 0, 10, 10);
    cd.draw(Point<2>({0,1}), Colors::BLUE);
//    cd.draw(p2);
//    cd.save("/Users/cosmi/phd/output.bmp");
    cd.open();
    return 0;
}
