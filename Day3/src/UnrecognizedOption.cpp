/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "UnrecognizedOption.hpp"
#include <sstream>
#include <string>

Step::UnrecognizedOption::UnrecognizedOption(std::string option)
    : _option{option}
{
}

std::string Step::UnrecognizedOption::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_red("error: ")
        << "unrecognized command-line option '"
        << _option << "'\n"
        << "compilation terminated!\n";
    return out.str();
}
