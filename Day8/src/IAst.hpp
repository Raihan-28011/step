/*
 * created by: shunnobir
 * date: 28/07/2024
 */

#ifndef __IAST_HPP__
#define __IAST_HPP__

#include <string>

namespace Step {
    class IAst {
    public:
        virtual ~IAst() = default;
        virtual std::string to_string(int indent) const = 0;
    };
}

#endif // __IAST_HPP__
