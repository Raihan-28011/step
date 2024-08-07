/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#include "GroupNode.hpp"
#include <memory>
#include <sstream>
#include <string>

Step::GroupNode::GroupNode(std::unique_ptr<Step::IAst> expr)
    : _expr{std::move(expr)}
{
    IAst::_return_type = _expr->_return_type;
}

std::string Step::GroupNode::to_string(int indent) const {
    std::ostringstream out;
    out << std::string(indent * 4, ' ') << "(Group\n"
        << _expr->to_string(indent+1)
        << std::string(indent * 4, ' ') << "Group)\n";
    return out.str();
}

void Step::GroupNode::emit() {
    _expr->emit();
}
