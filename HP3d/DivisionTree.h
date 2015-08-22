//
//  DivisionTree.h
//  HP3d
//
//  Created by Marcin on 11.08.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__DivisionTree__
#define __HP3d__DivisionTree__


#include <algorithm>
#include <memory>
#include "defs.h"
#include "CellIdSet.h"

template<int DIMS>
class DivisionTree {
public:
    using ptr = std::shared_ptr<DivisionTree>;
protected:
    CellIdSet<DIMS> ids;
    ptr ch[2];
    result_t cost;
    
    DivisionTree(CellIdSet<DIMS> ids, const ptr& a, const ptr& b):ids(ids), cost(0) {
        ch[0] = a; ch[1] = b;
    }
    DivisionTree(CellIdSet<DIMS> ids):ids(ids), cost(0) {
        assert(ids.size() == 1);
    }
public:
    void setCost(result_t c) {
        cost = c;
    }
    const result_t& getCost() const {
        return cost;
    }
    const CellIdSet<DIMS>& getIds() const {
        return ids;
    }
    bool hasChildren() const {
        return ch[0] != nullptr;
    }
    const DivisionTree& getChild(size_t id) const {
        return *ch[id];
    }
    static ptr build(CellIdSet<DIMS> ids, const ptr& a, const ptr& b) {
        return ptr(new DivisionTree(ids, a, b));
    }
    
    static ptr build(CellIdSet<DIMS> ids) {
        return ptr(new DivisionTree(ids));
    }
    void printToStream(std::ostream& os, int depth = 0) const {
        FOR(i, depth) os << "  ";
        if(ids.size() > 1) {
            os << '(' << ids.size() << ')' << " = " << cost << std::endl;
        } else {
            os << '(' << *ids.begin() << ')' << " = " << cost << std::endl;
        }
        if(ch[0] != nullptr) ch[0]->printToStream(os, depth+1);
        if(ch[1] != nullptr) ch[1]->printToStream(os, depth+1);
    }
};

template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const DivisionTree<DIMS>& tree) {
    tree.printToStream(os);
    return os;
}



#endif /* defined(__HP3d__DivisionTree__) */
