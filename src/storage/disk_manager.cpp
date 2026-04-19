#include "../../include/storage/disk_manager.h"

// Opens an existing database file or creates a new one if it does not exist.
// Also initializes the next page id based on the current file size.
DiskManager::DiskManager(const std::string &file_name) {
    db_file_.open(file_name, std::ios::in | std::ios::out | std::ios::binary);

    // If the file does not exist yet, create an empty one first.
    if (!db_file_.is_open()) {
        db_file_.clear();
        db_file_.open(file_name, std::ios::out | std::ios::binary);
        db_file_.close();
        db_file_.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }

    // Move to the end to determine the current file size.
    db_file_.seekg(0, std::ios::end);

    auto file_size = db_file_.tellg();
    if (file_size < 0) {
        // If the size cannot be determined, treat the file as empty.
        file_size = 0;
        db_file_.clear();
    }

    // Each page occupies PAGE_SIZE bytes, so the next page id is the
    // number of pages currently stored in the file.
    next_page_id_ = static_cast<page_id_t>(file_size / PAGE_SIZE);
}

DiskManager::~DiskManager() {
    if (db_file_.is_open())
        db_file_.close();
}

// Reads one page from disk into the provided buffer.
// If the page has not been written yet, the buffer is zero-filled.
void DiskManager::ReadPage(const page_id_t page_id, char* buffer) {
    const offset_t offset = static_cast<offset_t>(page_id) * PAGE_SIZE;

    // Clear any previous stream state before seeking.
    db_file_.clear();
    db_file_.seekg(offset, std::ios::beg);

    // Attempt to read exactly one page.
    if (!db_file_.read(buffer, PAGE_SIZE)) {
        // If the read fails, return an empty page instead of garbage data.
        std::memset(buffer, 0, PAGE_SIZE);
        db_file_.clear();
    }
}

// Writes one page from the provided buffer to disk at the given page id.
void DiskManager::WritePage(const page_id_t page_id, const char* buffer) {
    const offset_t offset = static_cast<offset_t>(page_id) * PAGE_SIZE;

    // Clear stream state before seeking for writing.
    db_file_.clear();
    db_file_.seekp(offset, std::ios::beg);

    // Write exactly one page.
    db_file_.write(buffer, PAGE_SIZE);

    if (!db_file_) {
        // Restore stream usability if the write triggered an error state.
        db_file_.clear();
    }

    // Flush immediately so data is persisted to disk.
    // TODO: This can be optimized later
    db_file_.flush();
}

// Returns a page id for a new page.
// Reuses a deallocated page if one is available; otherwise extends the file.
page_id_t DiskManager::AllocatePage() {
    if (!free_pages_.empty()) {
        const page_id_t page_id = free_pages_.back();
        free_pages_.pop_back();
        return page_id;
    }

    return next_page_id_++;
}

// Marks a page as free so it can be reused later.
void DiskManager::DeallocatePage(const page_id_t page_id) {
    free_pages_.push_back(page_id);
}