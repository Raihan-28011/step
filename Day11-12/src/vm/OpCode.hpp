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
    icmp,
    fcmp,
    jz,
    jnz,
    jp,
    jn,
    jt,
    jf,
    pushtp, // push true if positive
    pushfp, // push false if positive
    pushtz, // push true if zero
    pushfz, // push false if zero
    pushtn, // push true if negative
    pushfn, // push false if negative
    pop,
    total_op,
};

std::string to_string(OpCode op);

} // namespace Step

#endif // __OP_CODE_HPP__
