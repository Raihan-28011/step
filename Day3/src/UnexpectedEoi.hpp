/*
 * created by: shunnobir
 * date: 25/07/2024
 */

#ifndef __UNEXPECTEDEOI_HPP__
#define __UNEXPECTEDEOI_HPP__

#include "IError.hpp"

namespace Step {
    class UnexpectedEoi: public IError {
    public:
        UnexpectedEoi(std::string_view fname,
                          std::string_view line, 
                          std::string expectation,
                          std::size_t line_no, 
                          std::size_t col);
        virtual ~UnexpectedEoi() override = default;
        virtual std::string to_string() const override;
    private:
        std::string_view _fname;
        std::string_view _line;
        std::string _expectation;
        std::size_t _line_no;
        std::size_t _col;
    };
}



#endif // __UNEXPECTEDEOI_HPP__
