/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#ifndef __LITERAL_NODE_HPP__
#define __LITERAL_NODE_HPP__

#include "IAst.hpp"
#include "Lexeme.hpp"
#include "define.hpp"
#include <string>

namespace Step {
    class LiteralNode: public IAst {
    public:
        LiteralNode(literal_type literal, Step::LexemeKind type);
        virtual ~LiteralNode() override = default;
        virtual std::string to_string(int indent = 0) const override;
        virtual void emit() override;
    private:
        literal_type _literal;
        Step::LexemeKind _type;
    };
}

#endif // __LITERAL_NODE_HPP__


