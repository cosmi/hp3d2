//
//  Color.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__Color__
#define __HP3d__Color__


struct Color {
    unsigned char r, g, b;
    Color(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b) {}
};

namespace Colors {
    const Color RED = Color(255, 0, 0);
    const Color BLACK = Color(0, 0, 0);
    const Color WHITE = Color(255, 255, 255);
    const Color BLUE = Color(0, 0, 255);
    const Color GREEN = Color(0, 255, 0);
}

#endif /* defined(__HP3d__Color__) */
