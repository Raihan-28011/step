/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "MisplacedNumSeparator.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

Step::MisplacedNumSeparator::MisplacedNumSeparator(
        std::string_view fname,
        std::string_view line, 
        std::size_t line_no, 
        std::size_t col)
    : _fname{fname},
      _line{line}, 
      _line_no{line_no}, 
      _col{col}
{
}

std::string Step::MisplacedNumSeparator::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << _fname << " " << _line_no << ":" << _col << ": "
        << IError::make_red("error: ")
        << "misplaced number separator\n"
        << std::setw(std::log10(_line_no) + 4) << "     |\n"
        << std::setw(std::log10(_line_no) + 4) << _line_no << " |" << _line
        << std::setw(std::log10(_line_no) + 4) << "     |"
        << std::string(_col, ' ') << IError::make_red("^")
        << IError::make_bold("Note: ") << "Following are the rules to follow for numver separators,\n"
        << "    1. Numbers can not start or finish with a separator,\n"
        << "    2. Numbers can not have a separator before or after the radix point,\n"
        << "    3. Numbers can not have a separator before the exponent designator,\n"
        << "    4. Numbers can not have a separator between the exponent designator and the +/- sign of the exponent designator,\n"
        << "    5. Except the above mentioned 5 places, you can use separator anywhere else, as many as you like\n"
        << '\n';
    return out.str();
}


