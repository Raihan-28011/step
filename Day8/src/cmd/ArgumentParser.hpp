/*
 * created by: shunnobir
 * date: 22/07/2024
 */

#ifndef __ARGUMENTPARSER_HPP__
#define __ARGUMENTPARSER_HPP__

#include <string>
#include <unordered_map>
#include <vector>

namespace Step {
    struct ArgumentRule {
        std::string _option;
        std::string _description;
        std::string _long_option; // Default: no long option
        std::string _value_name;  // Default: _long_option | _option
        bool        _has_value;   // Default: false
    };

    class ArgumentParser {
    public:
        ArgumentParser(int argc, char *argv[]);

        /* Public Methods */
        ArgumentParser &add_rule(std::string option,
                                 std::string description,
                                 bool        has_value = false,
                                 std::string value_name = "",
                                 std::string long_option = "");
        bool parse();
        std::string const &get(std::string option);
        void help() const;
    private:
        /* Private Methods */
        void convert_to_std_string(int argc, char *argv[]);
    private:
        std::vector<std::string> _argv;
        std::vector<ArgumentRule> _rules;
        std::unordered_map<std::string, std::string> _options;
    };
}

#endif // __ARGUMENTPARSER_HPP__
