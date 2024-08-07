/*
 * created by: shunnobir
 * date: 05/08/2024
 */

#include "ExpressionStatementNode.hpp"
#include <memory>
#include <sstream>
#include <string>

Step::ExpressionStatementNode::ExpressionStatementNode(std::unique_ptr<Step::IAst> expr)
    : _expr{std::move(expr)}
{
    IAst::_return_type = _expr->_return_type;
}

std::string Step::ExpressionStatementNode::to_string(int indent) const {
    std::ostringstream out;
    out << std::string(indent * 4, ' ') << "(ExpressionStatement\n"
        << _expr->to_string(indent+1)
        << std::string(indent * 4, ' ') << "ExpressionStatement)\n";
    return out.str();
}

void Step::ExpressionStatementNode::emit() {
    _expr->emit();
}
