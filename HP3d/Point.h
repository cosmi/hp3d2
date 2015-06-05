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
    dimType dims[DIMS];
public:
    PointBase(std::initializer_list<dimType> list ) {
        int i = 0;
        for( auto elem: list) {
            dims[i++] = elem;
        }
    }
    const dimType& operator[](int dim) const {
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
public:
    using PointBase = PointBase<DIMS>;
    
//    Point(const Point&) = default;
    explicit Point(const PointBase& pb):PointBase(pb){};
    
    const bool operator==(const Point& pt) const {
        FOR(i, DIMS) if(pt[i] != PointBase::dims[i]) return false;
        return true;
    }
//    const bool operator<=(const Point& pt) const {
//        FOR(i, DIMS) if(pt[i] != dims[i]) return dims[i] <= pt[i];
//        return true;
//    }
//    const bool operator<(const Point& pt) const {
//        FOR(i, DIMS) if(pt[i] != dims[i]) return dims[i] <= pt[i];
//        return false;
//    }
//    const bool operator>=(const Point& pt) const {
//        return pt <= *this;
//    }
//    const bool operator>(const Point& pt) const {
//        return pt < *this;
//    }
    
    Point operator+(const PointDifference<DIMS>&);
    Point operator-(const PointDifference<DIMS>&);
};



template<int DIMS>
class PointDifference: public PointBase<DIMS> {
public:
    using PointBase = PointBase<DIMS>;
    using Point = Point<DIMS>;

    const char* getTypePrefix() const { return "PD"; }
    
    explicit PointDifference(const PointBase&p):PointBase(p) {}
    PointDifference(std::initializer_list<dimType> list ):PointBase(list) {}
    PointDifference(const Point & p, const Point & q):PointBase(q) {
        FOR(i, DIMS) this->dims[i] -= p[i];
    }
    
    PointDifference operator-() {
        PointDifference pd;
        FOR(i, DIMS) {
            pd.dims[i] = -(*this)[i];
        }
        return pd;
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
Point<DIMS> Point<DIMS>::operator+(const PointDifference<DIMS>& d) {
    Point p = *this;
    FOR(i, DIMS) {
        p[i]+=d[i];
    }
    return p;
}
template<int DIMS>
Point<DIMS> Point<DIMS>::operator-(const PointDifference<DIMS>& d) {
    Point p = *this;
    FOR(i, DIMS) {
        p[i]-=d[i];
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
