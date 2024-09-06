/*
 * created by: shunnobir
 * date: 04/08/2024
 */

#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "../define.hpp"
#include <cassert>
#include <cmath>
#include <locale>
#include <string>
#include <type_traits>

namespace Step {

class Object {
    public:
        virtual std::string to_string() const = 0;
};

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
class Integer: public Object {
    template <typename F, typename>
    friend class Float;
    public:
        using value_type = T;
    public:
        Integer(value_type val)
            : _val{val} 
        {
        }

        Integer &operator=(value_type val) {
            _val = val;
            return *this;
        }

        Integer operator+(Integer const &rhs) {
            return Integer(_val + rhs._val);
        }

        Integer operator-(Integer const &rhs) {
            return Integer(_val - rhs._val);
        }

        Integer operator*(Integer const &rhs) {
            assert(sizeof(_val * rhs._val) == sizeof(value_type));
            return Integer(_val * rhs._val);
        }

        Integer operator/(Integer const &rhs) {
            assert(rhs._val != 0);
            return Integer(_val / rhs._val);
        }

        Integer operator%(Integer const &rhs) {
            assert(rhs._val != 0);
            return Integer(_val % rhs._val);
        }

        bool operator<(Integer const &rhs) {
            return _val < rhs._val;
        }

        bool operator>(Integer const &rhs) {
            return _val > rhs._val;
        }

        bool operator<=(Integer const &rhs) {
            return _val <= rhs._val;
        }

        bool operator>=(Integer const &rhs) {
            return _val >= rhs._val;
        }

        bool operator==(Integer const &rhs) {
            return _val == rhs._val;
        }

        bool operator!=(Integer const &rhs) {
            return _val != rhs._val;
        }

        value_type get() const {
            return _val;
        }

        virtual std::string to_string() const override {
            return std::to_string(_val);
        }


    private:
        value_type _val;
};

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
class Float: public Object {
    public:
        using value_type = T;
    public:
        Float(value_type val)
            : _val{val} 
        {
        }

        template <typename F>
        Float(Integer<F> const &val)
            : _val{value_type(val._val)}
        {
        }

        Float &operator=(value_type val) {
            _val = val;
            return *this;
        }

        template <typename F>
        Float &operator=(Integer<F> const &val) {
            _val = value_type(val._val);
        }

        Float operator+(Float const &rhs) {
            return Float(_val + rhs._val);
        }

        Float operator-(Float const &rhs) {
            return Float(_val - rhs._val);
        }

        Float operator*(Float const &rhs) {
            assert(sizeof(_val * rhs._val) == sizeof(value_type));
            return Float(_val * rhs._val);
        }

        Float operator/(Float const &rhs) {
            assert(rhs._val != 0);
            return Float(_val / rhs._val);
        }

        Float operator%(Float const &rhs) {
            assert(rhs._val != 0);
            return Float(std::fmod(_val, rhs._val));
        }

        bool operator<(Float const &rhs) {
            return _val < rhs._val;
        }

        bool operator>(Float const &rhs) {
            return _val > rhs._val;
        }

        bool operator<=(Float const &rhs) {
            return _val <= rhs._val;
        }

        bool operator>=(Float const &rhs) {
            return _val >= rhs._val;
        }

        bool operator==(Float const &rhs) {
            return _val == rhs._val;
        }

        bool operator!=(Float const &rhs) {
            return _val != rhs._val;
        }

        value_type get() const {
            return _val;
        }

        virtual std::string to_string() const override {
            return std::to_string(_val);
        }

    private:
        value_type _val;
};

class Boolean: public Object {
    public:
        using value_type = u8;
    public:
        Boolean(value_type val)
            : _val{val}
        {
        }

        // Boolean(bool val)
        //     : _val{val}
        // {
        // }

        operator bool() const {
            return bool(_val);
        }

        virtual std::string to_string() const override {
            return _val != 0 ? "true" : "false";
        }
    private:
        value_type _val;
};

} // namespace Step


#endif // __OBJECT_HPP__
