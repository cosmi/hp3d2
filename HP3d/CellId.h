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
#include <list>
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
    CellId(std::initializer_list<dim_t> from, std::initializer_list<dim_t> to) : from(from), to(to) {}
    static CellId unit() {
        return CellId(Point::origin(), PointDifference::cube(1));
    }
    static CellId null() {
        return CellId(Point::origin(), Point::origin());
    }
    bool isValid() const {
        FOR(i, DIMS) {
            if(from[i] > to[i]) return false;
        }
        return true;
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
    CellId operator+(const PointDifference & pd) const {
        return CellId(getFrom() + pd, getTo() + pd);
    }
    bool isCube() const {
        auto size = getSize();
        return size.isCube();
    }
    bool isAligned() const {
        auto size = getSize();
        auto & from = getFrom();
        FOR(i, DIMS) {
            dim_t p = size[i]-1;
            if(from[i] & p) return false;
        }
        return true;
    }
    bool operator==(const CellId& cid) const {
        return getFrom() == cid.getFrom() && getTo() == cid.getTo();
    }
    bool operator!=(const CellId& cid) const {
        return getFrom() != cid.getFrom() || getTo() != cid.getTo();
    }
    
    CellId getChild(mask_t id) const {
        auto pd = getSize()>>1;
        auto bmpd = pd.selectDimsByBitMask(id);
        return CellId(getFrom() + bmpd, pd);
    }
    
    constexpr size_t getChildrenCount() const {
        return 1<<DIMS;
    }
    
    
    
    CellId move(int direction, int distance) const {
        auto size = getSize();
        auto & from = getFrom();
        auto diff = size.selectDim(direction) * distance;
        return CellId(from+diff, size);
    }
    
    CellId moveDimension(size_t dimension, dim_t distance) const {
        return *this + PointDifference::singleDimension(dimension, distance);
    }
    CellId withDimension(size_t dim, dim_t from, dim_t to) const {
        return CellId(getFrom().withDimension(dim, from), getTo().withDimension(dim, to));
    }
    
    bool covers(const CellId& cid) const {
        return getFrom().hasRelation(cid.getFrom(), PointRelation::LESS_OR_EQ)
            && getTo().hasRelation(cid.getTo(), PointRelation::MORE_OR_EQ);
    }
    /*
     * Has at least common boundary?
     */
    bool touches(const CellId& cid) const {
        FOR(i, DIMS) {
            if(to[i]<cid.getFrom()[i] || from[i] > cid.getTo()[i]) return false;
        }
        return true;
    }
    
    bool touchesInteriorOf(const CellId& cid) const {
        FOR(i, DIMS) {
            if(getTo()[i] <= cid.getFrom()[i] || getFrom()[i] >= cid.getTo()[i]) return false;
        }
        return true;
    }
    
    bool isSideOf(const CellId& cid) const {
        int eq = 0;
        int touch = 0;
        FOR(i, DIMS) {
            if(getFrom()[i] == getTo()[i]) {
                if(getFrom()[i] == cid.getFrom()[i] || getFrom()[i] == cid.getTo()[i]) touch++;
            } else {
                if(getFrom()[i] == cid.getFrom()[i] && getTo()[i] == cid.getTo()[i]) eq++;
            }
        }
        return (eq + touch == DIMS) && touch == 1;
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
        assert(size[splittingDim]%2 == (dim_t)0);
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
    size_t getDimensionality() const {
        return countNonZeroDims();
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
    
    using IdList = std::list<CellId>;
    IdList getLowerDimensionalityIds() const {
        IdList list;
        FOR(i, DIMS) if(from[i] != to[i]) {
            list.push_back(getReducedDimId(i, false));
            list.push_back(getReducedDimId(i, true));
        }
        return list;
    }
    
private:
    
    void getSameSizeNeighborsHelper(IdList& list, int dim) const {

        if(dim == DIMS) {
            list.push_back(*this);
        } else {
            for(int i = -1; i<=1; i++) {
                auto id = move(dim, i);
                id.getSameSizeNeighborsHelper(list, dim+1);
            }
        }
    }
public:
    
    IdList getSameSizeNeighbors() const {
        IdList list;
        getSameSizeNeighborsHelper(list, 0);
        return list;
    }
    
    CellId getAlignedAncestor(int bits) const {
        auto size = getSize();
        mask_t dimmask = size.getNonZeroMask();
        auto newpt = from.resetBitsInMaskedDims(bits, dimmask);
        auto newsize = size.setNonZeroDimsTo(1<<bits);
        return CellId(newpt, newsize);
    }
    
    CellId getAlignedParent() const {
        auto size = getSize();
        auto nsize = size << 1;
        mask_t dimmask = size.getNonZeroMask();
        int bits = nsize.countZeroBits();
        auto pt = getFrom().resetBitsInMaskedDims(bits, dimmask);
        return CellId(pt, nsize);
    }
    
    static CellId intersection(const CellId& a, const CellId& b)  {
        return CellId(Point(PointBase::selectMaxDims(a.getFrom(), b.getFrom())),
                      Point(PointBase::selectMinDims(a.getTo(), b.getTo())));
    }
    
    size_t hash() const {
        size_t ret = 0;
        const size_t PRIME = 920419823;
        ret = getFrom().hash() + PRIME * getTo().hash();
        return ret;
    }
    
    static CellId getForSize(std::initializer_list<dim_t> list) {
        return CellId(Point::origin(), Point(list));
    }
};


template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const CellId<DIMS>& id) {
//    return os << "C[" << id.getFrom() << ';'<< id.getTo() << ']';
    
    os << "C[";
    bool first = true;
    FOR(i, DIMS) {
        if(!first) os << ';';
        os << id.getFrom()[i] << ".." << id.getTo()[i];
        first = false;
    }
    return os << ']';
}

namespace std {
    template<int DIMS>
    struct hash<CellId<DIMS> >: public unary_function<CellId<DIMS>, size_t> {

        size_t operator()(const CellId<DIMS>& id) const {
            return id.hash();
        }
        hash() {}
    };
}


#endif /* defined(__HP3d__CellId__) */
