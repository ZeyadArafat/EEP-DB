#pragma once

#include "../config.h"

class Page {
public:
    char data[PAGE_SIZE];
    page_id_t page_id;
    int pin_count = 0;
    bool is_dirty = false;
};