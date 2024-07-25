/*
 * created by: shunnobir
 * date: 25/07/2024
 */

#ifndef __NUMBERTOOLARGE_HPP__
#define __NUMBERTOOLARGE_HPP__

#include "IError.hpp"

namespace Step {
    class NumberTooLarge: public IError {
    public:
        NumberTooLarge(std::string_view fname,
                          std::string_view line, 
                          std::size_t line_no, 
                          std::size_t col);
        virtual ~NumberTooLarge() override = default;
        virtual std::string to_string() const override;
    private:
        std::string_view _fname;
        std::string_view _line;
        std::size_t _line_no;
        std::size_t _col;
    };
}

#endif // __NUMBERTOOLARGE_HPP__
