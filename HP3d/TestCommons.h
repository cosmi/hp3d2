//
//  TestCommons.h
//  HP3d
//
//  Created by Marcin on 03.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef HP3d_TestCommons_h
#define HP3d_TestCommons_h

#include <cassert>
#include <iostream>


size_t getMillis();

static int err_count = 0;
#define TEST(msg, code) {::err_count = 0; std::cerr << "Testing: " << msg << "... " << std::endl; {code;} if(::err_count == 0) {std::cerr << "SUCCESS" << std::endl;} else {std::cerr << "ERROR" << std::endl;} };
#define CHECK(msg, code) if(!(code)) { std::cerr << "CHECK failed: " << msg << std::endl; ::err_count++;}
#define ASSERT(msg, code) if(!(code)) { std::cerr << "ASSERT failed: " << msg << std::endl; ::err_count++; return; }
#define CHECK_EQ(msg, value, code) {auto __result = code; if(__result != value) { std::cerr << "CHECK failed: " << msg << " (should be: " << value << ", was: " << __result << std::endl; ::err_count++;}}


#define TIME(msg, code) {size_t __start = getMillis(); {code;} std::cout << msg << " took " << getMillis() - __start << " ms" << endl;}

#endif
