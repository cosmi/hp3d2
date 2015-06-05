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
    CellId(const Point& from, const PointDifference& size): from(from), to(from+size){}
    const Point& getFrom() const {
        return from;
    }
    const Point& getTo() const {
        return to;
    }
    PointDifference getSize() const {
        return to - from;
    }
    
    bool isCube() const {
        auto size = getSize();
        return size.isCube();
    }
    bool isAligned() const {
        auto size = getSize();
        auto & from = getFrom();
        FOR(i, DIMS) {
            int p = size[i]-1;
            if(from[i] & p) return false;
        }
        return true;
    }
    
    CellId getChildId(mask_t id) const {
        auto pd = getSize()>>1;
        auto bmpd = pd.selectDimsByBitMask(id);
        return CellId(getFrom() + bmpd, pd);
    }
    
    CellId getAlignedParent() const {
        auto nsize = getSize()<<1;
        int bits = nsize.countZeroBits();
        auto pt = getFrom().resetBits(bits);
        return CellId(pt, nsize);
    }
};


template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const CellId<DIMS>& id) {
    return os << "C[" << id.getFrom() << ';'<< id.getTo() << ']';
}



#endif /* defined(__HP3d__CellId__) */
