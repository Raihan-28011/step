/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#include "BinaryOpNode.hpp"
#include "Lexeme.hpp"
#include <memory>
#include <sstream>
#include <string>

Step::BinaryOpNode::BinaryOpNode(std::unique_ptr<Step::IAst> left,
                                     std::unique_ptr<Step::IAst> right,
                                     Step::LexemeKind op)
    : _left{std::move(left)},
      _right{std::move(right)},
      _op{op}
{
}

std::string Step::BinaryOpNode::to_string(int indent) const {
    std::ostringstream out;
    out << std::string(indent * 4, ' ') << "(BinaryOp\n"
        << _left->to_string(indent+1)
        << std::string((indent+1) * 4, ' ') << Step::to_string(_op) << '\n'
        << _right->to_string(indent+1)
        << std::string(indent * 4, ' ') << "BinaryOp)\n";
    return out.str();
}

