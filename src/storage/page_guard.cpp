#include "../../include/storage/page_guard.h"
#include "../../include/storage/buffer_pool_manager.h"


PageGuard::PageGuard(BufferPoolManager *bpm, Page *page, bool is_dirty_on_drop) noexcept
    : bpm_(bpm), page_(page), is_dirty_on_drop_(is_dirty_on_drop){}

void PageGuard::stealFrom(PageGuard &other) noexcept {
    bpm_ = other.bpm_; page_ = other.page_;
    is_dirty_on_drop_ = other.is_dirty_on_drop_;
    other.bpm_ = nullptr;
    other.page_ = nullptr;
    other.is_dirty_on_drop_ = false;
}

PageGuard::PageGuard(PageGuard &&other) noexcept {
    stealFrom(other);
}

PageGuard& PageGuard::operator=(PageGuard &&other) noexcept {
    if (this == &other) return *this;
    Drop();
    stealFrom(other);
    return *this;
}

PageGuard::~PageGuard() noexcept {
    Drop();
}

bool PageGuard::IsDirtyOnDrop() const noexcept {
    return is_dirty_on_drop_;
}

void PageGuard::Drop() noexcept {
    if (!IsValid()) return; 
    (void)bpm_->UnpinInternal(page_->GetPageId(), IsDirtyOnDrop());
    bpm_ = nullptr; page_ = nullptr;
}

bool PageGuard::IsValid() const noexcept {
    return bpm_ != nullptr && page_ != nullptr;
}

page_id_t PageGuard::GetPageId() const noexcept {
    return IsValid()? page_->GetPageId() : INVALID_PAGE_ID;
}

/* READ PAGE GUARD */
ReadPageGuard::ReadPageGuard(BufferPoolManager *bpm, Page *page) noexcept
    : PageGuard(bpm, page, false) {}


const char* ReadPageGuard::GetData() const noexcept {
    return IsValid()? page_->GetData() : nullptr;
}


/* WRITE PAGE GUARD */
WritePageGuard::WritePageGuard(BufferPoolManager *bpm, Page *page) noexcept
    : PageGuard(bpm, page, true) {}

char* WritePageGuard::GetData() noexcept {
    return IsValid()? page_->GetData() : nullptr;
}

const char* WritePageGuard::GetData() const noexcept {
    return IsValid()? page_->GetData() : nullptr;
}