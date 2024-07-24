/*
 * created by: shunnobir
 * date: 24/07/2024
 */

#ifndef __NOTVALIDFILE_HPP__
#define __NOTVALIDFILE_HPP__

#include "IError.hpp"
#include <string>

namespace Step {
    class NotValidFile: public IError {
    public:
        NotValidFile(std::string fname);
        virtual ~NotValidFile() override = default;
        virtual std::string to_string() const override;
    private:
        std::string _fname;
    };
}

#endif // __NOTVALIDFILE_HPP__

