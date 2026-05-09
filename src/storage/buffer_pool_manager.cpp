#include "../../include/storage/buffer_pool_manager.h"
#include "../../include/storage/page_guard.h"

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager* disk_manager) :
    pool_size_(pool_size), disk_manager_(disk_manager) {}

ReadPageGuard BufferPoolManager::FetchPageRead(page_id_t page_id) {
    auto it = page_table_.find(page_id);
    Page* page = nullptr;
    if (page_table_.end() != it) {
        const frame_id_t frame_id = it->second;
        page = pages_[frame_id];
        page->pin_count_++;
    } else {
        page = FetchPageInternal(page_id);
        if (!page) {return ReadPageGuard();}
    }
    return ReadPageGuard(this, page);
}
