//
// Created by raihan on 7/10/22.
//

#pragma once

#ifndef STEP_AST_H
#define STEP_AST_H

#include "token.h"

namespace step {

    struct ExpressionNode;
    struct BinaryExpression;
    struct ConstantExpression;
    struct IdentifierExpression;
    struct FunctionCallExpression;
    using ExpressionNodePtr = std::shared_ptr<ExpressionNode>;

    struct StatementNode;
    struct PrintStatement;
    struct AssignmentStatement;
    struct ExpressionStatement;
    struct FunctionDefStatement;
    struct ReturnStatement;
    struct IfStatement;
    struct ElseStatement;
    struct ElifStatement;
    using StatementNodePtr = std::shared_ptr<StatementNode>;

    struct ExpressionVisitor {
        virtual void print(BinaryExpression *bexpr) = 0;
        virtual void print(ConstantExpression *nexpr) = 0;
        virtual void print(IdentifierExpression *nexpr) = 0;
        virtual void print(FunctionCallExpression *nexpr) = 0;
    };

    struct ExpressionEvaluatorVisitor {
        virtual void evaluate(ConstantExpression *expr) = 0;
        virtual void evaluate(BinaryExpression *expr) = 0;
        virtual void evaluate(IdentifierExpression *expr) = 0;
        virtual void evaluate(FunctionCallExpression *expr) = 0;
    };

    struct ExpressionNode {
        virtual void accept_evaluator(ExpressionEvaluatorVisitor *acceptor) = 0;
        virtual void accept(ExpressionVisitor *acceptor) = 0;
    };

    struct ConstantExpression : public ExpressionNode {
    public:
        ConstantExpression(Token const &n);

        Token const &get_constant() { return constant; }
        void accept_evaluator(ExpressionEvaluatorVisitor *acceptor) override;
        void accept(ExpressionVisitor *acceptor) override;
    private:
        Token constant;
    };

    struct IdentifierExpression : public ExpressionNode {
    public:
        IdentifierExpression(Token const &ident);

        Token const &get_ident() { return identifier; }
        void accept_evaluator(ExpressionEvaluatorVisitor *acceptor) override;
        void accept(ExpressionVisitor *acceptor) override;
    private:
        Token identifier;
    };

    struct FunctionCallExpression : public ExpressionNode {
    public:
        FunctionCallExpression(Token const &n, vector<ExpressionNodePtr> &&args);

        Token const &get_name() { return name; }
        vector<ExpressionNodePtr> const &get_args() { return arguments; }
        void accept_evaluator(ExpressionEvaluatorVisitor *acceptor) override;
        void accept(ExpressionVisitor *acceptor) override;
    private:
        Token name;
        vector<ExpressionNodePtr> arguments;
    };

    struct BinaryExpression : public ExpressionNode {
    public:
        BinaryExpression(TokenKind kind, ExpressionNodePtr lhs, ExpressionNodePtr rhs);

        ExpressionNodePtr  get_left() { return left; }
        ExpressionNodePtr  get_right() { return right; }
        TokenKind get_op() { return op; }

        void accept_evaluator(ExpressionEvaluatorVisitor *acceptor) override;
        void accept(ExpressionVisitor *acceptor) override;
    private:
        TokenKind op; // Operator
        ExpressionNodePtr left;
        ExpressionNodePtr right;
    };

    struct StatementVisitor {
        virtual void print(PrintStatement *stmt) = 0;
        virtual void print(AssignmentStatement *stmt) = 0;
        virtual void print(ExpressionStatement *stmt) = 0;
        virtual void print(FunctionDefStatement *stmt) = 0;
        virtual void print(ReturnStatement *stmt) = 0;
        virtual void print(IfStatement *stmt) = 0;
        virtual void print(ElseStatement *stmt) = 0;
        virtual void print(ElifStatement *stmt) = 0;
    };

    struct StatementEvaluatorVisitor {
        virtual void evaluate(PrintStatement *stmt) = 0;
        virtual void evaluate(ExpressionStatement *stmt) = 0;
        virtual void evaluate(ReturnStatement *stmt) = 0;
        virtual void evaluate(IfStatement *stmt) = 0;
        virtual void evaluate(ElseStatement *stmt) = 0;
        virtual void evaluate(ElifStatement *stmt) = 0;
        virtual void evaluate(AssignmentStatement *stmt) = 0;
        /*virtual void evaluate(FunctionDefStatement *stmt) = 0;*/
    };

    enum StatementKind {
        s_fdef,
        s_ret,
        s_assign,
        s_expr,
        s_if,
        s_else,
        s_elif,
    };

    struct StatementNode {
        virtual StatementKind get_type() = 0;
        virtual ~StatementNode() = default;
        virtual void accept_evaluator(StatementEvaluatorVisitor *acceptor) = 0;
        virtual void accept(StatementVisitor *acceptor) = 0;
    };

    struct PrintStatement : public StatementNode {
    public:
        PrintStatement(ExpressionNodePtr exp);

        ExpressionNodePtr const &get_expr() { return expr; }
        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_expr; }
    private:
        ExpressionNodePtr expr;
    };

    struct AssignmentStatement : public StatementNode {
    public:
        AssignmentStatement(std::shared_ptr<IdentifierExpression> lhs, ExpressionNodePtr rhs);

        std::shared_ptr<IdentifierExpression> const &get_left() { return left; }
        ExpressionNodePtr const &get_right() { return right; }
        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_assign; }
    private:
        std::shared_ptr<IdentifierExpression> left;
        ExpressionNodePtr right;
    };

    struct ExpressionStatement : public StatementNode {
    public:
        ExpressionStatement(ExpressionNodePtr expr);

        ExpressionNodePtr const &get_expr() { return expr; }
        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_expr; }
    private:
        ExpressionNodePtr expr;
    };

    struct FunctionDefStatement : public StatementNode {
    public:
        FunctionDefStatement(Token const &name, vector<ExpressionNodePtr> &&params, vector<StatementNodePtr> &&bd);

        Token const &get_name() { return name; }
        vector<ExpressionNodePtr> const &get_params() { return parameters; }
        vector<StatementNodePtr> const &get_body() { return body; }
        void set_return_statement(bool v) { return_statement = v; }
        bool has_return_statement() { return return_statement; }

        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_fdef; }
    private:
        Token name;
        vector<ExpressionNodePtr> parameters;
        vector<StatementNodePtr> body;
        bool return_statement{false};
    };

    struct ReturnStatement : public StatementNode {
    public:
        explicit ReturnStatement(ExpressionNodePtr exp);

        ExpressionNodePtr const &get_Expr() { return expr; }

        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_ret; }
    private:
        ExpressionNodePtr expr;
    };

    struct IfStatement : public StatementNode {
    public:
        IfStatement(ExpressionNodePtr cond, vector<StatementNodePtr> &&bd, vector<StatementNodePtr> &&elif);

        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_if; }
        ExpressionNodePtr const &get_condition() { return condition; }
        vector<StatementNodePtr> const &get_body() { return body; }
        vector<StatementNodePtr> const &get_elifs() { return elifs; }
    private:
        ExpressionNodePtr condition;
        vector<StatementNodePtr> body;
        vector<StatementNodePtr> elifs;
    };

    struct ElseStatement : public StatementNode {
    public:
        ElseStatement(vector<StatementNodePtr> &&bd);

        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_if; }
        vector<StatementNodePtr> const &get_body() { return body; }
    private:
        vector<StatementNodePtr> body;
    };

    struct ElifStatement : public StatementNode {
    public:
        ElifStatement(ExpressionNodePtr cond, vector<StatementNodePtr> &&bd);

        void accept(StatementVisitor *acceptor) override;
        void accept_evaluator(StatementEvaluatorVisitor *acceptor) override;
        StatementKind get_type() override { return s_if; }
        ExpressionNodePtr const &get_condition() { return condition; }
        vector<StatementNodePtr> const &get_body() { return body; }
    private:
        ExpressionNodePtr condition;
        vector<StatementNodePtr> body;
    };

} // step

#endif //STEP_AST_H
