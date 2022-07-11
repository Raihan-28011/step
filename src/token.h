//
// Created by raihan on 7/9/22.
//

//#ifndef STEP_TOKEN_H
//#define STEP_TOKEN_H
#pragma once

#ifndef STEP_TOKEN_H
#define STEP_TOKEN_H

#include "common.h"

namespace step {

    enum TokenKind {
        t_error = -2,
        t_eof,
        t_def,
        t_print,
        t_input,
        t_true,
        t_false,
        t_null,
        t_ident,
        t_string,
        t_num,
        t_equal,
        t_semicolon,
        t_comma,
        t_dot,
        t_lparen,
        t_rparen,
        t_lbrace,
        t_rbrace,
        t_lsqbrace,
        t_rsqbrace,
        t_plus,
        t_minus,
        t_star,
        t_slash,
        t_modulus,
        t_equalequal,
        t_notequal,
        t_bang,
        t_less,
        t_greater,
        t_lessequal,
        t_greaterequal,
        t_logicaland,
        t_logicalor,

        t_unknown,
        t_total
    };

    class Token {
    public:
        Token(TokenKind k = t_eof, string &&t = {}, i32 l = 0, i32 c = 0)
            : kind{k}, tok{std::move(t)}, line{l}, col{c}
        {
        }

        ~Token() = default;

        TokenKind get_kind() const { return kind; }
        string const &get_tok() const { return tok; }
        i32 get_line() const {  return line; }
        i32 get_col() const {  return col; }

        static std::array<string, t_total> const tok_s;

#ifdef __DEBUG_STEP__
        void print() const {
            switch (kind) {
                case t_error: std::cout << "t_error" << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_eof: std::cout << "t_eof" << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_def: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_print: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_input: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_true: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_false: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_null: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_ident: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_string: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_num: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_equal: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_semicolon: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_comma: std::cout << tok_s[kind] << "\t'"
                                        << tok
                                        << "'\tat line: "
                                        << line
                                        << ", col: "
                                        << col
                                        << '\n'; break;
                case t_dot: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_lparen: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_rparen: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_lbrace: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_rbrace: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_lsqbrace: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_rsqbrace: std::cout << tok_s[kind] << "\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_plus: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_minus: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_star: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_slash: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_modulus: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_equalequal: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_notequal: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_bang: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_less: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_greater: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_lessequal: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_greaterequal: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_logicaland: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_logicalor: std::cout << tok_s[kind] << "\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;

                default: std::cout << "t_unknown\t"
                                   << tok
                                   << "\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
            }
        }
#endif

    private:
        TokenKind kind;
        string tok;
        i32 line;
        i32 col;
    };
}


#endif //STEP_TOKEN_H
