/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __ERRORMANAGER_HPP__
#define __ERRORMANAGER_HPP__

#include "IError.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Step {
    class ErrorManager {
        public:
            ErrorManager &add(std::unique_ptr<IError> &&error, bool is_warning = false);
            static ErrorManager &instance() {
                static ErrorManager emanager;
                return emanager;
            }

            static void dump(bool exit = false, int exit_code = 0);
        private:
            /* Default and Deleted Special methods */
            ErrorManager() = default;
            ErrorManager(ErrorManager const &) = delete;
            ErrorManager(ErrorManager &&) = delete;
            ErrorManager &operator=(ErrorManager const &) = delete;
            ErrorManager &operator=(ErrorManager &&) = delete;
        private:
            static std::vector<std::string> _errors;
            static std::vector<std::string> _warnings;
            static std::string _order;
    };
}

#endif // __ERRORMANAGER_HPP__
