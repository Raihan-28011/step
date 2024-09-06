/*
 * created by: shunnobir
 * date: 05/08/2024
 */

#include "ModuleNode.hpp"
#include "IAst.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

Step::ModuleNode::ModuleNode(std::vector<std::unique_ptr<Step::IAst>> &&stmts)
    : _stmts{std::move(stmts)}
{
    IAst::_return_type = AstReturnType::NONE;
}

std::string Step::ModuleNode::to_string(int indent) const {
    std::ostringstream out;
    out << std::string(indent * 4, ' ') << "(Module\n";
    for (auto &_stmt: _stmts) {
            out << _stmt->to_string(indent+1);
    }
    out << std::string(indent * 4, ' ') << "Module)\n";
    return out.str();
}

void Step::ModuleNode::emit() {
    for (auto &_stmt: _stmts) {
        _stmt->emit();
    }
}
