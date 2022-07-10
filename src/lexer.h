//
// Created by raihan on 7/9/22.
//

#pragma once

#ifndef STEP_LEXER_H
#define STEP_LEXER_H

#include "token.h"
#include "error.h"

namespace step {

    class Lexer {
    public:
        Lexer(string const &file, string const &fname);
        ~Lexer() = default;

        vector<Token> const &get_tokens() const { return tokens; }

#ifdef __DEBUG_STEP__
        void print() {
            for (auto const &t: tokens)
                t.print();
        }
#endif

    private:
        string const &file;
        string const &fname;
        ErrorManager &errorManger;

        vector<Token> tokens;
        Token eof_token{};
        i32 line{1};
        i32 col{0};
        i32 tok_index{0};

        i32 text_index{0};
        i32 text_size{0};

        void tokenize();
        bool is_eof(char c) { return c == std::char_traits<char>::eof(); }
        char eof_char() { return std::char_traits<char>::eof(); }
        void skip_whitespace();
        char next_char();
        char peek_char();
        void add_t(TokenKind tok, string &&s);
        void num_token(char c);
        void indentifier_token(char c);
        void unknown_token(char c);
        void string_token(char c);
        void comment_token(char c);
    };

} // step

#endif //STEP_LEXER_H
