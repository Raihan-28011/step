/*
 * created by: shunnobir
 * date: 24/07/2024
 */


#include "IError.hpp"
#include <limits>
#include <string>
#include <unordered_map>

std::string const Step::IError::BOLD    = 
#if defined (__linux__) || defined (MAC)
                "\x1b[1m"
#else
                ""
#endif
                ;
std::string const Step::IError::RED     = 
#if defined (__linux__) || defined (MAC)
                "\x1b[31m"
#else
                ""
#endif
                ;
std::string const Step::IError::GREEN   = 
#if defined (__linux__) || defined (MAC)
                "\x1b[32m"
#else
                ""
#endif
                ;
std::string const Step::IError::YELLOW  = 
#if defined (__linux__) || defined (MAC)
                "\x1b[33m"
#else
                ""
#endif
                ;
std::string const Step::IError::BLUE    = 
#if defined (__linux__) || defined (MAC)
                "\x1b[34m"
#else
                ""
#endif
                ;
std::string const Step::IError::PURPLE  = 
#if defined (__linux__) || defined (MAC)
                "\x1b[35m"
#else
                ""
#endif
                ;
std::string const Step::IError::TEAL    = 
#if defined (__linux__) || defined (MAC)
                "\x1b[36m"
#else
                ""
#endif
                ;
std::string const Step::IError::NORMAL  = 
#if defined (__linux__) || defined (MAC)
                "\x1b[m"
#else
                ""
#endif
                ;

std::unordered_map<Step::IError::ErrorCode, std::string> Step::IError::_error_table = {
    {Step::IError::ErrorCode::E001, "No input file(s) provided"},
    {Step::IError::ErrorCode::E002, "Unrecognized command-line option"},
    {Step::IError::ErrorCode::E003, "Missing value after argument"},
    {Step::IError::ErrorCode::E004, "Not a valid step file format"},
    {Step::IError::ErrorCode::E005, "File not found. No such file or directory"},
    {Step::IError::ErrorCode::E006, "File could not be opened (Probable cause: No read permission)"},
    {Step::IError::ErrorCode::E007, "unrecognized sequence of character(s)"},
    {Step::IError::ErrorCode::E008, "expected digit after radix point"},
    {Step::IError::ErrorCode::E009, "expected digit after radix point; radix point must be followed by a digit"},
    {Step::IError::ErrorCode::E010, "exponent has no digit"},
    {Step::IError::ErrorCode::E011, "expected digit after exponent"},
    {Step::IError::ErrorCode::E012, "integer too large; maximum supported integer is " + std::to_string(std::numeric_limits<long long>::max())},
    {Step::IError::ErrorCode::E013, "float too large; maximum supported float is " + std::to_string(std::numeric_limits<double>::max())},
    {Step::IError::ErrorCode::E014, "integer too small; minimum supported integer is " + std::to_string(std::numeric_limits<long long>::min())},
    {Step::IError::ErrorCode::E015, "float too large; minimum supported float is " + std::to_string(std::numeric_limits<long long>::min())},
    {Step::IError::ErrorCode::E016, "misplaced number separator"},
};
