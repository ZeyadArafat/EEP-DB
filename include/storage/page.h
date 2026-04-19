#pragma once

#include <cstring>
#include "../config.h"

class Page {
public:
    Page() = default;
    char* GetData() noexcept { return data_; }
    const char* GetData() const noexcept { return data_; }
    page_id_t GetPageId() const { return page_id_; }
    uint32_t GetPinCount() const { return pin_count_; }
    bool IsDirty() const { return is_dirty_; }
    void ResetMemory() {
        std::memset(data_, 0, PAGE_SIZE);
    }

private:
    char data_[PAGE_SIZE] {};
    page_id_t page_id_ = INVALID_PAGE_ID;
    pin_count_t pin_count_ = 0;
    bool is_dirty_ = false;

    friend class BufferPoolManager;
};