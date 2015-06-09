//
//  CostFunction.h
//  HP3d
//
//  Created by Marcin on 09.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CostFunction__
#define __HP3d__CostFunction__

#include <cmath>
#include "defs.h"

struct FlopsFunction {
    result_t operator()(result_t eliminated, result_t total) const {
        result_t a = eliminated;
        result_t b = total;
        result_t res = (a * (6*b*b - 6*a*b + 6*b + 2*a*a - 3*a + 1))/6;
        return res;
    }
};

#endif /* defined(__HP3d__CostFunction__) */
