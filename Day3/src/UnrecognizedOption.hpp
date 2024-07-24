/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __UNRECOGNIZEDOPTION_HPP__
#define __UNRECOGNIZEDOPTION_HPP__

#include "IError.hpp"
#include <string>

namespace Step {
    class UnrecognizedOption: public IError {
    public:
        UnrecognizedOption(std::string option);
        virtual ~UnrecognizedOption() override = default;
        virtual std::string to_string() const override;
    private:
        std::string _option;
    };
}

#endif // __UNRECOGNIZEDOPTION_HPP__
