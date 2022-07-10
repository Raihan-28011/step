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
    struct NumberExpression;
    using ExpressionNodePtr = std::shared_ptr<ExpressionNode>;

    struct AstNode {
    protected:
#ifdef __DEBUG_STEP__
        virtual void accept() = 0;
#endif
    };

    struct ExpressionPrintVisiter {
        virtual void print(BinaryExpression *bexpr) = 0;
        virtual void print(NumberExpression *nexpr) = 0;
    };

    struct ExpressionNode {
        virtual void accept(ExpressionPrintVisiter *acceptor) = 0;
    };

    struct NumberExpression : public ExpressionNode {
    public:
        NumberExpression(Token const &n);

        Token const &get_num() { return num; }
#ifdef __DEBUG_STEP__
        void accept(ExpressionPrintVisiter *acceptor) override;
#endif
    private:
        Token num;
    };

    struct BinaryExpression : public ExpressionNode {
    public:
        BinaryExpression(TokenKind kind, ExpressionNodePtr lhs, ExpressionNodePtr rhs);

        ExpressionNodePtr  get_left() { return left; }
        ExpressionNodePtr  get_right() { return right; }
        TokenKind get_op() { return op; }
#ifdef __DEBUG_STEP__
        void accept(ExpressionPrintVisiter *acceptor) override;
#endif
    private:
        TokenKind op; // Operator
        ExpressionNodePtr left;
        ExpressionNodePtr right;
    };
} // step

#endif //STEP_AST_H