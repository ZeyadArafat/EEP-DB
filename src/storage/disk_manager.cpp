#include "../../include/storage/disk_manager.h"

DiskManager::DiskManager(const std::string &file_name) {
    db_file_.open(file_name, std::ios::in | std::ios::out | std::ios::binary);

    if (!db_file_.is_open()) {
        // if file doesn't exist -> create it
        db_file_.clear();
        db_file_.open(file_name, std::ios::out | std::ios::binary);
        db_file_.close();
        // reopen in read/write mode
        db_file_.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }
    db_file_.seekg(0, std::ios::end);

    auto file_size = db_file_.tellg();
    if (file_size < 0) {
        file_size = 0; // fallback
        db_file_.clear();
    }
    next_page_id_ = static_cast<page_id_t>(file_size / PAGE_SIZE);
}

DiskManager::~DiskManager() {
    if (db_file_.is_open())
        db_file_.close();
}

void DiskManager::read_page(const page_id_t page_id, char* buffer) {
    const offset_t offset = static_cast<offset_t>(page_id) * PAGE_SIZE;
    db_file_.clear();
    db_file_.seekg(offset, std::ios::beg);

    if (!db_file_.read(buffer, PAGE_SIZE)) {
        // if reading fails (page not written yet)
        std::memset(buffer, 0, PAGE_SIZE);
        db_file_.clear();
    }
}

void DiskManager::write_page(const page_id_t page_id, const char* buffer) {
    const offset_t offset = static_cast<offset_t>(page_id) * PAGE_SIZE;
    db_file_.clear();
    db_file_.seekp(offset, std::ios::beg);
    db_file_.write(buffer, PAGE_SIZE);

    if (!db_file_) {
        db_file_.clear();
    }
    // TODO: Optimize later
    db_file_.flush();
}

page_id_t DiskManager::allocate_page() {
    if (!free_pages_.empty()) {
        const page_id_t page_id = free_pages_.back();
        free_pages_.pop_back();
        return page_id;
    }

    return next_page_id_++;
}

void DiskManager::deallocate_page(const page_id_t page_id) {
    free_pages_.push_back(page_id);
}