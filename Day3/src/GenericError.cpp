/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "GenericError.hpp"
#include <sstream>
#include <string>

Step::GenericError::GenericError(std::string error)
    : _error{error}
{
}

std::string Step::GenericError::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_red("error: ")
        << _error << (_error.back() != '\n' ? '\n' : '\0');
    return out.str();
}


