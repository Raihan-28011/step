/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#ifndef __LITERAL_NODE_HPP__
#define __LITERAL_NODE_HPP__

#include "IAst.hpp"
#include "Lexeme.hpp"
#include <string>
#include <variant>

namespace Step {
    class LiteralNode: public IAst {
    public:
        LiteralNode(std::variant<std::string, long long, double> literal, Step::LexemeKind type);
        virtual ~LiteralNode() override = default;
        virtual std::string to_string(int indent = 0) const override;
        virtual void emit() override;
    private:
        std::variant<std::string, long long, double> _literal;
        Step::LexemeKind _type;
    };
}

#endif // __LITERAL_NODE_HPP__


