/*
 * created by: shunnobir
 * date: 23/07/2024
 */


#include "Lexeme.hpp"
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

std::string Step::to_string(Step::LexemeKind kind) {
    switch (kind) {
    case Step::LexemeKind::INTEGER:
        return "INTEGER";
    case Step::LexemeKind::FLOAT:
        return "FLOAT";
    // case Step::LexemeKind::STRING:
    //     return "STRING";
    // case Step::LexemeKind::IDENTIFIER:
    //     return "IDENTIFIER";
    case Step::LexemeKind::PLUS:
        return "PLUS";
    case Step::LexemeKind::HYPHEN:
        return "HYPHEN";
    case Step::LexemeKind::STAR:
        return "STAR";
    case Step::LexemeKind::SLASH:
        return "SLASH";
    case Step::LexemeKind::MODULOUS:
        return "MODULOUS";
    case Step::LexemeKind::LEFT_PAREN:
        return "LEFT_PAREN";
    case Step::LexemeKind::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case Step::LexemeKind::NEWLINE:
        return "NEWLINE";
    case Step::LexemeKind::MISCELLANEOUS:
        return "MISCELLANEOUS";
    default:
        return "E_OI";
    }
}

std::string Step::to_string(Lexeme const &lexeme) {
    auto const &token = lexeme._lexeme;
    if (token.index() == 0) {
        return std::get<std::string>(token);
    } else if (token.index() == 1) {
        return std::to_string(std::get<long long>(token));
    } else {
        std::ostringstream out;
        out << std::scientific << std::get<double>(token);
        return out.str();
    }
}
