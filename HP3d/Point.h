//
//  Point.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__Point__
#define __HP3d__Point__
#include <algorithm>
#include <iostream>

#include "defs.h"

template<int DIMS>
class PointBase {
protected:
    PointBase() {}
    dim_t dims[DIMS];
public:
    PointBase(std::initializer_list<dim_t> list ) {
        int i = 0;
        for( auto elem: list) {
            dims[i++] = elem;
        }
    }
    const dim_t& operator[](int dim) const {
        return PointBase::dims[dim];
    }
    
    void printTo(std::ostream & os) const {
        os << "(";
        for(int i = 0; i<DIMS; i++) {
            if(i>0) os << ',';
            os << dims[i];
        }
        os << ")";
    }

};

template<int DIMS>
class PointDifference;

template<int DIMS>
class Point : public PointBase<DIMS>{
protected:
    Point() {}
public:
    using PointBase = PointBase<DIMS>;
    
//    Point(const Point&) = default;
    explicit Point(const PointBase& pb):PointBase(pb){};
    
    const bool operator==(const Point& pt) const {
        FOR(i, DIMS) if(pt[i] != PointBase::dims[i]) return false;
        return true;
    }
    
    Point operator+(const PointDifference<DIMS>&) const;
    Point operator-(const PointDifference<DIMS>&) const;
    
    Point resetBits(int bits) const{
        Point p;
        int mask = ~((1<<bits)-1);
        FOR(i, DIMS) {
            p.dims[i] = PointBase::dims[i] & mask;
        }
        return p;
    }
};



template<int DIMS>
class PointDifference: public PointBase<DIMS> {
protected:
    PointDifference(){}
public:
    using PointBase = PointBase<DIMS>;
    using Point = Point<DIMS>;

    const char* getTypePrefix() const { return "PD"; }
    
    explicit PointDifference(const PointBase&p):PointBase(p) {}
//    PointDifference(std::initializer_list<dim_t> list ):PointBase(list) {}
    PointDifference(const Point & p, const Point & q):PointBase(q) {
        FOR(i, DIMS) this->dims[i] -= p[i];
    }
    
    PointDifference operator-() const{
        PointDifference pd;
        FOR(i, DIMS) {
            pd.dims[i] = -(*this)[i];
        }
        return pd;
    }
    PointDifference operator>>(const int& by) const {
        PointDifference pd;
        int mask = (1<<by)-1;
        FOR(i, DIMS) {
            assert(((*this)[i]&mask) == 0);
            pd.dims[i] = (*this)[i]>>by;
        }
        return pd;
    }

    PointDifference operator<<(const int& by) const {
        PointDifference pd;
        FOR(i, DIMS) {
            pd.dims[i] = (*this)[i]<<by;
        }
        return pd;
    }

    
    PointDifference selectDimsByBitMask(mask_t mask) const {
        PointDifference pd;
        FOR(i, DIMS) {
            pd.dims[i] = (mask&(1<<i))?(*this)[i]:0;
        }
        return pd;
    }
    bool isCube() const {
        int p = PointBase::dims[0];
        if(p == 0) return false;
        FOR(i, DIMS) {
            if(p != PointBase::dims[i]) return false;
        }
        return true;
    }
    
    int countZeroBits() const {
        assert(isCube());
        int p = PointBase::dims[0];
        int cnt = 0;
        while((p&1) == 0) {
            cnt++;
            p>>=1;
        }
        return cnt;
    }

};


template<int DIMS>
PointDifference<DIMS> operator- (const Point<DIMS>& p, const Point<DIMS>& q) {
    return PointDifference<DIMS>(q, p);
}

template<int DIMS>
Point<DIMS> operator- (const Point<DIMS>& p, const PointDifference<DIMS>& q) {
    Point<DIMS> pd = p;
    pd -= q;
    return pd;
}
template<int DIMS>
Point<DIMS> Point<DIMS>::operator+(const PointDifference<DIMS>& d) const {
    Point p = *this;
    FOR(i, DIMS) {
        p.dims[i]+=d[i];
    }
    return p;
}
template<int DIMS>
Point<DIMS> Point<DIMS>::operator-(const PointDifference<DIMS>& d) const {
    Point p = *this;
    FOR(i, DIMS) {
        p.dims[i]-=d[i];
    }
    return p;
}

template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const Point<DIMS>& point) {
    os << "P";
    point.printTo(os);
    return os;
}
template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const PointDifference<DIMS>& point) {
    os << "PD";
    point.printTo(os);
    return os;
}
#endif /* defined(__HP3d__Point__) */
