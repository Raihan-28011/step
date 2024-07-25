/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __MISPLACEDNUMSEPARATOR_HPP__
#define __MISPLACEDNUMSEPARATOR_HPP__

#include "IError.hpp"
#include <string>
#include <string_view>

namespace Step {
    class MisplacedNumSeparator: public IError {
    public:
        MisplacedNumSeparator(std::string_view fname,
                              std::string_view line, 
                              std::size_t line_no, 
                              std::size_t col);
        virtual ~MisplacedNumSeparator() override = default;
        virtual std::string to_string() const override;
    private:
        std::string_view _fname;
        std::string_view _line;
        std::size_t _line_no;
        std::size_t _col;
    };
}

#endif // __MISPLACEDNUMSEPARATOR_HPP__


