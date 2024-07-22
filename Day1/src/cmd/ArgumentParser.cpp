/*
 * created by: shunnobir
 * date: 22/07/2024
 */

#include "ArgumentParser.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <vector>

Step::ArgumentParser::ArgumentParser(int argc, char *argv[]) {
    convert_to_std_string(argc, argv);
    add_rule("-h", "Show this information.", false, "", "--help");
}

void Step::ArgumentParser::convert_to_std_string(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        _argv.emplace_back(argv[i]);
    } 
}

std::string const &Step::ArgumentParser::get(std::string option) {
    return _options[option];
}

Step::ArgumentParser &Step::ArgumentParser::add_rule(std::string option,
                                                     std::string description,
                                                     bool        has_value,
                                                     std::string value_name,
                                                     std::string long_option) {
    if (option.empty() && long_option.empty()) {
        std::cout << "Warning: trying to add rule without an option name is is not-allowed (rule-not-added)\n";
        return *this;
    }

    if (description.empty()) {
        std::cout << "Warning: empty description is not-allowed (rule-not-added)\n";
        return *this;
    }

    if (option.size() < 2 || (option.size() >= 2 && (option == "--" || option[0] != '-'))) {
        // Warning: invalid option name. must be of form '-o' or '-op' or '-opt' or '-opt1' (rule-not-added)
        std::cout << "Warning: invalid option name. must be of form '-o' or '-op' or '-opt' or '-opt1' (rule-not-added)\n";
        return *this;
    }

    if (long_option.size() < 3 || 
       (long_option.size() >= 3 && 
       (long_option == "---" || 
        long_option[0] != '-' ||
        long_option.substr(0, 2) != "--"))) {
        // Warning: invalid long option name. must be of form '--o' or '--op' or '--opt1' (rule-not-added)
        std::cout << "Warning: invalid long option name. must be of form '--o' or '--op' or '--opt1' (rule-not-added)\n";
        return *this;
    }

    _rules.push_back(ArgumentRule{option, description, long_option, value_name, has_value});
    _options[value_name.empty() ? (option.empty() ? long_option : option) : value_name] = has_value ? "" : "false";
    return *this;
}

bool Step::ArgumentParser::parse() {
    if (_argv.size() < 1) {
        // Error: No input files provided
        std::cout << "Error: No input files provided\n";
        // TODO: Print usage/help -> Done
        help();
        return false;
    }

    std::vector<std::string>::size_type index = 0;
    while (index < _argv.size()) {
        auto arg = _argv.at(index);
        auto rule = std::find_if(_rules.begin(), _rules.end(), [&arg](ArgumentRule const &a) {
            return a._option == arg || a._long_option == arg;
        });


        if (rule == _rules.end()) {
            // Error: unrecognized command-line option '_argv.at(index)'
            std::cout << "Error: unrecognized command-line option '" << _argv.at(index) << "'\n";
            // TODO: Print usage/help -> Done
            help();
            return false;
        }

        auto const &dref_rule = *rule;
        if (dref_rule._option == "-h") {
            help();
            std::exit(0);
            return true;
        }

        std::string value;
        if (dref_rule._has_value) {
            ++index;
            if (index >= _argv.size()) {
                // Error: missing value for argument '_argv.at(index-1)'
                std::cout << "Error: missing value for argument '" << _argv.at(index-1) << "'\n";
                // TODO: Print usage/help -> done
                help();
                return false;
            }

           value = _argv.at(index);
        } else {
            value = "true";
        }

        if (!dref_rule._value_name.empty()) {
            arg = dref_rule._value_name;
        } else if (!dref_rule._long_option.empty() && dref_rule._long_option == arg) {
            arg = dref_rule._option;
        }
        _options[arg] = value;
        ++index;
    }
    return true;
}

void Step::ArgumentParser::print() const {
    std::cout << "{\n";
    for (auto const &p: _options) {
        std::cout << '\t' << p.first << ": " << p.second << '\n';
    }
    std::cout << "}\n";
}

void Step::ArgumentParser::help() const {
    std::ostringstream out;
    out << "Usage: step [options]\n"
              << "Options:\n";
    for (auto const &rule: _rules) {
        std::string value_name = "";
        if (!rule._value_name.empty()) {
            value_name = rule._value_name;
        } else if (rule._has_value && !rule._long_option.empty()) {
            value_name = rule._long_option.substr(2);
        } else if (rule._has_value && !rule._option.empty()) {
            value_name = rule._option;
        }

        std::transform(value_name.begin(), value_name.end(), value_name.begin(), [](char c) { return std::toupper(c); });
        
        std::ostringstream temp;
        temp << "   " << rule._option
             << (value_name.empty() ? "" : " <" + value_name + ">") 
             << (rule._long_option.empty() ? "" : ", " + rule._long_option)
             << (value_name.empty() ? "" : " <" + value_name + ">");

        out << std::setw(24) << std::left << temp.str();
        out << (temp.str().length() >= 24 ? "\n" : "");
        out << (temp.str().length() >= 24 ? std::setw(25) : std::setw(1)) << " "
            << rule._description
            << '\n';
    }
    out << "\nFor bug report, please visit:\n"
        << "<https://github.com/shunnobir/step>\n";
    std::cout << out.str();
}
