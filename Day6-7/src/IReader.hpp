/*
 * created by: shunnobir
 * date: 22/07/2024
 */

#ifndef __IREADER_HPP__
#define __IREADER_HPP__

#include <string>

namespace Step {
    /* Reader Interface (Abstract Class) */
    class IReader {
    public:
        /* End Of Input */
        static std::string const E_OI;
    public:
        virtual ~IReader() = default;
        virtual std::string read() = 0;
    };
}

#endif // __IREADER_HPP__
