cmake_minimum_required(VERSION 4.1)
project(EEPDB)

set(CMAKE_CXX_STANDARD 23)

add_executable(EEPDB main.cpp
        include/execution/parser.h
        include/index/bp_tree.h
        include/storage/disk_manager.h
        src/execution/parser.cpp
        src/index/bp_tree.cpp
        src/storage/disk_manager.cpp
        src/table/table_heap.cpp
        include/storage/page.h
        include/storage/buffer_pool_manager.h
        include/config.h)
