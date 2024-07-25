/*
 * created by: shunnobir
 * date: 25/07/2024
 */

#ifndef __EXPECTATIONERROR_HPP__
#define __EXPECTATIONERROR_HPP__

#include "IError.hpp"
namespace Step {
    class ExpectationError: public IError {
    public:
        ExpectationError(std::string_view fname,
                          std::string_view line, 
                          std::string expectation,
                          std::size_t line_no, 
                          std::size_t col);
        virtual ~ExpectationError() override = default;
        virtual std::string to_string() const override;
    private:
        std::string_view _fname;
        std::string_view _line;
        std::string _expectation;
        std::size_t _line_no;
        std::size_t _col;
    };
}

#endif // __EXPECTATIONERROR_HPP__
