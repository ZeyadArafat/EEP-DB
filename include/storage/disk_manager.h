#pragma once

#include <fstream>
#include <cstring>
#include <iosfwd>
#include <vector>

#include "../config.h"

class DiskManager {
public:
    void read_page(int page_id, char* buffer);
    void write_page(int page_id, const char* buffer);
    int allocate_page();
    void deallocate_page(int page_id);

    DiskManager(const std::string& file_name);

    ~DiskManager();

private:
    std::fstream db_file;
    int next_page_id = 0;
    std::vector<int> free_pages;
};
