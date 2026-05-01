#pragma once

// TODO: fix : Remove PageManager and use buffer pool manager instead (storage API)
#include <iostream>
#include <ostream>
#include <cstring>

#include "BpNode.h"
#include "PageManager.h"

class BpTree {
private:
    PageManager* pm;
    int root_page_id;
public:
    BpTree(PageManager* manager);
    int get_root_id();
    int find_leaf(int key);
    RID search(int key);
};
