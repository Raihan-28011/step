/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "NoInputFile.hpp"
#include <sstream>

std::string Step::NoInputFile::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_red("error: ")
        << "no input files provided\n"
        << "compilation terminated!\n";
    return out.str();
}
