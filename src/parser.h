//
// Created by raihan on 7/10/22.
//

#pragma once

#ifndef STEP_PARSER_H
#define STEP_PARSER_H

#include "lexer.h"
#include "ast.h"

namespace step {
    class Parser : public ExpressionVisitor, public StatementVisitor {
    public:
        Parser(string const &content, string const &fname);

        /*void parse_file(string const &fname);*/
        StatementNodePtr parse();
        void print(BinaryExpression *bexpr) override;
        void print(ConstantExpression *nexpr) override;
        void print(FunctionCallExpression *nexpr) override;

        void print(PrintStatement *stmt) override;
        void print(AssignmentStatement *stmt) override;
        void print(ExpressionStatement *stmt) override;
        void print(FunctionDefStatement *stmt) override;
    private:
        vector<Token> tokens;
        i32 tIndex{};  // 'tokens' index
        i32 cur_token{};
        ErrorManager &errorManager;

        Token const &next_token();
        Token const &peek_token(i32 peek=0);
        bool is_next(TokenKind kind);
        bool is_next_n(TokenKind kind, i32 peek);
        bool eat_if(TokenKind kind);
        bool eat_if(std::initializer_list<TokenKind> kinds);
        ExpressionNodePtr parse_expression();
        ExpressionNodePtr parse_term(i32 precedence);
        ExpressionNodePtr parse_factor(i32 precedence);
        ExpressionNodePtr parse_primary(i32 precedence);
        ExpressionNodePtr parse_function_call(Token const &name);
        i32 binary_precedence(TokenKind op);
        i32 unary_precedence(TokenKind op);

        StatementNodePtr parse_statement();
        StatementNodePtr parse_print_statement();
        StatementNodePtr parse_assignment_statement();
        StatementNodePtr parse_function_def_statement();

        bool is_eof();
    };

} // step

#endif //STEP_PARSER_H