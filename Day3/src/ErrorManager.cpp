/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#include "ErrorManager.hpp"
#include "IError.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

std::vector<std::string> Step::ErrorManager::_errors;
std::vector<std::string> Step::ErrorManager::_warnings;
std::string Step::ErrorManager::_order = "";

Step::ErrorManager &Step::ErrorManager::add(std::unique_ptr<IError> &&error, bool is_warning) {
    if (is_warning) {
        _warnings.emplace_back(error->to_string());
        _order.append("w");
    } else {
        _errors.emplace_back(error->to_string());
        _order.append("e");
    }
    return *this;
}

void Step::ErrorManager::dump(bool exit, int exit_code) {
    std::ostringstream out;
    for (std::size_t i = 0, j = 0, k = 0; i < _order.length(); ++i) {
        if (_order.at(i) == 'w') {
            out << _warnings.at(j++);
        } else {
            out << _errors.at(k++);
        }
    }
    std::cerr << out.str();
    std::cerr << _warnings.size() << " warning" << (_warnings.size() > 1 ? "s" : "")
              << " and " << _errors.size() << " error" << (_errors.size() > 1 ? "s generated\n" : " generated\n");
    _errors.clear();
    if (exit) std::exit(exit_code);
}
