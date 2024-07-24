/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "NotValidFile.hpp"
#include <sstream>
#include <string>

Step::NotValidFile::NotValidFile(std::string fname)
    : _fname{fname}
{
}

std::string Step::NotValidFile::to_string() const {
    std::ostringstream out;
    out << IError::make_bold("step: ")
        << IError::make_red("error: ")
        << "'" << _fname 
        << "' is not a valid step file\n"
        << "step recognizes input files with .step extension and in step programming language format.\n"
        << "To learn more about step programming language, please visit:\n"
        << "<https://github.com/shunnobir/step>\n"
        << "compilation terminated!\n";
    return out.str();
}

