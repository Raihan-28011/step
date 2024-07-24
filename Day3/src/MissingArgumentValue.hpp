/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __MISSINGARGUMENTVALUE_HPP__
#define __MISSINGARGUMENTVALUE_HPP__

#include "IError.hpp"
#include <string>

namespace Step {
    class MissingArgumentValue: public IError {
    public:
        MissingArgumentValue(std::string option);
        virtual ~MissingArgumentValue() override = default;
        virtual std::string to_string() const override;
    private:
        std::string _option;
    };
}

#endif // __MISSINGARGUMENTVALUE_HPP__

