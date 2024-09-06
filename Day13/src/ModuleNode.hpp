/*
 * created by: shunnobir
 * date: 05/08/2024
 */

#ifndef __MODULE_NODE_HPP__
#define __MODULE_NODE_HPP__

#include "IAst.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Step {

class ModuleNode: public IAst {
    public:
        ModuleNode(std::vector<std::unique_ptr<IAst>> &&stmts);
        virtual ~ModuleNode() = default;
        virtual std::string to_string(int indent) const override;
        virtual void emit() override;
    private:
        std::vector<std::unique_ptr<IAst>> _stmts;
};

} // namespace Step

#endif // __MODULE_NODE_HPP__
