/*
 * created by: shunnobir
 * date: 26/07/2024
 */

#ifndef __PRECOMPILATIONERROR_HPP__
#define __PRECOMPILATIONERROR_HPP__

#include "IError.hpp"

namespace Step {
    class PreCompilationError: public IError {
    public:
        PreCompilationError(IError::ErrorCode code, std::string extra = "");
        virtual ~PreCompilationError() override = default;
        virtual std::string to_string() const override;
    private:
        std::string _error;
    };
}

#endif // __PRECOMPILATIONERROR_HPP__
