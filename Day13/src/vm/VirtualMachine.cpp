/*
 * created by: shunnobir
 * date: 31/07/2024
 */

#include "VirtualMachine.hpp"
#include "../define.hpp"
#include "Object.hpp"
#include "OpCode.hpp"
#include <bitset>
#include <iomanip>
#include <iostream>
#include <memory>

using LongInteger = Step::Integer<i64>;
using DoubleFloat = Step::Float<f64>;

void Step::StackVirtualMachine::run() {
    _ip = _code.begin();
    bool is_halted = false;
    while (!is_halted) {
        auto instr = fetch();
        OpCode op = to_opcode(instr);
        switch (op) {
            case OpCode::halt:
                is_halted = true;
                break;
            case OpCode::iadd:
                execute_iadd();
                break;
            case OpCode::iconst:
                execute_iconst(get_immediate());
                break;
            case OpCode::idiv:
                execute_idiv();
                break;
            case OpCode::iload:
                execute_iload(get_immediate());
                break;
            case OpCode::imul:
                execute_imul();
                break;
            case OpCode::imod:
                execute_imod();
                break;
            case OpCode::ipush:
                execute_ipush(get_immediate());
                break;
            case OpCode::istore:
                execute_istore(get_immediate());
                break;
            case OpCode::isub:
                execute_isub();
                break;
            case OpCode::fadd:
                execute_fadd();
                break;
            case OpCode::fconst:
                execute_fconst(get_immediate());
                break;
            case OpCode::fdiv:
                execute_fdiv();
                break;
            case OpCode::fload:
                execute_fload(get_immediate());
                break;
            case OpCode::fmul:
                execute_fmul();
                break;
            case OpCode::fmod:
                execute_fmod();
                break;
            case OpCode::fpush:
                execute_fpush(get_immediate());
                break;
            case OpCode::fstore:
                execute_fstore(get_immediate());
                break;
            case OpCode::fsub:
                execute_fsub();
                break;
            case OpCode::itof:
                execute_itof();
                break;
            case OpCode::icmp:
                execute_icmp();
                break;
            case OpCode::fcmp:
                execute_fcmp();
                break;
            case OpCode::jz:
                execute_jz(get_immediate());
                break;
            case OpCode::jnz:
                execute_jnz(get_immediate());
                break;
            case OpCode::jn:
                execute_jn(get_immediate());
                break;
            case OpCode::jp:
                execute_jp(get_immediate());
                break;
            case OpCode::jt:
                execute_jt(get_immediate());
                break;
            case OpCode::jf:
                execute_jf(get_immediate());
                break;
            case OpCode::pushtp:
                *_sp++ = std::make_shared<Boolean>(_flags & POSITIVE);
                break;
            case OpCode::pushfp:
                *_sp++ = std::make_shared<Boolean>(!(_flags & POSITIVE));
                break;
            case OpCode::pushtz:
                *_sp++ = std::make_shared<Boolean>(_flags & ZERO);
                break;
            case OpCode::pushfz:
                *_sp++ = std::make_shared<Boolean>(!(_flags & ZERO));
                break;
            case OpCode::pushtn:
                *_sp++ = std::make_shared<Boolean>(_flags & NEGATIVE);
                break;
            case OpCode::pushfn:
                *_sp++ = std::make_shared<Boolean>(!(_flags & NEGATIVE));
                break;
            case OpCode::pop:
                execute_pop();
                break;
            default:
                is_halted = true;
                break;
        }
    }
}

Step::StackVirtualMachine::value_type Step::StackVirtualMachine::fetch() {
    if (_ip >= _code.end())
        return 0x0;
    return *_ip++;
}

Step::OpCode Step::StackVirtualMachine::to_opcode(value_type instr) const {
    return Step::as<OpCode>(instr);
}

Step::StackVirtualMachine::operand_type
Step::StackVirtualMachine::get_immediate() {
    operand_type immediate = (Step::as<u64>(fetch()) << 56);
    immediate |= (Step::as<u64>(fetch()) << 48);
    immediate |= (Step::as<u64>(fetch()) << 40);
    immediate |= (Step::as<u64>(fetch()) << 32);
    immediate |= (Step::as<u64>(fetch()) << 24);
    immediate |= (Step::as<u64>(fetch()) << 16);
    immediate |= (Step::as<u64>(fetch()) << 8);
    immediate |= (Step::as<u64>(fetch()));
    return immediate;
}

void Step::StackVirtualMachine::execute_pop() { --_sp; }

void Step::StackVirtualMachine::execute_iconst(
        Step::StackVirtualMachine::operand_type operand) {
    *_sp = std::make_shared<LongInteger>(operand);
    ++_sp;
}

void Step::StackVirtualMachine::execute_ipush(
        Step::StackVirtualMachine::operand_type operand) {
    *_sp = std::make_shared<LongInteger>(operand);
    ++_sp;
}

void Step::StackVirtualMachine::execute_iload(
        Step::StackVirtualMachine::operand_type operand) {
    *_sp = _constant_pool.at(operand);
    ++_sp;
}

void Step::StackVirtualMachine::execute_istore(
        Step::StackVirtualMachine::operand_type operand) {
    *std::dynamic_pointer_cast<LongInteger>(_constant_pool.at(operand)) = operand;
}

void Step::StackVirtualMachine::execute_iadd() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    *_sp = std::make_shared<LongInteger>(operand1 + operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_isub() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    *_sp = std::make_shared<LongInteger>(operand1 - operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_imul() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    *_sp = std::make_shared<LongInteger>(operand1 * operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_idiv() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    *_sp = std::make_shared<LongInteger>(operand1 / operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_imod() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    *_sp = std::make_shared<LongInteger>(operand1 % operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fconst(
        Step::StackVirtualMachine::operand_type operand) {
    union {
        Step::StackVirtualMachine::operand_type _data;
        f64 _d;
    } f = {operand};
    *_sp = std::make_shared<DoubleFloat>(f._d);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fpush(
        Step::StackVirtualMachine::operand_type operand) {
    union {
        Step::StackVirtualMachine::operand_type _data;
        f64 _d;
    } f = {operand};
    *_sp = std::make_shared<DoubleFloat>(f._d);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fload(
        Step::StackVirtualMachine::operand_type operand) {
    *_sp = _constant_pool.at(operand);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fstore(
        Step::StackVirtualMachine::operand_type operand) {
    union {
        Step::StackVirtualMachine::operand_type _data;
        f64 _d;
    } f = {operand};
    *std::dynamic_pointer_cast<DoubleFloat>(_constant_pool.at(operand)) = f._d;
}

void Step::StackVirtualMachine::execute_fadd() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    *_sp = std::make_shared<DoubleFloat>(operand1 + operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fsub() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    *_sp = std::make_shared<DoubleFloat>(operand1 - operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fmul() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    *_sp = std::make_shared<DoubleFloat>(operand1 * operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fdiv() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    *_sp = std::make_shared<DoubleFloat>(operand1 / operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_fmod() {
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    *_sp = std::make_shared<DoubleFloat>(operand1 % operand2);
    ++_sp;
}

void Step::StackVirtualMachine::execute_itof() {
    auto operand = *std::dynamic_pointer_cast<LongInteger>(*(_sp - 1));
    *(_sp - 1) = std::make_shared<DoubleFloat>(operand);
}

void Step::StackVirtualMachine::execute_icmp() {
    _flags = 0;
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<LongInteger>(*_sp);
    auto result = operand1 - operand2;
    if (result > LongInteger(0)) _flags |= POSITIVE;
    if (result < LongInteger(0)) _flags |= NEGATIVE;
    if (result == LongInteger(0)) _flags |= ZERO;
}

void Step::StackVirtualMachine::execute_fcmp() {
    _flags = 0;
    --_sp;
    auto operand2 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    --_sp;
    auto operand1 = *std::dynamic_pointer_cast<DoubleFloat>(*_sp);
    auto result = operand1 - operand2;
    if (result > DoubleFloat(0)) _flags |= POSITIVE;
    if (result < DoubleFloat(0)) _flags |= NEGATIVE;
    if (result == DoubleFloat(0)) _flags |= ZERO;
}

void Step::StackVirtualMachine::execute_jz(operand_type operand) {
    if (_flags & ZERO) {
        _ip += operand;
    }
}

void Step::StackVirtualMachine::execute_jnz(operand_type operand) {
    if (!(_flags & ZERO)) {
        _ip += operand;
    }
}

void Step::StackVirtualMachine::execute_jn(operand_type operand) {
    if (_flags & NEGATIVE) {
        _ip += operand;
    }
}

void Step::StackVirtualMachine::execute_jp(operand_type operand) {
    if (_flags & POSITIVE) {
        _ip += operand;
    }
}

void Step::StackVirtualMachine::execute_jt(operand_type operand) {
    --_sp;
    auto condition = *std::dynamic_pointer_cast<Boolean>(*_sp);
    if (condition) {
        _ip += operand;
    }
}

void Step::StackVirtualMachine::execute_jf(operand_type operand) {
    --_sp;
    auto condition = *std::dynamic_pointer_cast<Boolean>(*_sp);
    if (!condition) {
        _ip += operand;
    }
}

void Step::StackVirtualMachine::print1byte(u8 byte) const {
    std::cout << std::hex << std::showbase << std::setw(2) << std::setfill('0')
        << as<i32>(byte) << ' '
        << std::resetiosflags(std::ios_base::basefield);
}

void Step::StackVirtualMachine::print1byte_bin(u8 byte) const {
    std::cout << std::bitset<8>(byte).to_string() << ' ';
}

i32 Step::StackVirtualMachine::print_instruction(u8 opcode, std::size_t index,
        bool binary) const {
    bool integer = true;
    switch (Step::as<OpCode>(opcode)) {
        case OpCode::halt:
        case OpCode::iadd:
        case OpCode::idiv:
        case OpCode::imul:
        case OpCode::imod:
        case OpCode::pop:
        case OpCode::isub:
        case OpCode::fadd:
        case OpCode::fdiv:
        case OpCode::fmul:
        case OpCode::fmod:
        case OpCode::fsub:
        case OpCode::itof:
        case OpCode::icmp:
        case OpCode::fcmp:
        case OpCode::pushtz:
        case OpCode::pushfz:
        case OpCode::pushtn:
        case OpCode::pushfn:
        case OpCode::pushtp:
        case OpCode::pushfp:
            if (!binary) {
                std::cout << "    // " << Step::to_string(to_opcode(opcode)) << " ";
            }
            break;
        case OpCode::fconst:
        case OpCode::fload:
        case OpCode::fpush:
        case OpCode::fstore:
            integer = false;
        case OpCode::iconst:
        case OpCode::iload:
        case OpCode::ipush:
        case OpCode::istore: {
                                 union {
                                     u8 _bytes[8];
                                     f64 _d;
                                     i64 _i;
                                 } t = {{_code[index + 7], _code[index + 6], _code[index + 5],
                                     _code[index + 4], _code[index + 3], _code[index + 2],
                                     _code[index + 1], _code[index]}};
                                 for (i32 i = 0; i < 8; ++i) {
                                     if (binary)
                                         print1byte_bin(t._bytes[7 - i]);
                                     else
                                         print1byte(t._bytes[7 - i]);
                                 }
                                 if (!binary) {
                                     std::cout << "    // " << Step::to_string(to_opcode(opcode)) << " ";
                                     std::cout << (integer ? t._i : t._d);
                                 }
                             }
                             return 8;
        default:
                             break;
    }
    std::cout << std::resetiosflags(std::ios_base::basefield);
    return 0;
}

void Step::StackVirtualMachine::print() const {
    print_constant_pool();
    print_code();
    print_stack();
}

void Step::StackVirtualMachine::print_code() const {
    std::cout << ".code\n";
    int offset = 0;
    std::cout << "offset\topcode byte8 byte7 byte6 byte5 byte4 byte3 byte2 "
        "byte1\tinstruction\n";
    std::cout << std::string(75, '-') << '\n';
    for (std::size_t j = 0; j < _code.size(); ++j) {
        auto i = _code.at(j);
        std::cout << std::setw(4) << std::setfill('0') << offset << "\t";
        print1byte(i);
        print_instruction(i, j + 1);
        std::cout << '\n';

        std::cout << std::string(4, ' ') << "\t";
        print1byte_bin(i);
        j += print_instruction(i, j + 1, true);
        std::cout << "\n\n";
        offset += 2;
    }
}

void Step::StackVirtualMachine::print_stack() const {
    std::cout << ".stack\n";
    int index = 1;
    for (auto i = _stack.begin(); i != _sp; ++i) {
        std::cout << std::setw(4) << std::setfill('0') << index << '\t';
        std::cout << (*i)->to_string() << "\n";
        ++index;
    }
}

void Step::StackVirtualMachine::print_constant_pool() const {
    std::cout << ".constants\n";
    for (auto &i : _constant_pool) {
        std::cout << i->to_string() << '\n';
    }
}
