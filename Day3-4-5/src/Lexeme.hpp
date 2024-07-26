/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __LEXEME_HPP__
#define __LEXEME_HPP__

#include <string>

namespace Step {
    enum class LexemeKind {
        INTEGER, FLOAT,
        // STRING, IDENTIFIER,
        PLUS, HYPHEN,
        STAR, SLASH,
        MODULOUS,
        LEFT_PAREN, RIGHT_PAREN,
        NEWLINE,
        MISCELLANEOUS, E_OI,
    };

    std::string to_string(LexemeKind kind);

    struct Lexeme {
        std::string _fname;
        std::string _lexeme;
        std::size_t _line;
        std::size_t _col;
        LexemeKind  _kind;
    };
}

#endif // __LEXEME_HPP__

