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
#include "DivisionStrategy.h"
template<int DIMS>
class CellDrawer;

#include <typeinfo>
template<>
class CellDrawer<2> {
    static int counter;
    dim_t x1, x2, y1, y2;
    int scale, width, height;
    BMP image;
    bool shouldDrawNodesCount;
    CellDrawer<2>(const CellDrawer& that) = delete;
public:
    CellDrawer(dim_t x1, dim_t x2, dim_t y1, dim_t y2, int scale) :
    x1(x1), x2(x2), y1(y1), y2(y2),
    width(scale*int(x2-x1)+1),
    height(scale*int(y2-y1)+1),
    scale(scale),
    shouldDrawNodesCount(false) {
        image.SetSize(width, height);
        image.SetBitDepth(24);
    }
    CellDrawer(const CellId<2> & bounds, int scale)
        :CellDrawer(bounds.getFrom()[0], bounds.getTo()[0], bounds.getFrom()[1], bounds.getTo()[1], scale) {
    }
    
    int toX(dim_t p) {
        return (int)(p-x1)*scale;
    }
    int toY(dim_t p) {
        return (int)(p-y1)*scale;
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
    
    void printAtCorner(const CellId<2>& i, const std::string& s, const Color& c = Colors::BLACK) {
        int x1 = toX(i.getFrom()[0]);
        int x2 = toX(i.getTo()[0]);
        int y1 = toX(i.getFrom()[1]);
        int y2 = toX(i.getTo()[1]);
        int x = x1 + 5;// - 8*(int)s.size();
        int y = y2 - 16;
        if(y > height-10) {
            y-= 16;
        }
        if(x > width-10) {
            x-= 16;
        }
        PrintString(image, s.c_str(),  x, y, 10, c);
    }
    void draw(const CellId<2>& i, const Color& c1 = Colors::BLUE, const Color& c2 = Colors::GREEN, const int pixelOffset = 0) {
        int x1 = toX(i.getFrom()[0]) + pixelOffset;
        int x2 = toX(i.getTo()[0])-1 - pixelOffset;
        int y1 = toX(i.getFrom()[1]) + pixelOffset;
        int y2 = toX(i.getTo()[1])-1 - pixelOffset;
        DrawFastLine(image, x2, y2, x1, y2, c2);
        DrawFastLine(image, x2, y2, x2, y1, c2);
        DrawFastLine(image, x1, y1, x1, y2, c1);
        DrawFastLine(image, x1, y1, x2, y1, c1);
//        drawMidPoint(i, c3, radius);
    }
    
    
    void draw(const CellSpace<2> & cs) {
        for(auto & cell : cs.getCells()) {
            draw(cell.getId(), Colors::BLACK, Colors::GRAY);
        }
    }
    void draw(const CellNodeSpace<2> & cs) {
        for(auto & cell : cs.getLeaves()) {
            draw(cell.getId());
        }
        for(auto & id : cs.getConstrainedNodes()) {
            drawMidPoint(id, Colors::RED,4 + 2*(int)id.countNonZeroDims());
            
        }
        for(auto & id : cs.getFreeNodes()) {
            drawMidPoint(id, Colors::GREEN, 4 + 2*(int)id.countNonZeroDims());
        }
        
        if(this->shouldDrawNodesCount) {
            for(auto & it : cs.getNodeToCellsMapping()) {
                printAtMidPoint(it.first, toString(it.second.size()));
                
            }
        }
    }

    void draw(const CellIdSet<2> & cis) {
        for(auto & cid : cis.getIds()) {
            draw(cid);
        }
    }
    
    void draw(const DivisionTree<2> & dt, const Color* colors, size_t colorsCount, size_t index, int offsetMultiplier) {
        if(dt.hasChildren()) {
            FOR(i, 2) {
                draw(dt.getChild(i), colors, colorsCount, index+1, offsetMultiplier);
            }
        }
        if(index > 0) {
            draw(dt.getIds().getBounds(), colors[index%colorsCount], colors[index%colorsCount], 2 + ((int)index-1)*offsetMultiplier);
        } else if(offsetMultiplier == 0) {
            draw(dt.getIds().getBounds(), Colors::WHITE, Colors::WHITE, 2 + ((int)index-1)*offsetMultiplier);
        }
    }
    
    void draw(const DivisionTree<2> & dt, bool offset = false) {
        draw(dt, Colors::CYAN_SEQ, Colors::CYAN_SEQ_LEN, 0, offset?2:0);
        printAtCorner(dt.getIds().getBounds(), "Cost: " + toString(dt.getCost()));
    }

    void save(const std::string& s) {
        image.WriteToFile(s.c_str());
    }
    
    void open(bool removeFile) {
        std::string fname = "/tmp/celldrawer";
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




#endif /* defined(__HP3d__CellDrawer__) */
