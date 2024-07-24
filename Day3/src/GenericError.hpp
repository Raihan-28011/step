/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __GENERICERROR_HPP__
#define __GENERICERROR_HPP__

#include "IError.hpp"
#include <string>

namespace Step {
    class GenericError: public IError {
    public:
        GenericError(std::string error);
        virtual ~GenericError() override = default;
        virtual std::string to_string() const override;
    private:
        std::string _error;
    };
}

#endif // __GENERICERROR_HPP__


