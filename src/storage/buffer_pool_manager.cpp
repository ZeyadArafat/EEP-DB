#include "../../include/storage/buffer_pool_manager.h"
#include "../../include/storage/page_guard.h"

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager* disk_manager) :
    pool_size_(pool_size), disk_manager_(disk_manager) {
    pages_.resize(pool_size);
    for (frame_id_t i = 0; i < pool_size; i++) {
        pages_[i] = Page();
        free_list_.push_back(i);
    }
}

frame_id_t BufferPoolManager::search_page(const page_id_t page_id) {
    if (const auto it = page_table_.find(page_id); it != page_table_.end()) {
        const frame_id_t frame_id = it->second;
        return frame_id;
    }
    return INVALID_FRAME_ID;
}

ReadPageGuard BufferPoolManager::FetchPageRead(page_id_t page_id) {
    Page* page = FetchPageInternal(page_id);
    if (!page)
        return ReadPageGuard();
    return ReadPageGuard(this, page);
}

WritePageGuard BufferPoolManager::FetchPageWrite(page_id_t page_id) {
    Page* page = FetchPageInternal(page_id);
    if (!page)
        return WritePageGuard();
    return WritePageGuard(this, page);
}

bool BufferPoolManager::UnpinInternal(page_id_t page_id, bool is_dirty) {
    const frame_id_t frame_id = search_page(page_id);
    if (frame_id == INVALID_FRAME_ID)
        return false;
    Page page = pages_[frame_id];
    if (page.pin_count_ <= 0)
        return false;
    page.is_dirty_ |= is_dirty;
    page.pin_count_--;
    return true;
}

Page* BufferPoolManager::FetchPageInternal(page_id_t page_id) {

}