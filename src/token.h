//
// Created by raihan on 7/9/22.
//

//#ifndef STEP_TOKEN_H
//#define STEP_TOKEN_H
#pragma once

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

#ifdef __DEBUG_STEP__
        void print() const {
            switch (kind) {
                case t_error: std::cout << "t_error\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_eof: std::cout << "t_eof\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_def: std::cout << "t_def\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_print: std::cout << "t_print\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_input: std::cout << "t_input\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_true: std::cout << "t_true\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_false: std::cout << "t_false\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_null: std::cout << "t_null\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_ident: std::cout << "t_ident\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_string: std::cout << "t_string\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_num: std::cout << "t_num\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_equal: std::cout << "t_equal\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_semicolon: std::cout << "t_semicolon\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_dot: std::cout << "t_dot\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_lparen: std::cout << "t_lparent\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_rparen: std::cout << "t_rparent\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_lbrace: std::cout << "t_lbrace\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_rbrace: std::cout << "t_rbrace\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_lsqbrace: std::cout << "t_lsqbrace\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_rsqbrace: std::cout << "t_rsqbrace\t'"
                                   << tok
                                   << "'\tat line: "
                                   << line
                                   << ", col: "
                                   << col
                                   << '\n'; break;
                case t_plus: std::cout << "t_plus\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_minus: std::cout << "t_minus\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_star: std::cout << "t_star\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_slash: std::cout << "t_slash\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_modulus: std::cout << "t_modulus\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_equalequal: std::cout << "t_equalequal\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_notequal: std::cout << "t_notequal\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_bang: std::cout << "t_bang\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_less: std::cout << "t_less\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_greater: std::cout << "t_greater\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_lessequal: std::cout << "t_lessequal\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_greaterequal: std::cout << "t_greaterequal\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_logicaland: std::cout << "t_logicaland\t'"
                                       << tok
                                       << "'\tat line: "
                                       << line
                                       << ", col: "
                                       << col
                                       << '\n'; break;
                case t_logicalor: std::cout << "t_logicalor\t'"
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


//#endif //STEP_TOKEN_H
