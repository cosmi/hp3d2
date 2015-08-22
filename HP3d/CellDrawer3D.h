//
//  CellDrawer3D.h
//  HP3d
//
//  Created by Marcin on 22.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellDrawer3D__
#define __HP3d__CellDrawer3D__


#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cmath>

#include "EasyBMP.h"
#include "EasyBMP.h"
#include "EasyBMP_Geometry.h"
#include "EasyBMP_Font.h"
#include "defs.h"
#include "Point.h"
#include "CellId.h"
#include "CellSpace.h"
#include "CellNodeSpace.h"
#include "CellIdSet.h"
#include "Color.h"
#include "DivisionStrategy.h"

struct DrawPoint {
    double x; double y; double z;
    DrawPoint(double x = 0, double y = 0, double z = 0):x(x), y(y), z(z) {}
    DrawPoint(const Point<3>& pt):DrawPoint(pt[0], pt[1], pt[2]) {}
    DrawPoint(const Point<2>& pt):DrawPoint(pt[0], pt[1]) {}
    
};

inline std::ostream& operator<<(std::ostream& os,
                         const DrawPoint& pt) {
    return os << "$[" << pt.x << ";" << pt.y << ";" << pt.z << "]";
}



class CellDrawer3D {
    static int counter;
    int width, height;
    double scale;
    DrawPoint camera, angle;
    BMP image;
    CellDrawer3D(const CellDrawer3D& that) = delete;
    
    
public:
    DrawPoint project(const DrawPoint& pt) {
        double sx = sin(angle.x), sy = sin(angle.y), sz = sin(angle.z);
        double cx = cos(angle.x), cy = cos(angle.x), cz = cos(angle.z);
        double x = pt.x - camera.x, y = pt.y - camera.y, z = pt.z-camera.z;
        double ex = -camera.x, ey = -camera.y, ez = -camera.z;
        double dt = (cy*z + sy*(sz*y + cz*x));
        
        
        double dx = cy*(sz*y + cz*x) - sy*z;
        double dy = sx*dt + cx*(cz*y - sz*x);
        double dz = cx*dt - sx*(cz*y - sz*x);
        double bx = ez/dz*dx - ex;
        double by = ez/dz*dy - ey;
        return DrawPoint(bx, by);
    }
    
    CellDrawer3D(int width, int height):width(width), height(height) {
        
        image.SetSize(width, height);
        image.SetBitDepth(24);
    }
    
    void setCamera(const DrawPoint& camera, const DrawPoint& angle, double scale) {
        this->camera = camera;
        this->angle = angle;
        this->scale = scale;
    }
    
//    void init(double x1, double x2, double y1, double y2, double scale) {
//        width = (scale*(x2-x1)+1);
//        height = (scale*(y2-y1)+1);
//        offset = DrawPoint(x1, y1);
//        boundary = DrawPoint(x2, y2);
//        
//        camera = DrawPoint((x1+x2)/2, (y1+y2)/2, -1);
//        angle = DrawPoint(0.15,0.15,0);
//        this->scale = scale;
//    }
//    
////    CellDrawer3D(double x1, double x2, double y1, double y2, double scale) {
////         init(x1,x2,y1,y2,scale);
////    }
//    
//    CellDrawer3D(const DrawPoint& a, const DrawPoint& b, double scale) {
//        
//        camera = DrawPoint((a.x + b.x)/2, (a.y+b.y)/2, 4);
//
//        DrawPoint a1 = project(a);
//        DrawPoint b1 = project(b);
//        std::cout << a <<"->" << a1 << " " << b << "->" << b1 << std::endl;
//        init(a1.x, b1.x, a1.y, b1.y, scale);
//        std::cout << "INIT " << a1 << "-" << b1 << std::endl;
//    }
//    
//    template<int DIMS>
//    CellDrawer3D(const CellId<DIMS> & bounds, int scale) : CellDrawer3D(bounds.getFrom(), bounds.flattenDimsByBitMask(4, 0).getTo(), scale) {
//        std::cout << "INIT " << bounds << std::endl;
//    }
//    
    
    void drawLine(DrawPoint a, DrawPoint b, const Color& color = Colors::BLACK) {
        std::cout << "LINE0 " << a << "-" << b << std::endl;
        a = project(a);
        b = project(b);
        std::cout << "LINE1 " << a << "-" << b << std::endl;
//        std::cout << "DOES " << (offset.x + a.x)*scale<< ' ' <<(offset.y + a.y)*scale<< ' ' <<(offset.x + b.x)*scale<< ' ' <<(offset.y + b.y)*scale<< std::endl;
//        DrawFastLine(image, (offset.x + a.x)*scale, (offset.y + a.y)*scale, (offset.x + b.x)*scale, (offset.y + b.y)*scale, color);
        std::cout << " DOES " << a.x << " " << a.y << " " <<b.x << " " << b.y << std::endl;
        DrawFastLine(image, a.x*scale, a.y*scale, b.x*scale, b.y*scale, color);
    }
    
    template<int DIMS>
    void drawCell(const CellId<DIMS>& bounds, const Color& color = Colors::BLACK) {
        auto lines = bounds.getLowerDimensionalityBounds(1);
        std::cout << "BT " << bounds << " " << lines.size() << std::endl;
        for(auto & line: lines) {
            drawLine(line.getFrom(), line.getTo(), color);
        }
    }
    
    void save(const std::string& s) {
        image.WriteToFile(s.c_str());
    }
    
    void open(bool removeFile = true) {
        std::string fname = "/tmp/celldrawer3d";
        int tsn = (int)time(nullptr);
        std::string ts = toString(tsn);
        fname+=ts;
        fname+="-";
        fname+=toString(counter++);
        fname+=".bmp";
        save(fname);
        std::system(("open " + fname).c_str());
        if(removeFile) std::system(("sleep 3 && rm " + fname).c_str());
    }
};

#endif /* defined(__HP3d__CellDrawer3D__) */
