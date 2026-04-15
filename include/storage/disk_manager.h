#pragma once

#include <fstream>
#include <cstring>
#include <iosfwd>
#include <vector>

#include "../config.h"

class DiskManager {
public:
    void read_page(page_id_t page_id, char* buffer);
    void write_page(page_id_t page_id, const char* buffer);
    page_id_t allocate_page();
    void deallocate_page(page_id_t page_id);

    explicit DiskManager(const std::string& file_name);
    ~DiskManager();

private:
    std::fstream db_file_;
    page_id_t next_page_id_;
    std::vector<page_id_t> free_pages_; // TODO: CAUTION!! free_pages_ list is memory ONLY should fix this later
};
