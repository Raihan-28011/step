/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#include "LiteralNode.hpp"
#include <sstream>
#include <string>
#include <variant>

Step::LiteralNode::LiteralNode(std::variant<std::string, long long, double> literal, Step::LexemeKind type)
    : _literal{std::move(literal)},
      _type{type}
{
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
