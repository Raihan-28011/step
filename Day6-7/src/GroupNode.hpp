/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#ifndef __GROUP_NODE_HPP__
#define __GROUP_NODE_HPP__

#include "IAst.hpp"
#include <memory>

namespace Step {
    class GroupNode: public IAst {
    public:
        explicit GroupNode(std::unique_ptr<IAst> expr);
        virtual ~GroupNode() override = default;
        virtual std::string to_string(int indent = 0) const override;
    private:
        std::unique_ptr<IAst> _expr;
    };
}

#endif // __GROUP_NODE_HPP__


