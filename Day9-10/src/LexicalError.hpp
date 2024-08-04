/*
 * created by: shunnobir
 * date: 26/07/2024
 */

#ifndef __LEXICAL_ERROR_HPP__
#define __LEXICAL_ERROR_HPP__

#include "IError.hpp"

namespace Step {
    class LexicalError: public IError {
    public:
        LexicalError(IError::ErrorCode error_code,
                    std::string_view fname,
                    std::string_view line,
                    // std::string expectation,
                    std::size_t line_no,
                    std::size_t col);
        virtual ~LexicalError() override = default;
        virtual std::string to_string() const override;

    private:
        Step::IError::ErrorCode _error_code;
        std::string_view _fname;
        std::string_view _line;
        // std::string _expectation;
        std::size_t _line_no;
        std::size_t _col;
    };
}

#endif // __LEXICAL_ERROR_HPP__
