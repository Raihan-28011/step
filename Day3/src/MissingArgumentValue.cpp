/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "MissingArgumentValue.hpp"
#include <sstream>
#include <string>

Step::MissingArgumentValue::MissingArgumentValue(std::string option)
    : _option{option}
{
}

std::string Step::MissingArgumentValue::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_red("error: ")
        << "missing value after argument '"
        << _option << "'\n"
        << "compilation terminated!\n";
    return out.str();
}
