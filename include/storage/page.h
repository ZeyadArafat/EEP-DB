#pragma once

#include "../config.h"

class Page {
public:
    char data[PAGE_SIZE];
    using page_id_t = int;
    int pin_count = 0;
    bool is_dirty = false;
};