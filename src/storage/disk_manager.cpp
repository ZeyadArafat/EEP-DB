#include "../../include/storage/disk_manager.h"

DiskManager::DiskManager(const std::string &file_name) {
    db_file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);

    if (!db_file.is_open()) {
        // if file doesn't exist -> create it
        db_file.clear();
        db_file.open(file_name, std::ios::out | std::ios::binary);
        db_file.close();
        // reopen in read/write mode
        db_file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }
    db_file.seekg(0, std::ios::end);
    const int file_size = db_file.tellg();

    next_page_id = file_size / PAGE_SIZE;
}

DiskManager::~DiskManager() {
    if (db_file.is_open()) {
        db_file.close();
    }
}

void DiskManager::read_page(int page_id, char* buffer) {
    const int offset = page_id * PAGE_SIZE;
    db_file.seekg(offset);

    if (!db_file.read(buffer, PAGE_SIZE)) {
        // if reading fails (page not written yet)
        std::memset(buffer, 0, PAGE_SIZE);
        db_file.clear();
    }
}

void DiskManager::write_page(int page_id, const char* buffer) {
    const int offset = page_id * PAGE_SIZE;

    db_file.seekp(offset);
    db_file.write(buffer, PAGE_SIZE);
    db_file.flush();
}

int DiskManager::allocate_page() {
    if (!free_pages.empty()) {
        const int page_id = free_pages.back();
        free_pages.pop_back();
        return page_id;
    }

    return next_page_id++;
}

void DiskManager::deallocate_page(int page_id) {
    free_pages.push_back(page_id);
}