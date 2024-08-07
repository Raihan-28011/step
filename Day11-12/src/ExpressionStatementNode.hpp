/*
 * created by: shunnobir
 * date: 05/08/2024
 */

#ifndef __EXPRESSION_STATEMENT_NODE_HPP__
#define __EXPRESSION_STATEMENT_NODE_HPP__

#include "IAst.hpp"
#include <memory>
namespace Step {

class ExpressionStatementNode: public IAst {
    public:
        explicit ExpressionStatementNode(std::unique_ptr<IAst> expr);
        virtual ~ExpressionStatementNode() override = default;
        virtual std::string to_string(int indent = 0) const override;
        virtual void emit() override;
    private:
        std::unique_ptr<IAst> _expr;
};

}

#endif // __EXPRESSION_STATEMENT_NODE_HPP__
