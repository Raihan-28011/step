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
    struct FunctionCallExpression;
    using ExpressionNodePtr = std::shared_ptr<ExpressionNode>;

    struct StatementNode;
    struct PrintStatement;
    struct AssignmentStatement;
    struct ExpressionStatement;
    struct FunctionDefStatement;
    using StatementNodePtr = std::shared_ptr<StatementNode>;

    struct ExpressionVisitor {
        virtual void print(BinaryExpression *bexpr) = 0;
        virtual void print(ConstantExpression *nexpr) = 0;
        virtual void print(FunctionCallExpression *nexpr) = 0;
    };

    struct ExpressionNode {
        virtual void accept(ExpressionVisitor *acceptor) = 0;
    };

    struct ConstantExpression : public ExpressionNode {
    public:
        ConstantExpression(Token const &n);

        Token const &get_constant() { return constant; }
        void accept(ExpressionVisitor *acceptor) override;
    private:
        Token constant;
    };

    struct FunctionCallExpression : public ExpressionNode {
    public:
        FunctionCallExpression(Token const &n, vector<ExpressionNodePtr> &&args);

        Token const &get_name() { return name; }
        vector<ExpressionNodePtr> const &get_args() { return arguments; }
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
    };

    struct StatementNode {
        virtual void accept(StatementVisitor *acceptor) = 0;
    };

    struct PrintStatement : public StatementNode {
    public:
        PrintStatement(ExpressionNodePtr exp);

        ExpressionNodePtr const &get_expr() { return expr; }
        void accept(StatementVisitor *acceptor) override;
    private:
        ExpressionNodePtr expr;
    };

    struct AssignmentStatement : public StatementNode {
    public:
        AssignmentStatement(ExpressionNodePtr lhs, ExpressionNodePtr rhs);

        ExpressionNodePtr const &get_left() { return left; }
        ExpressionNodePtr const &get_right() { return right; }
        void accept(StatementVisitor *acceptor) override;
    private:
        ExpressionNodePtr left;
        ExpressionNodePtr right;
    };

    struct ExpressionStatement : public StatementNode {
    public:
        ExpressionStatement(ExpressionNodePtr expr);

        ExpressionNodePtr const &get_expr() { return expr; }
        void accept(StatementVisitor *acceptor) override;
    private:
        ExpressionNodePtr expr;
    };

    struct FunctionDefStatement : public StatementNode {
    public:
        FunctionDefStatement(Token const &name, vector<ExpressionNodePtr> &&params, vector<StatementNodePtr> &&bd);

        Token const &get_name() { return name; }
        vector<ExpressionNodePtr> const &get_params() { return parameters; }
        vector<StatementNodePtr> const &get_body() { return body; }
        void accept(StatementVisitor *acceptor) override;
    private:
        Token name;
        vector<ExpressionNodePtr> parameters;
        vector<StatementNodePtr> body;
    };

} // step

#endif //STEP_AST_H