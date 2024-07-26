/*
 * created by: shunnobir
 * date: 26/07/2024
 */

#include "LexicalError.hpp"
#include "IError.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

Step::LexicalError::LexicalError(Step::IError::ErrorCode error_code,
                                std::string_view fname,
                                std::string_view line, 
                                // std::string expectation,
                                std::size_t line_no, 
                                std::size_t col)
    : _error_code{error_code},
      _fname{fname},
      _line{line}, 
      // _expectation(expectation),
      _line_no{line_no}, 
      _col{col}
{
}

std::string Step::LexicalError::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_bold(_fname.data()) << IError::make_bold(":" + std::to_string(_line_no) + ":" + std::to_string(_col) + ": ")
        << IError::make_red("error: ")
        << IError::_error_table[_error_code]
        // << "; " << _expectation 
        << " [E" << std::setw(3) << std::setfill('0') << static_cast<int>(_error_code)+1 << "]\n" << std::setfill(' ')
        << std::setw(std::log10(_line_no) + 4) << "     | \n"
        << std::setw(std::log10(_line_no) + 4) << _line_no << " | " << _line
        << std::setw(std::log10(_line_no) + 4) << "     | "
        << std::string(_col, ' ') << IError::make_red("^")
        << '\n';
    return out.str();
}
