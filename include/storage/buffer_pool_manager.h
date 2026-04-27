#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include "page.h"
#include "disk_manager.h"
#include "../config.h"
#include "page_guard.h"


class BufferPoolManager {
public:
    /*
     * Public API */
    [[nodiscard]] ReadPageGuard FetchPageRead(page_id_t page_id);
    [[nodiscard]] WritePageGuard FetchPageWrite(page_id_t page_id);
    [[nodiscard]] WritePageGuard NewPage();
    [[nodiscard]] bool DeletePage(page_id_t page_id);
    [[nodiscard]] bool FlushPage(page_id_t page_id);
    void FlushAllPages();

    [[nodiscard]] pool_size_t GetPoolSize() const noexcept;

    explicit BufferPoolManager(size_t pool_size, DiskManager* disk_manager);
    // non-copyable
    BufferPoolManager(const BufferPoolManager&) = delete;
    BufferPoolManager& operator=(const BufferPoolManager&) = delete;

private:
    friend class PageGuard;
    /*
     * Guard-only Internal API */
    [[nodiscard]] bool UnpinInternal(page_id_t page_id, bool is_dirty);
    [[nodiscard]] Page* FetchPageInternal(page_id_t page_id);
    [[nodiscard]] Page* AllocateNewPageInternal();
    [[nodiscard]] frame_id_t AcquireFrame();
    void EvictFrame(frame_id_t frame_id);

private:
    pool_size_t pool_size_;
    DiskManager* disk_manager_;
    // fixed-size frame array
    std::vector<Page> pages_;
    // page_id -> frame_id
    std::unordered_map<page_id_t, frame_id_t> page_table_;
    // never-used frames
    std::list<frame_id_t> free_list_;
    // TODO:
    // LRUReplacer replacer_;
};