//
//  types.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef HP3d_types_h
#define HP3d_types_h

#include <sstream>

using dim_t = int;
using mask_t = unsigned;
#define FOR(x, n) for(int x = 0, __n = (n); x < __n; x++)
#define FORI(x, a, n) for(int x = (a), __n = (n); x < __n; x++)
#define FORR(x, n) for(int x = (n)-1; x >= 0; x--)


template<class T>
std::string toString(const T& t, bool *ok = NULL)
{
    std::ostringstream stream;     // line A
    stream << t;              // line B
    if(ok != NULL)
        *ok = (stream.fail() == false);  // line C
    return stream.str();      // Line D
}


#endif
