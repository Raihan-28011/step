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

    void BinaryExpression::accept_evaluator(ExpressionEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    ConstantExpression::ConstantExpression(Token const &n)
        : constant{n}
    {
    }

    void ConstantExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    void ConstantExpression::accept_evaluator(ExpressionEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    PrintStatement::PrintStatement(ExpressionNodePtr exp)
        : expr{exp}
    {
    }

    void PrintStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void PrintStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    AssignmentStatement::AssignmentStatement(std::shared_ptr<IdentifierExpression> lhs, ExpressionNodePtr rhs)
        : left{lhs}, right{rhs}
    {
    }

    void AssignmentStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void AssignmentStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    ExpressionStatement::ExpressionStatement(ExpressionNodePtr exp)
        : expr{exp}
    {
    }

    void ExpressionStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void ExpressionStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    FunctionCallExpression::FunctionCallExpression(Token const &n, vector<ExpressionNodePtr> &&args)
        : name{n}, arguments{std::move(args)}
    {
    }

    void FunctionCallExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    void FunctionCallExpression::accept_evaluator(ExpressionEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    FunctionDefStatement::FunctionDefStatement(Token const &n, vector<ExpressionNodePtr> &&params,
                                               vector<StatementNodePtr> &&bd)
        : name{n}, parameters{std::move(params)}, body{std::move(bd)}, return_statement{false}
    {
    }

    void FunctionDefStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void FunctionDefStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        (void)acceptor;
    }

    IdentifierExpression::IdentifierExpression(Token const &ident)
        : identifier{ident}
    {
    }

    void IdentifierExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    void IdentifierExpression::accept_evaluator(ExpressionEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    ReturnStatement::ReturnStatement(ExpressionNodePtr exp)
        : expr{exp}
    {
    }

    void ReturnStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void ReturnStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    IfStatement::IfStatement(ExpressionNodePtr cond, vector<StatementNodePtr> &&bd, vector<StatementNodePtr> &&elif)
        : condition{cond}, body{std::move(bd)}, elifs{std::move(elif)}
    {
    }

    void IfStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void IfStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    ElseStatement::ElseStatement(vector<StatementNodePtr> &&bd)
        : body{std::move(bd)}
    {
    }

    void ElseStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void ElseStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    ElifStatement::ElifStatement(ExpressionNodePtr cond, vector<StatementNodePtr> &&bd)
        : condition{cond}, body{std::move(bd)}
    {
    }

    void ElifStatement::accept(StatementVisitor *acceptor) {
        acceptor->print(this);
    }

    void ElifStatement::accept_evaluator(StatementEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }

    ArrayExpression::ArrayExpression(vector<ExpressionNodePtr> &&elems)
        : elements{std::move(elems)}
    {
    }

    void ArrayExpression::accept(ExpressionVisitor *acceptor) {
        acceptor->print(this);
    }

    void ArrayExpression::accept_evaluator(ExpressionEvaluatorVisitor *acceptor) {
        acceptor->evaluate(this);
    }
} // step
