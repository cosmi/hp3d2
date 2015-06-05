//
//  CellId.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellId__
#define __HP3d__CellId__
#include <algorithm>
#include "defs.h"
#include "Point.h"

template <int DIMS>
class CellId {
    using Point = Point<DIMS>;
    using PointDifference = PointDifference<DIMS>;
    Point from, to;
    
public:
    CellId(const Point& from, const Point& to): from(from), to(to){}
    const Point& getFrom() {
        return from;
    }
    const Point& getTo() {
        return to;
    }
    PointDifference getSize() const {
        return to - from;
    }
};


template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const CellId<DIMS>& id) {
    return os << "C[" << id.getFrom() << ';'<< id.getTo() << ']';
}



#endif /* defined(__HP3d__CellId__) */
