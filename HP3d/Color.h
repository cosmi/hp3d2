//
//  Color.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__Color__
#define __HP3d__Color__

#include "EasyBMP.h"

struct Color {
    unsigned char r, g, b;
    Color(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b) {}
    operator RGBApixel() const {
        RGBApixel p;
        p.Red = r;
        p.Green = g;
        p.Blue = b;
        return p;
    }
};

namespace Colors {
    const Color RED = Color(255, 0, 0);
    const Color BLACK = Color(0, 0, 0);
    const Color WHITE = Color(255, 255, 255);
    const Color BLUE = Color(0, 0, 255);
    const Color GREEN = Color(0, 255, 0);
    const Color MAGENTA = Color(255, 0, 255);
}

#endif /* defined(__HP3d__Color__) */
