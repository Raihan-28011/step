//
// Created by raihan on 7/10/22.
//

#pragma once

#ifndef STEP_PARSER_H
#define STEP_PARSER_H

#include "lexer.h"
#include "ast.h"

namespace step {
    class Parser : ExpressionPrintVisiter {
    public:
        Parser(string const &content, string const &fname);

        /*void parse_file(string const &fname);*/
        ExpressionNodePtr parse();
        void print(BinaryExpression *bexpr) override;
        void print(NumberExpression *nexpr) override;
    private:
        vector<Token> tokens;
        i32 tIndex{};  // 'tokens' index
        i32 cur_token{};
        ErrorManager &errorManager;

        Token const &next_token();
        Token const &peek_token(i32 peek=0);
        bool is_next(TokenKind kind);
        bool eat_if(TokenKind kind);
        bool eat_if(std::initializer_list<TokenKind> kinds);
        ExpressionNodePtr parse_expression();
        ExpressionNodePtr parse_term(i32 precedence);
        ExpressionNodePtr parse_factor(i32 precedence);
        ExpressionNodePtr parse_primary(i32 precedence);
        i32 binary_precedence(TokenKind op);
        i32 unary_precedence(TokenKind op);

        bool is_eof();
    };

} // step

#endif //STEP_PARSER_H