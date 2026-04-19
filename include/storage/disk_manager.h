#pragma once

#include <fstream>
#include <cstring>
#include <iosfwd>
#include <vector>

#include "../config.h"

class DiskManager {
public:
    void ReadPage(page_id_t page_id, char* buffer);
    void WritePage(page_id_t page_id, const char* buffer);
    [[nodiscard]] page_id_t AllocatePage();
    void DeallocatePage(page_id_t page_id);

    explicit DiskManager(const std::string& file_name);
    ~DiskManager() noexcept;
    // disable copying file streams
    DiskManager(const DiskManager&) = delete;
    DiskManager& operator=(const DiskManager&) = delete;

private:
    std::fstream db_file_;
    page_id_t next_page_id_;
    std::vector<page_id_t> free_pages_; // TODO: CAUTION!! free_pages_ list is memory ONLY should fix this later
};
