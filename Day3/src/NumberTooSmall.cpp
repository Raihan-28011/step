/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "NumberTooSmall.hpp"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>

Step::NumberTooSmall::NumberTooSmall(std::string_view fname,
                                   std::string_view line, 
                                   std::size_t line_no, 
                                   std::size_t col)
    : _fname{fname},
      _line{line}, 
      _line_no{line_no}, 
      _col{col}
{
}

std::string Step::NumberTooSmall::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << _fname << " " << _line_no << ":" << _col << ": "
        << IError::make_red("error: ")
        << "number too small; minimum supported integer is " 
        << std::numeric_limits<long long>::min() << " and minimum supported float is "
        << std::setprecision(17) << std::numeric_limits<double>::min() << "\n"
        << std::setw(std::log10(_line_no) + 4) << "     |\n"
        << std::setw(std::log10(_line_no) + 4) << _line_no << " |" << _line
        << std::setw(std::log10(_line_no) + 4) << "     |"
        << std::string(_col, ' ') << IError::make_red("^")
        << '\n';
    return out.str();
}



