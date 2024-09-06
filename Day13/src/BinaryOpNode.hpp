/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#ifndef __BINARY_OP_NODE_HPP__
#define __BINARY_OP_NODE_HPP__

#include "IAst.hpp"
#include "Lexeme.hpp"
#include <memory>

namespace Step {
    class BinaryOpNode: public IAst {
    public:
        BinaryOpNode(std::unique_ptr<IAst> left, std::unique_ptr<IAst> right, Step::LexemeKind op);
        virtual ~BinaryOpNode() override = default;
        virtual std::string to_string(int indent = 0) const override;
        virtual void emit() override;
    private:
        void set_return_type();
    private:
        std::unique_ptr<IAst> _left;
        std::unique_ptr<IAst> _right;
        Step::LexemeKind _op;
    };
}

#endif // __BINARY_OP_NODE_HPP__

