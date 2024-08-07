/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#include "LiteralNode.hpp"
#include "IAst.hpp"
#include "define.hpp"
#include "vm/OpCode.hpp"
#include "vm/VirtualMachine.hpp"
#include <sstream>
#include <string>

Step::LiteralNode::LiteralNode(literal_type literal, Step::LexemeKind type)
    : _literal{std::move(literal)},
      _type{type}
{
    if (literal.index() == 1) {
        IAst::_return_type = AstReturnType::INTEGER;
    } else if (literal.index() == 2) {
        IAst::_return_type = AstReturnType::DOUBLE;
    }
}

std::string Step::LiteralNode::to_string(int indent) const {
    std::ostringstream out;
    out << std::string(indent * 4, ' ') << "(Literal\n"
        << std::string((indent+1) * 4, ' ');

    if (_literal.index() == 0)
        out << '"' << std::get<0>(_literal) << '"';
    else if (_literal.index() == 1)
        out << std::get<1>(_literal);
    else
        out << std::get<2>(_literal);

    out << '\n' << std::string(indent * 4, ' ') << "Literal)\n";
    return out.str();
}

void Step::LiteralNode::emit() {
    auto &vm = StackVirtualMachine::instance();
    if (_literal.index() == 1) {
        vm.push_back(Step::OpCode::ipush);
        vm.push_back(std::get<1>(_literal));
    } else if (_literal.index() == 2) {
        vm.push_back(Step::OpCode::fpush);
        vm.push_back(std::get<2>(_literal));
    }
}
