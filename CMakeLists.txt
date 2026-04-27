cmake_minimum_required(VERSION 4.1)
project(EEPDB)

set(CMAKE_CXX_STANDARD 23)

add_executable(EEPDB main.cpp
        include/execution/parser.h
        include/storage/disk_manager.h
        src/execution/parser.cpp
        src/storage/disk_manager.cpp
        src/table/table_heap.cpp
        include/storage/page.h
        include/storage/buffer_pool_manager.h
        include/config.h
        include/index/BpNode.h
        include/index/BpTree.h
        src/index/BpTree.cpp
        src/index/BpNode.cpp
        include/storage/page_guard.h
        src/storage/page_guard.cpp
        src/storage/buffer_pool_manager.cpp)
