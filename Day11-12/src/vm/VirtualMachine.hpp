/*
 * created by: shunnobir
 * date: 31/07/2024
 */

#ifndef __VIRTUAL_MACHINE_HPP__
#define __VIRTUAL_MACHINE_HPP__

#include "../define.hpp"
#include "Object.hpp"
#include "OpCode.hpp"
#include <memory>
#include <vector>

namespace Step {

class NotCopyableOrMovable {
    public:
        NotCopyableOrMovable() = default;
        NotCopyableOrMovable(NotCopyableOrMovable const &) = delete;
        NotCopyableOrMovable(NotCopyableOrMovable &&) = delete;

        NotCopyableOrMovable &operator=(NotCopyableOrMovable const &) = delete;
        NotCopyableOrMovable &operator=(NotCopyableOrMovable &&) = delete;
};

/*
 * A) What does a Virtual Machine do?
 * Ans:
 * 1. It fetched an instruction from memory
 * 2. It decodes the instruction
 * 3. Executes the instruction
 *
 * B) What will be the size of each instruction?
 * Ans: Depends on the type of virtual machine we will be using.
 * If it is a stack based virtual machine, then the opcodes are between 1 byte
 * and operands can span between 1 to 4 bytes.
 * If it is a register based virtual machine, then the opcodes are between [5-8] bits
 * and operands can span between 5 to 20 bits depending on the architecture.
 */

class VirtualMachine: public NotCopyableOrMovable {
    public:
    public:
        virtual void run() = 0; // This function does the decoding and executes instructions
    protected:
        VirtualMachine() = default;
};

/*
 * What does a stack machine needs?
 * 1. Instruction set (opcode)
 * 2. Stack
 * 3. Constant pool (optional but absolutely useful)
 * 4. Instruciton pointer
 * 5. Stack pointer
 */

class StackVirtualMachine: public VirtualMachine {
    public:
        /* aliases */
        using value_type = u8;
        using code_type = std::vector<value_type>;
        using constant_type = std::vector<std::shared_ptr<Step::Object>>;
        using stack_type = std::vector<std::shared_ptr<Step::Object>>;
        using operand_type = u64;
        using flag_type = u8;

        /* constants */
        static constexpr std::size_t MAX_CODE_SIZE = (1LU << 20);
        static constexpr std::size_t MAX_STACK_SIZE = (1LU << 10);
        static constexpr std::size_t MAX_CONSTANT_POOL_SIZE = (1LU << 10);
        static constexpr flag_type ZERO = 0x1;
        static constexpr flag_type POSITIVE = 0x2;
        static constexpr flag_type NEGATIVE = 0x4;
    public:
        static StackVirtualMachine &instance() {
            static StackVirtualMachine _instance;
            return _instance;
        }

        template <typename T>
        void push_back(T val) {
            union {
                T _data;
                u8 _bytes[8];
            } t = { val };

            for (i64 i = sizeof(T)-1; i >= 0; --i) {
                _code.push_back(t._bytes[i]);
            }
        }

        void push_back(Step::OpCode val) {
            _code.push_back(Step::as<u8>(val));
        }

        void run() override;

        void print1byte(u8 byte) const;
        void print1byte_bin(u8 byte) const;
        i32 print_instruction(u8 opcode, std::size_t index, bool binary = false) const;
        void print() const;
        void print_code() const;
        void print_stack() const;
        void print_constant_pool() const;

    protected:
        /* Constructor */
        StackVirtualMachine() 
            : _flags{0} 
        {
            _stack.resize(MAX_STACK_SIZE);
            _sp = _stack.begin();
        }
    private:
        value_type fetch();
        OpCode to_opcode(value_type instr) const;
        operand_type get_immediate();
        void execute_pop();
        void execute_iconst(operand_type operand);
        void execute_ipush(operand_type operand);
        void execute_iload(operand_type operand);
        void execute_istore(operand_type operand);
        void execute_iadd();
        void execute_isub();
        void execute_imul();
        void execute_idiv();
        void execute_imod();
        void execute_fconst(operand_type operand);
        void execute_fpush(operand_type operand);
        void execute_fload(operand_type operand);
        void execute_fstore(operand_type operand);
        void execute_fadd();
        void execute_fsub();
        void execute_fmul();
        void execute_fdiv();
        void execute_fmod();
        void execute_itof();
        void execute_icmp();
        void execute_fcmp();
        void execute_jz(operand_type operand);
        void execute_jnz(operand_type operand);
        void execute_jn(operand_type operand);
        void execute_jp(operand_type operand);
        void execute_jt(operand_type operand);
        void execute_jf(operand_type operand);
    private:
        code_type _code;
        code_type::iterator _ip;

        stack_type _stack;
        stack_type::iterator _sp;

        constant_type _constant_pool;
        flag_type _flags;
};



} // namespace Step

#endif // __VIRTUAL_MACHINE_HPP__
