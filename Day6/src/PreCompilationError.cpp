/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "PreCompilationError.hpp"
#include "IError.hpp"
#include <sstream>
#include <string>

Step::PreCompilationError::PreCompilationError(Step::IError::ErrorCode code, std::string extra)
    : _error{IError::_error_table[code] + 
            (extra.length() > 0 
             ? ": " + IError::make_bold(IError::make_purple(extra)) 
             : "")}
{
}

std::string Step::PreCompilationError::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_red("error: ")
        << _error << '\n'
        << "Run 'step --help' for more information.\n"
        << "compilation terminated!\n";
    return out.str();
}

