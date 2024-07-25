/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "ExpectationError.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

Step::ExpectationError::ExpectationError(std::string_view fname,
                                   std::string_view line, 
                                   std::string expectation,
                                   std::size_t line_no, 
                                   std::size_t col)
    : _fname{fname},
      _line{line}, 
      _expectation(expectation),
      _line_no{line_no}, 
      _col{col}
{
}

std::string Step::ExpectationError::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << _fname << " " << _line_no << ":" << _col << ": "
        << IError::make_red("error: ")
        << _expectation << '\n'
        << std::setw(std::log10(_line_no) + 4) << "     |\n"
        << std::setw(std::log10(_line_no) + 4) << _line_no << " |" << _line
        << std::setw(std::log10(_line_no) + 4) << "     |"
        << std::string(_col, ' ') << IError::make_red("^")
        << '\n';
    return out.str();
}




