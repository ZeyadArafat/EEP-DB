#pragma once

#include "../config.h"

class BufferPoolManager;
class Page;

/*
 * PageGuard
 * Shared RAII ownership for pinned pages.
 * Non-copyable, movable.
 * */
class PageGuard {
public:
    PageGuard() = default;
    PageGuard(BufferPoolManager* bpm, Page* page, bool is_dirty_on_drop) noexcept;

    ~PageGuard() noexcept;

    // non-copyable
    PageGuard(const PageGuard&) = delete;
    PageGuard& operator=(const PageGuard&) = delete;

    // movable
    PageGuard(PageGuard&& other) noexcept;
    PageGuard& operator=(PageGuard&& other) noexcept;

    /* tell if this guard currently own a real page */
    [[nodiscard]] bool IsValid() const noexcept;
    [[nodiscard]] page_id_t GetPageId() const noexcept;

private:
    void stealFrom(PageGuard& other) noexcept;
protected:
    [[nodiscard]] bool IsDirtyOnDrop() const noexcept;
    void Drop() noexcept; /* use when that instance of guard no longer owns the page **/

protected:
    BufferPoolManager* bpm_ = nullptr;
    Page* page_ = nullptr;
private:
    bool is_dirty_on_drop_ = false;
};


/*
 * ReadPageGuard
 * Read-only access. */
class ReadPageGuard final : public PageGuard {
public:
    ReadPageGuard() = default;
    ReadPageGuard(BufferPoolManager* bpm, Page* page) noexcept;

    [[nodiscard]] const char* GetData() const noexcept;
};


/*
 * WritePageGuard
 * Writable access.
 * Automatically marks dirty on destruction. */
class WritePageGuard final : public PageGuard {
public:
    WritePageGuard() = default;
    WritePageGuard(BufferPoolManager* bpm, Page* page) noexcept;

    [[nodiscard]] char* GetData() noexcept;
    [[nodiscard]] const char* GetData() const noexcept;
};