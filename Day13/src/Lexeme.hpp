/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __LEXEME_HPP__
#define __LEXEME_HPP__

#include "define.hpp"
#include <string>

namespace Step {
    enum class LexemeKind {
        miscellaneous = -1, e_io,
        integer, FLOAT,
        // STRING, IDENTIFIER,
        plus, hyphen,
        star, slash,
        modulous,
        lparen, rparen,
        less_than, greater_than,
        less_than_equal, greater_than_equal,
        equal_equal, not_equal,
        logical_or, logical_and, logical_not,
        newline,
    };

    std::string to_string(LexemeKind kind);

    struct Lexeme {
        using lexeme_type = literal_type;
        using lexeme_int_type = i64;
        using lexeme_float_type = f64;

        std::string _fname;
        lexeme_type _lexeme;
        std::size_t _line;
        std::size_t _col;
        LexemeKind  _kind;
    };

    std::string to_string(Lexeme const &lexeme);
}

#endif // __LEXEME_HPP__

