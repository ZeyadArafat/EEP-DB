#pragma once

#include <cstdint>

constexpr std::size_t PAGE_SIZE = 4096;

using page_id_t = int32_t;
using frame_id_t = int32_t;
using offset_t = std::size_t;
using pin_count_t = uint32_t;
using pool_size_t = std::size_t;

constexpr page_id_t INVALID_PAGE_ID = -1;
constexpr frame_id_t INVALID_FRAME_ID = -1;
