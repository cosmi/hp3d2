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
#include <cassert>
#include <iostream>
#include <functional>

#include "defs.h"

enum class PointRelation {
    LESS, LESS_OR_EQ,
    EQ, MORE_OR_EQ, MORE,
    UNORDERED
};

template<int DIMS>
class PointBase {
protected:
    dim_t dims[DIMS];
    PointBase() {
        FOR(i, DIMS) dims[i] = 0;
    }
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
    PointRelation getRelation(const PointBase& pb) const {
        int le = 0, eq = 0, mo = 0;
        FOR(i, DIMS) {
            if(dims[i] < pb[i]) le++;
            else if(dims[i] > pb[i]) mo++;
            else eq++;
        }
        if(mo == 0) {
            if(le == 0) return PointRelation::EQ;
            if(eq == 0) return PointRelation::LESS;
            return PointRelation::LESS_OR_EQ;
        } else {
            if(le != 0) return PointRelation::UNORDERED;
            if(eq == 0) return PointRelation::MORE;
            return PointRelation::MORE_OR_EQ;
        }
    }
    bool hasRelation(const PointBase& pb, PointRelation rel) const {
        auto r = getRelation(pb);
        if(r == rel) return true;
        if(rel == PointRelation::LESS_OR_EQ) {
            return r == PointRelation::EQ || r == PointRelation::LESS;
        }
        if(rel == PointRelation::MORE_OR_EQ) {
            return r == PointRelation::EQ || r == PointRelation::MORE;
        }
        return false;
    }
    
    static PointBase selectMinDims(const PointBase& a, const PointBase& b) {
        PointBase pb;
        FOR(i, DIMS) {
            pb.dims[i] = std::min(a[i], b[i]);
        }
        return pb;
    }
    static PointBase selectMaxDims(const PointBase& a, const PointBase& b) {
        PointBase pb;
        FOR(i, DIMS) {
            pb.dims[i] = std::max(a[i], b[i]);
        }
        return pb;
    }
    PointBase replaceDim(int dim, dim_t value) const {
        auto pb = *this;
        pb.dims[dim] = value;
        return pb;
    }
};

template<int DIMS>
class PointDifference;

template<int DIMS>
class Point : public PointBase<DIMS>{
protected:
    Point() {}
public:
    using PointBaseN = ::PointBase<DIMS>;
    
//    Point(const Point&) = default;
    explicit Point(const PointBaseN& pb):PointBaseN(pb){};
    
    const bool operator==(const Point& pt) const {
        FOR(i, DIMS) if(pt[i] != PointBaseN::dims[i]) return false;
        return true;
    }
    
    Point operator+(const PointDifference<DIMS>&) const;
    Point operator-(const PointDifference<DIMS>&) const;
    
    Point resetBits(int bits) const{
        Point p;
        int mask = ~((1<<bits)-1);
        FOR(i, DIMS) {
            p.dims[i] = PointBaseN::dims[i] & mask;
        }
        return p;
    }
    Point resetLowestBits(int bits) const {
        // alias for resetBits
        return resetBits(bits);
    }
    
    Point resetBitsInMaskedDims(int bits, mask_t dimmask) const {
        Point p;
        int mask = ~((1<<bits)-1);
        FOR(i, DIMS) if(dimmask&(1<<i)){
            p.dims[i] = PointBaseN::dims[i] & mask;
        }
        return p;
    }
    
    static Point origin() {
        return Point();
    }
};



template<int DIMS>
class PointDifference: public PointBase<DIMS> {
protected:
    PointDifference(){}
    
    explicit PointDifference(dim_t size) {
        FOR(i, DIMS) this->dims[i] = size;
    }
public:
    using PointBase = ::PointBase<DIMS>;
    using Point = ::Point<DIMS>;

    const char* getTypePrefix() const { return "PD"; }
    
    explicit PointDifference(const PointBase&p):PointBase(p) {}
//    PointDifference(std::initializer_list<dim_t> list ):PointBase(list) {}
    PointDifference(const Point & p, const Point & q):PointBase(q) {
        FOR(i, DIMS) this->dims[i] -= p[i];
    }
    bool operator==(const PointDifference& pd) const {
        FOR(i, DIMS) if(this->dims[i] != pd[i]) return false;
        return true;
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
    PointDifference operator*(const int& by) const {
        PointDifference pd;
        FOR(i, DIMS) {
            pd.dims[i] = (*this)[i]*by;
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
    PointDifference selectDim(int dim) const {
        PointDifference pd;
        pd.dims[dim] = PointBase::dims[dim];
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
    size_t getLongestDim() const {
        auto ret = std::max_element(PointBase::dims, PointBase::dims+DIMS) - PointBase::dims;
        return ret;
    }
//    size_t getLongestBits() const {
//        dim_t val = PointBase::dims[getLongestDim()];
//        dim_t cp = 1;
//        size_t bits = 0;
//        while(cp < val) {
//            cp<<=1;
//            bits++;
//        }
//        assert(cp == val);
//        return bits;
//    }
    
    int countZeroBits() const {
//        assert(isCube());
        int p = 0;
        FOR(i, DIMS) {
            if(PointBase::dims[i] > 0) {
                p = PointBase::dims[i];
                break;
            }
        }
        assert(p>0);
        int cnt = 0;
        while((p&1) == 0) {
            cnt++;
            p>>=1;
        }
        return cnt;
    }
    dim_t getSide() const {
        assert(isCube());
        return PointBase::dims[0];
    }
    
    static PointDifference cube(dim_t size) {
        return PointDifference(size);
    }
    
    PointDifference setNonZeroDimsTo(dim_t value) const {
        PointDifference pd = *this;
        FOR(i, DIMS) if(pd[i] > 0) pd.dims[i] = value;
    }
    mask_t getNonZeroMask() const {
        mask_t m = 0;
        FOR(i, DIMS) if(PointBase::dims[i] > 0) m|=(1<<i);
        return m;
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

namespace std {
    template<int DIMS>
    struct hash<PointBase<DIMS> >: public unary_function<PointBase<DIMS>, size_t> {
        const hash<dim_t> subHash;
        
        hash():subHash(){}
        size_t operator()(const PointBase<DIMS>& pb) const {
            size_t ret = 0;
            const size_t PRIME = 961748941;
            FOR(i, DIMS) {
                ret *= PRIME;
                ret += subHash(pb[i]);
            }
            return ret;
        }
    };
}
#endif /* defined(__HP3d__Point__) */
