//
// Created by raihan on 7/10/22.
//

#pragma once

#ifndef STEP_PARSER_H
#define STEP_PARSER_H

#include "lexer.h"
#include "ast.h"
#include "frame.h"

namespace step {
    class Parser : public ExpressionVisitor,
                   public StatementVisitor,
                   public StatementEvaluatorVisitor,
                   public ExpressionEvaluatorVisitor
    {
    public:
        using frame_t = std::shared_ptr<Frame>;

        Parser(string const &content, string const &fname);
        ~Parser();

        /*void parse_file(string const &fname);*/
        StatementNodePtr parse();
        void print(BinaryExpression *bexpr) override;
        void print(ConstantExpression *nexpr) override;
        void print(IdentifierExpression *nexpr) override;
        void print(FunctionCallExpression *nexpr) override;

        void print(PrintStatement *stmt) override;
        void print(AssignmentStatement *stmt) override;
        void print(ExpressionStatement *stmt) override;
        void print(FunctionDefStatement *stmt) override;
        void print(ReturnStatement *stmt) override;
        void print(IfStatement *stmt) override;
        void print(ElseStatement *stmt) override;
        void print(ElifStatement *stmt) override;
    private:
        vector<Token> tokens;
        i32 tIndex{};  // 'tokens' index
        i32 cur_token{};
        std::shared_ptr<ErrorManager> errorManager;
        frame_t cur_frame;
        stack<frame_t> call_stack;
        bool inside_function = false;
        bool return_statement_evaluated = false;
        bool parsed_if = false;

        Token const &next_token();
        Token const &peek_token(i32 peek=0);
        bool is_next(TokenKind kind);
        bool is_next_n(TokenKind kind, i32 peek);
        bool eat_if(TokenKind kind);
        bool eat_if(std::initializer_list<TokenKind> kinds);
        bool is_eof();

        ExpressionNodePtr parse_expression();
        ExpressionNodePtr parse_logicalOr(i32 precedence);
        ExpressionNodePtr parse_logicalAnd(i32 precedence);
        ExpressionNodePtr parse_equality(i32 precedence);
        ExpressionNodePtr parse_comparison(i32 precedence);
        ExpressionNodePtr parse_term(i32 precedence);
        ExpressionNodePtr parse_factor(i32 precedence);
        ExpressionNodePtr parse_primary(i32 precedence);
        ExpressionNodePtr parse_identifier();
        ExpressionNodePtr parse_function_call(Token const &name);
        i32 binary_precedence(TokenKind op);
        i32 unary_precedence(TokenKind op);

        StatementNodePtr parse_statement();
        StatementNodePtr parse_print_statement();
        StatementNodePtr parse_assignment_statement();
        StatementNodePtr parse_function_def_statement();
        StatementNodePtr parse_return_statement();
        StatementNodePtr parse_if_statement();
        StatementNodePtr parse_else_statement();
        StatementNodePtr parse_elif_statement();
        /* StatementNodePre parse_if_statement(); */

        void evaluate(PrintStatement *stms) override;
        void evaluate(ExpressionStatement *stms) override;
        void evaluate(ReturnStatement *stms) override;
        void evaluate(IfStatement *stms) override;
        void evaluate(ElseStatement *stms) override;
        void evaluate(ElifStatement *stms) override;
        void evaluate(AssignmentStatement *stms) override;

        void evaluate(FunctionDefStatement *stms);

        void evaluate(ConstantExpression *expr) override;
        void evaluate(IdentifierExpression *expr) override;
        void evaluate(BinaryExpression *expr) override;
        void evaluate(FunctionCallExpression *expr) override;

        void __builtin_print_function(i64 args);
    };

} // step

#endif //STEP_PARSER_H
