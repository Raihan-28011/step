/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __IERROR_HPP__
#define __IERROR_HPP__

#include <string>
#include <unordered_map>

namespace Step {
    class IError {
        public:
            static std::string const BOLD  ;
            static std::string const RED   ;
            static std::string const GREEN ;
            static std::string const YELLOW;
            static std::string const BLUE  ;
            static std::string const PURPLE;
            static std::string const TEAL  ;
            static std::string const NORMAL;

            enum class ErrorCode {
                E001, // E001: No input file(s) provided
                E002, // E002: Unrecognized command-line option
                E003, // E003: Missing value after argument
                E004, // E004: Not a valid step file
                E005, // E005: File not found. No such file or directory
                E006, // E006: File could not be opened. (Probable cause: No read permission)
                E007, // E007: unrecognized sequence of character(s)
                E008, // E008: expected digit after radix point
                E009, // E009: expected digit after radix point: radix point must be followed by a digit
                E010, // E010: exponent has no digit
                E011, // E011: expected digit after exponent
                E012, // E012: integer too large; maximum supported integer is {std::numeric_limits<long long>::max()}
                E013, // E013: float too large; maximum supported float is {std::numeric_limits<double>::max()}
                E014, // E014: integer too small; minimum supported integer is {std::numeric_limits<long long>::min()}
                E015, // E015: float too small; minimum supported float is {std::numeric_limits<double>::min()}
                E016, // E016: misplaced number separator
            };
        public:
            virtual ~IError() = default;
            virtual std::string to_string() const = 0;

            static std::string make_bold(std::string s) {
                return BOLD + s + NORMAL;
            }

            static std::string make_red(std::string s) {
                return RED + s + NORMAL;
            }

            static std::string make_green(std::string s) {
                return GREEN + s + NORMAL;
            }

            static std::string make_yellow(std::string s) {
                return YELLOW + s + NORMAL;
            }

            static std::string make_blue(std::string s) {
                return BLUE + s + NORMAL;
            }

            static std::string make_purple(std::string s) {
                return PURPLE + s + NORMAL;
            }

            static std::string make_teal(std::string s) {
                return TEAL + s + NORMAL;
            }

        protected:
            static std::unordered_map<ErrorCode, std::string> _error_table;
    };
}

#endif // __IERROR_HPP__
