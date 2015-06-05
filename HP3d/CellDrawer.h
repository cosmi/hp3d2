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
#include <sstream>
#include <cstdio>
#include <ctime>
#include <string>
#include <cstdlib>

#include "EasyBMP.h"
#include "EasyBMP.h"
#include "EasyBMP_Geometry.h"
#include "defs.h"
#include "Point.h"
#include "Color.h"
template<int DIMS>
class CellDrawer;


template<>
class CellDrawer<2> {
    int x1, x2, y1, y2, scale, width, height;
    BMP image;

    CellDrawer<2>(const CellDrawer& that) = delete;
public:
    CellDrawer(int x1, int x2, int y1, int y2, int scale) :
    width(scale*(x2-x1)+1),
    height(scale*(y2-y1)+1),
    scale(scale){
        image.SetSize(width, height);
        image.SetBitDepth(24);
//        image.set_all_channels(255,255,255);
    }
    
    int toX(int p) {
        return (p-x1)*scale;
    }
    int toY(int p) {
        return (p-y1)*scale;
    }
    void draw(const Point<2>& pt, const Color& c = Colors::RED, int radius = 3) {
        DrawArc(image, toX(pt[0]), toY(pt[1]), radius, 0, 7, c);
    }
    
    
    void save(const std::string& s) {
        image.WriteToFile(s.c_str());
    }
    
    void open() {
        std::string fname = "/tmp/celldrawer";
        int tsn = (int)time(nullptr);
        std::string ts = (std::ostringstream() << tsn).str();
        fname+=ts;
        fname+=".bmp";
        save(fname);
        std::system(("open " + fname).c_str());
        std::system(("sleep 3 && rm " + fname).c_str());
    }
};




#endif /* defined(__HP3d__CellDrawer__) */
