/*
 * created by: shunnobir
 * date: 04/08/2024
 */

#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

#include <cstdint>
#include <string>
#include <variant>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::uint32_t;
using i64 = std::int64_t;
using sz = std::size_t;
using f32 = float;
using f64 = double;

using literal_type = std::variant<std::string, i64, f64>;

namespace Step {

template <typename To, typename From>
To as(From obj) {
    return static_cast<To>(obj);
}

} // namespace Step

#endif // __DEFINE_HPP__
