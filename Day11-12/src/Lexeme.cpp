/*
 * created by: shunnobir
 * date: 23/07/2024
 */


#include "Lexeme.hpp"
#include <ios>
#include <sstream>
#include <string>

std::string Step::to_string(Step::LexemeKind kind) {
    switch (kind) {
    case Step::LexemeKind::integer:
        return "integer";
    case Step::LexemeKind::FLOAT:
        return "float";
    case Step::LexemeKind::plus:
        return "plus";
    case Step::LexemeKind::hyphen:
        return "hyphen";
    case Step::LexemeKind::star:
        return "star";
    case Step::LexemeKind::slash:
        return "slash";
    case Step::LexemeKind::modulous:
        return "modulous";
    case Step::LexemeKind::lparen:
        return "lparen";
    case Step::LexemeKind::rparen:
        return "rparen";
    case Step::LexemeKind::less_than:
        return "less_than";
    case Step::LexemeKind::greater_than:
        return "greater_than";
    case Step::LexemeKind::less_than_equal:
        return "less_than_equal";
    case Step::LexemeKind::greater_than_equal:
        return "greater_than_equal";
    case Step::LexemeKind::equal_equal:
        return "equal_equal";
    case Step::LexemeKind::not_equal:
        return "not_equal";
    case Step::LexemeKind::newline:
        return "newline";
    case Step::LexemeKind::miscellaneous:
        return "miscellaneous";
    default:
        return "E_OI";
    }
}

std::string Step::to_string(Lexeme const &lexeme) {
    auto const &token = lexeme._lexeme;
    if (token.index() == 0) {
        return std::get<std::string>(token);
    } else if (token.index() == 1) {
        return std::to_string(std::get<1>(token));
    } else {
        std::ostringstream out;
        out << std::scientific << std::get<2>(token);
        return out.str();
    }
}
