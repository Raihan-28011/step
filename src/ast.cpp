//
// Created by raihan on 7/10/22.
//

#include "ast.h"

namespace step {
    void BinaryExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    BinaryExpression::BinaryExpression(TokenKind kind, ExpressionNodePtr lhs, ExpressionNodePtr rhs)
        : op{kind}, left{lhs}, right{rhs}
    {
    }

    ConstantExpression::ConstantExpression(Token const &n)
        : constant{n}
    {
    }

    void ConstantExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    PrintStatement::PrintStatement(ExpressionNodePtr exp)
        : expr{exp}
    {
    }

    void PrintStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    AssignmentStatement::AssignmentStatement(ExpressionNodePtr lhs, ExpressionNodePtr rhs)
        : left{lhs}, right{rhs}
    {
    }

    void AssignmentStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    ExpressionStatement::ExpressionStatement(ExpressionNodePtr exp)
        : expr{exp}
    {
    }

    void ExpressionStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    FunctionCallExpression::FunctionCallExpression(Token const &n, vector<ExpressionNodePtr> &&args)
        : name{n}, arguments{std::move(args)}
    {
    }

    void FunctionCallExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    FunctionDefStatement::FunctionDefStatement(Token const &n, vector<ExpressionNodePtr> &&params,
                                               vector<StatementNodePtr> &&bd)
        : name{n}, parameters{std::move(params)}, body{std::move(bd)}
    {
    }

    void FunctionDefStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }
} // step