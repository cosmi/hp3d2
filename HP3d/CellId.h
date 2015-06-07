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
    using PointBase = ::PointBase<DIMS>;
    using Point = ::Point<DIMS>;
    using PointDifference = ::PointDifference<DIMS>;
    Point from, to;
    
public:
    CellId(const Point& from, const Point& to): from(from), to(to){}
    CellId(const Point& from, const PointDifference& size): from(from), to(from+size){}
    static CellId unit() {
        return CellId(Point::origin(), PointDifference::cube(1));
    }
    static CellId null() {
        return CellId(Point::origin(), Point::origin());
    }
    
    
    const Point& getFrom() const {
        return from;
    }
    const Point& getTo() const {
        return to;
    }
    PointDifference getSize() const {
        return to - from;
    }
    dim_t getSide() const {
        return getSize().getSide();
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
    bool operator==(const CellId& cid) const {
        return getFrom() == cid.getFrom() && getTo() == cid.getTo();
    }
    
    CellId getChild(mask_t id) const {
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
    
    CellId move(int direction, int distance) const {
        auto size = getSize();
        auto & from = getFrom();
        auto diff = size.selectDim(direction) * distance;
        return CellId(from+diff, size);
    }
    
    bool covers(const CellId& cid) const {
        return getFrom().hasRelation(cid.getFrom(), PointRelation::LESS_OR_EQ)
            && getTo().hasRelation(cid.getTo(), PointRelation::MORE_OR_EQ);
    }
    
    
    /* Bounds methods */
 
    static CellId getBounds(const CellId& a, const CellId& b) {
        return CellId(Point(PointBase::selectMinDims(a.getFrom(),b.getFrom())), Point(PointBase::selectMaxDims(a.getTo(),b.getTo())));
    }
    
    template <typename Iterator>
    static CellId getBounds(Iterator begin, const Iterator& end) {
        assert(begin != end);
        CellId res = *begin;
        while(++begin != end) {
            res = CellId::getBounds(res, *begin);
        }
        return res;
    }
 
    
    
    CellId getHalf() const {
        auto size = getSize();
        size_t splittingDim = size.getLongestDim();
        assert(size[splittingDim]%2 == 0);
        auto nsize = PointDifference(size.replaceDim(splittingDim, size[splittingDim]/2));
        return CellId(getFrom(), nsize);
    }
    
    
    /* Node methods */
    
    size_t countNonZeroDims() const {
        size_t ret = 0;
        FOR(i, DIMS) {
            if(from[i] != to[i]) ret++;
        }
        return ret;
    }
    
    size_t countDimsOnBounds(const CellId& bounds) const {
        size_t ret = 0;
        FOR(i, DIMS) if(from[i] == to[i]) {
            auto v = from[i];
            if(bounds.getFrom()[i] == v || bounds.getTo()[i] == v) {
                ret++;
            }
        }
        return ret;
    }
    
    bool isNodeOnCell(const CellId& cid) const {
        FOR(i, DIMS) if(from[i] != to[i]) {
            if(cid.getFrom()[i] != from[i] || cid.getTo()[i] != to[i]) return false;
        } else {
            
//            assert(!(cid.getFrom()[i] != from[i] && cid.getTo()[i] != from[i])); // TODO: for now, disallow false here
            if(cid.getFrom()[i] != from[i] && cid.getTo()[i] != from[i]) return false;
        }
        return true;
    }
    
    CellId getReducedDimId(int dim, bool onTo) const {
        if(onTo) {
            return CellId(Point(from.replaceDim(dim, to[dim])), to);
        } else {
            return CellId(from, Point(to.replaceDim(dim, from[dim])));
        }
    }
    

};


template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const CellId<DIMS>& id) {
    return os << "C[" << id.getFrom() << ';'<< id.getTo() << ']';
}

namespace std {
    template<int DIMS>
    struct hash<CellId<DIMS> >: public unary_function<CellId<DIMS>, size_t> {
        const hash<PointBase<DIMS> > subHash;
        size_t operator()(const CellId<DIMS>& id) const {
            size_t ret = 0;
            const size_t PRIME = 920419823;
            ret = subHash(id.getFrom()) + PRIME * subHash(id.getTo());
            return ret;
        }
    };
}


#endif /* defined(__HP3d__CellId__) */
