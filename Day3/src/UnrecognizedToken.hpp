/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __UNRECOGNIZEDTOKEN_HPP__
#define __UNRECOGNIZEDTOKEN_HPP__

#include "IError.hpp"
#include <string>
#include <string_view>

namespace Step {
    class UnrecognizedToken: public IError {
    public:
        UnrecognizedToken(std::string_view fname,
                          std::string_view line, 
                          std::size_t line_no, 
                          std::size_t col);
        virtual ~UnrecognizedToken() override = default;
        virtual std::string to_string() const override;
    private:
        std::string_view _fname;
        std::string_view _line;
        std::size_t _line_no;
        std::size_t _col;
    };
}

#endif // __UNRECOGNIZEDTOKEN_HPP__

