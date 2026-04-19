#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include "page.h"
#include "disk_manager.h"
#include "../config.h"

class BufferPoolManager {
public:
    [[nodiscard]] Page* FetchPage(page_id_t page_id);
    [[nodiscard]] bool UnpinPage(page_id_t page_id, bool is_dirty);
    [[nodiscard]] Page* NewPage(page_id_t& page_id);
    [[nodiscard]] bool DeletePage(page_id_t page_id);
    [[nodiscard]] bool FlushPage(page_id_t page_id);
    void FlushAllPages();
    explicit BufferPoolManager(size_t pool_size, DiskManager* disk_manager);

private:
    pool_size_t pool_size_;
    DiskManager* disk_manager_;
    std::vector<Page> pages_;
    std::unordered_map<page_id_t, frame_id_t> page_table_;
    std::list<frame_id_t> free_list_;
};