/*
 * created by: shunnobir
 * date: 31/07/2024
 */

#include "OpCode.hpp"
#include <string>

std::string Step::to_string(Step::OpCode op) {
    switch (op) {
        case OpCode::halt:
            return "halt";
        case OpCode::iadd:
            return "iadd";
        case OpCode::iconst:
            return "iconst";
        case OpCode::idiv:
            return "idiv";
        case OpCode::iload:
            return "iload";
        case OpCode::imod:
            return "imod";
        case OpCode::imul:
            return "imul";
        case OpCode::pop:
            return "pop";
        case OpCode::ipush:
            return "ipush";
        case OpCode::istore:
            return "istore";
        case OpCode::isub:
            return "isub";
        case OpCode::fadd:
            return "fadd";
        case OpCode::fconst:
            return "fconst";
        case OpCode::fdiv:
            return "fdiv";
        case OpCode::fload:
            return "fload";
        case OpCode::fmod:
            return "fmod";
        case OpCode::fmul:
            return "fmul";
        case OpCode::fpush:
            return "fpush";
        case OpCode::fstore:
            return "fstore";
        case OpCode::fsub:
            return "fsub";
        case OpCode::itof:
            return "itof";
        default:
            return "error";
    }
}
