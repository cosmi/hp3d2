//
//  main.cpp
//  HP3d
//
//  Created by Marcin Skotniczny on 05.06.2015.
//  Copyright (c) 2015 Marcin Skotniczny. All rights reserved.
//

#include <iostream>
#include "Point.h"
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    auto p = Point<3>({1,1,1});
    auto p2 = Point<3>({4,4,4});
    Point<3> p3 (p2 - p);
    std::cout << p << p2 << (p2 - p)<< p3<< endl;
    return 0;
}
