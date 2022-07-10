//
// Created by raihan on 7/8/22.
//

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <cctype>
#include <fstream>
#include <cstring>
#include <memory>

#define __DEBUG_STEP__

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using string = std::string;

template <typename T>
using vector = std::vector<T>;

template <typename T, typename U>
using umap = std::unordered_map<T, U>;

template <typename T, typename U>
using map = std::map<T, U>;

namespace step {
    template <typename TO>
    TO as(auto obj) {
        return static_cast<TO>(obj);
    }
}
