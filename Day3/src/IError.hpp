/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __IERROR_HPP__
#define __IERROR_HPP__

#include <string>

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
                return RED + s + NORMAL;
            }

            static std::string make_yellow(std::string s) {
                return RED + s + NORMAL;
            }

            static std::string make_blue(std::string s) {
                return RED + s + NORMAL;
            }

            static std::string make_purple(std::string s) {
                return RED + s + NORMAL;
            }

            static std::string make_teal(std::string s) {
                return RED + s + NORMAL;
            }

    };
}

#endif // __IERROR_HPP__
