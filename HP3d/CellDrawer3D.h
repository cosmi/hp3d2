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
#include "DrawPoint.h"


class CellDrawer3D {
    static int counter;
    int width, height;
    double scale;
    DrawPoint camera, angle, offset;
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
    
    void setCamera(const DrawPoint& camera, const DrawPoint& angle, double scale, const DrawPoint& offset) {
        this->camera = camera;
        this->angle = angle;
        this->scale = scale;
        this->offset = offset;
    }
    

    
    void drawLine(DrawPoint a, DrawPoint b, const Color& color = Colors::BLACK) {
        std::cout << "LINE0 " << a << "-" << b << std::endl;
        a = project(a);
        b = project(b);
        std::cout << "LINE1 " << a << "-" << b << std::endl;
//        std::cout << "DOES " << (offset.x + a.x)*scale<< ' ' <<(offset.y + a.y)*scale<< ' ' <<(offset.x + b.x)*scale<< ' ' <<(offset.y + b.y)*scale<< std::endl;
//        DrawFastLine(image, (offset.x + a.x)*scale, (offset.y + a.y)*scale, (offset.x + b.x)*scale, (offset.y + b.y)*scale, color);
        std::cout << " DOES " << a.x << " " << a.y << " " <<b.x << " " << b.y << std::endl;
        DrawFastLine(image, a.x*scale-offset.x, a.y*scale-offset.y, b.x*scale-offset.x, b.y*scale-offset.y, color);
    }
    
    template<int DIMS>
    void drawCell(const CellId<DIMS>& bounds, const Color& color = Colors::BLACK) {
        auto lines = bounds.getLowerDimensionalityBounds(1);
        std::cout << "BT " << bounds << " " << lines.size() << std::endl;
        for(auto & line: lines) {
            drawLine(line.getFrom(), line.getTo(), color);
        }
    }
    
    template<int DIMS>
    void drawExternalLines(const CellId<DIMS>& bounds, const CellId<DIMS>& externalBounds, const Color& color = Colors::BLACK) {
        auto lines = bounds.getLowerDimensionalityBounds(1);
        std::cout << "BT " << bounds << " " << lines.size() << std::endl;
        for(auto & line: lines) {
            if(line.isOnFrontalSideOf(externalBounds)) {
                drawLine(line.getFrom(), line.getTo(), color);
            }
        }
    }
    template<int DIMS>
    void drawExternalLines(const CellIdSet<DIMS>& ids, const CellId<DIMS>& bounds, const Color& color = Colors::BLACK) {
        for(auto& id: ids) {
            drawExternalLines(id, bounds);
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
