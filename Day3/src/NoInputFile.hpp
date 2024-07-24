/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __NOINPUTFILE_HPP__
#define __NOINPUTFILE_HPP__

#include "IError.hpp"
#include <string>

namespace Step {
    class NoInputFile: public IError {
    public:
        virtual ~NoInputFile() override = default;
        virtual std::string to_string() const override;
    };
}

#endif // __NOINPUTFILE_HPP__
