//
//  TestCommons.cpp
//  HP3d
//
//  Created by Marcin on 04.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//
#include <ctime>
#include "TestCommons.h"


size_t getMillis() {
    return clock()*1000/CLOCKS_PER_SEC;
}
