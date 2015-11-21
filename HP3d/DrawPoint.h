//
//  DrawPoint.h
//  HP3d
//
//  Created by Marcin on 21.11.2015.
//  Copyright © 2015 Marcin. All rights reserved.
//

#ifndef DrawPoint_h
#define DrawPoint_h
#include "simple_svg.h"

struct DrawPoint {
    double x; double y; double z;
    DrawPoint(double x = 0, double y = 0, double z = 0):x(x), y(y), z(z) {}
    DrawPoint(const Point<3>& pt):DrawPoint(pt[0], pt[1], pt[2]) {}
    DrawPoint(const Point<2>& pt):DrawPoint(pt[0], pt[1]) {}
    operator svg::Point() const {
        return svg::Point(x, y);
    }
};

inline std::ostream& operator<<(std::ostream& os,
                                const DrawPoint& pt) {
    return os << "$[" << pt.x << ";" << pt.y << ";" << pt.z << "]";
}

#endif /* DrawPoint_h */
