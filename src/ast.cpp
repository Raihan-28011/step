//
// Created by raihan on 7/10/22.
//

#include "ast.h"

namespace step {
#ifdef __DEBUG_STEP__
    void BinaryExpression::accept(ExpressionPrintVisiter *acceptor) {
        acceptor->print(this);
    }

    BinaryExpression::BinaryExpression(TokenKind kind, ExpressionNodePtr lhs, ExpressionNodePtr rhs)
        : op{kind}, left{lhs}, right{rhs}
    {
    }

#endif

    NumberExpression::NumberExpression(Token const &n)
        : num{n}
    {
    }

    void NumberExpression::accept(ExpressionPrintVisiter *acceptor) {
        acceptor->print(this);
    }
} // step