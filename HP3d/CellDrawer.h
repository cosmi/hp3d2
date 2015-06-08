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
#include "EasyBMP_Font.h"
#include "defs.h"
#include "Point.h"
#include "CellId.h"
#include "CellSpace.h"
#include "CellNodeSpace.h"
#include "CellIdSet.h"
#include "Color.h"
template<int DIMS>
class CellDrawer;

#include <typeinfo>
template<>
class CellDrawer<2> {
    int x1, x2, y1, y2, scale, width, height;
    BMP image;

    CellDrawer<2>(const CellDrawer& that) = delete;
public:
    CellDrawer(int x1, int x2, int y1, int y2, int scale) :
    x1(x1), x2(x2), y1(y1), y2(y2),
    width(scale*(x2-x1)+1),
    height(scale*(y2-y1)+1),
    scale(scale){
        image.SetSize(width, height);
        image.SetBitDepth(24);
    }
    CellDrawer(const CellId<2> & bounds, int scale)
        :CellDrawer(bounds.getFrom()[0], bounds.getTo()[0], bounds.getFrom()[1], bounds.getTo()[1], scale) {
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
    void drawMidPoint(const CellId<2>& i, const Color& c = Colors::MAGENTA, int radius = 2) {
        int x1 = toX(i.getFrom()[0]);
        int x2 = toX(i.getTo()[0]);
        int y1 = toX(i.getFrom()[1]);
        int y2 = toX(i.getTo()[1]);
        DrawArc(image, (x1+x2)/2, (y1+y2)/2, radius, 0, 7, c);
    }
    
    void printAtMidPoint(const CellId<2>& i, const std::string& s, const Color& c = Colors::BLACK) {
        int x1 = toX(i.getFrom()[0]);
        int x2 = toX(i.getTo()[0]);
        int y1 = toX(i.getFrom()[1]);
        int y2 = toX(i.getTo()[1]);
        int x = (x1+x2)/2 + 3;
        int y = (y1+y2)/2 + 3;
        if(y > height-10) {
            y-= 16;
        }
        if(x > width-10) {
            x-= 16;
        }
        PrintString(image, s.c_str(),  x, y, 10, c);
    }
    void draw(const CellId<2>& i, const Color& c1 = Colors::BLUE, const Color& c2 = Colors::GREEN, const Color& c3 = Colors::MAGENTA, int radius = 2) {
        int x1 = toX(i.getFrom()[0]);
        int x2 = toX(i.getTo()[0])-1;
        int y1 = toX(i.getFrom()[1]);
        int y2 = toX(i.getTo()[1])-1;
        DrawFastLine(image, x2, y2, x1, y2, c2);
        DrawFastLine(image, x2, y2, x2, y1, c2);
        DrawFastLine(image, x1, y1, x1, y2, c1);
        DrawFastLine(image, x1, y1, x2, y1, c1);
//        drawMidPoint(i, c3, radius);
    }
    
    
    void draw(const CellSpace<2> & cs) {
        for(auto & cell : cs.getCells()) {
            draw(cell.getId());
        }
    }
    void draw(const CellNodeSpace<2> & cs) {
        for(auto & cell : cs.getLeaves()) {
            draw(cell.getId());
        }
        for(auto & id : cs.getConstrainedNodes()) {
            drawMidPoint(id, Colors::RED,4 + 2*id.countNonZeroDims());
            
        }
        for(auto & id : cs.getFreeNodes()) {
            drawMidPoint(id, Colors::GREEN, 4 + 2*id.countNonZeroDims());
        }
        
        for(auto & it : cs.getNodeToCellsMapping()) {
            printAtMidPoint(it.first, toString(it.second.size()));
            
        }
    }

    void draw(const CellIdSet<2> & cis) {
        for(auto & cid : cis.getIds()) {
            draw(cid);
        }
    }

    void save(const std::string& s) {
        image.WriteToFile(s.c_str());
    }
    
    void open(bool removeFile) {
        std::string fname = "/tmp/celldrawer";
        int tsn = (int)time(nullptr);
        std::string ts = toString(tsn);
        fname+=ts;
        fname+=".bmp";
        save(fname);
        std::system(("open " + fname).c_str());
        if(removeFile) std::system(("sleep 3 && rm " + fname).c_str());
    }
};




#endif /* defined(__HP3d__CellDrawer__) */
