/*
 * created by: shunnobir
 * date: 31/07/2024
 */

#ifndef __OP_CODE_HPP__
#define __OP_CODE_HPP__

#include "../define.hpp"
#include <string>

namespace Step {

enum class OpCode: u8 {
    halt = 0,
    fadd,
    fconst,
    fdiv,
    fload,
    fmod,
    fmul,
    fpush,
    fstore,
    fsub,
    iadd,
    iconst,
    idiv,
    iload,
    imod,
    imul,
    ipush,
    istore,
    isub,
    itof,
    pop,
    total_op,
};

std::string to_string(OpCode op);

} // namespace Step

#endif // __OP_CODE_HPP__
