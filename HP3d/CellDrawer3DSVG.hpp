//
//  CellDrawer3DSVG.hpp
//  HP3d
//
//  Created by Marcin on 21.11.2015.
//  Copyright © 2015 Marcin. All rights reserved.
//

#ifndef CellDrawer3DSVG_hpp
#define CellDrawer3DSVG_hpp

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

#include "simple_svg.h"


class CellDrawer3DSVG {
    static int counter;
    int width, height;
    double scale;
    DrawPoint camera, angle, offset;
    std::string filename;
    svg::Document * output;
    CellDrawer3DSVG(const CellDrawer3DSVG& that) = delete;
    
    
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
    
    CellDrawer3DSVG(int width, int height, std::string fname = ""):width(width), height(height) {

        if(fname == "") {
            fname = "/tmp/celldrawer3d";
            int tsn = (int)time(nullptr);
            std::string ts = toString(tsn);
            fname+=ts;
            fname+="-";
            fname+=toString(counter++);
            fname+=".svg";

        }
        filename = fname;
        using namespace svg;
        output = new Document(filename, Layout(Dimensions(width, height), Layout::BottomLeft));
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
        using namespace svg;
        *output << Line(a, b, Stroke(0.5, color));
    }
    
    template<int DIMS>
    void drawCell(const CellId<DIMS>& bounds, const Color& color = Colors::BLACK) {
        auto lines = bounds.getLowerDimensionalityBounds(1);
        std::cout << "BT " << bounds << " " << lines.size() << std::endl;
        for(auto & line: lines) {
            drawLine(line.getFrom(), line.getTo(), color);
        }
    }
    
    void save() {
        output->save();
    }
    
    void open(bool removeFile = true) {
        output->save();
        std::system(("/usr/bin/open -a '/Applications/Google Chrome.app' " + filename).c_str());
        if(removeFile) std::system(("sleep 3 && rm " + filename).c_str());
    }
};



#endif /* CellDrawer3DSVG_hpp */
