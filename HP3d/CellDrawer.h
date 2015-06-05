//
//  CellDrawer.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellDrawer__
#define __HP3d__CellDrawer__

#include <algorithm>
#include <iostream>
#include <string>

#include "bitmap_image.hpp"
#include "defs.h"
#include "Point.h"
template<int DIMS>
class CellDrawer;

template<>
class CellDrawer<2> {
    int x1, x2, y1, y2, scale, width, height;
    bitmap_image image;

    CellDrawer<2>(const CellDrawer& that) = delete;
public:
    CellDrawer(int x1, int x2, int y1, int y2, int scale) :
    width(scale*(x2-x1+1)),
    height(scale*(y2-y1+1)),
    scale(scale),
    image(width, height)
    {
        image.set_all_channels(255,255,255);
    }
    
    int toX(int p) {
        return (p-x1)*scale;
    }
    int toY(int p) {
        return (p-y1)*scale;
    }
    void draw(const Point<2>& pt) {
        
    }
    
    
    void save(const std::string& s) {
        image.save_image(s);
    }
};




#endif /* defined(__HP3d__CellDrawer__) */
