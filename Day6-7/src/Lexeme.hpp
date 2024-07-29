/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __LEXEME_HPP__
#define __LEXEME_HPP__

#include <string>
#include <variant>

namespace Step {
    enum class LexemeKind {
        MISCELLANEOUS = -1, E_OI,
        INTEGER, FLOAT,
        // STRING, IDENTIFIER,
        PLUS, HYPHEN,
        STAR, SLASH,
        MODULOUS,
        LEFT_PAREN, RIGHT_PAREN,
        NEWLINE,
    };

    std::string to_string(LexemeKind kind);

    struct Lexeme {
        using lexeme_type = std::variant<std::string, long long, double>;
        using lexeme_int_type = long long;
        using lexeme_float_type = double;

        std::string _fname;
        lexeme_type _lexeme;
        std::size_t _line;
        std::size_t _col;
        LexemeKind  _kind;
    };

    std::string to_string(Lexeme const &lexeme);
}

#endif // __LEXEME_HPP__

