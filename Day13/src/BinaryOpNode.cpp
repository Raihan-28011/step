/*
 * created by: shunnobir
 * date: 29/07/2024
 */

#include "BinaryOpNode.hpp"
#include "IAst.hpp"
#include "Lexeme.hpp"
#include "vm/OpCode.hpp"
#include "vm/VirtualMachine.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

Step::BinaryOpNode::BinaryOpNode(std::unique_ptr<Step::IAst> left,
                                     std::unique_ptr<Step::IAst> right,
                                     Step::LexemeKind op)
    : _left{std::move(left)},
      _right{std::move(right)},
      _op{op}
{
}

void Step::BinaryOpNode::set_return_type() {
    auto _lrt = _left->_return_type;
    auto _rrt = _right->_return_type;

    if ((_op != LexemeKind::logical_or && 
         _op != LexemeKind::logical_and) &&
        (_lrt == AstReturnType::BOOLEAN ||
        _rrt == AstReturnType::BOOLEAN)) {
        // Error: invaid operation on boolean
    }
    if ((_op == LexemeKind::logical_or || 
         _op == LexemeKind::logical_and) &&
        (_lrt != AstReturnType::BOOLEAN ||
        _rrt != AstReturnType::BOOLEAN)) {
        // Error: invaid operands; || and && are defined for only booleans
    }

    if (_op == LexemeKind::logical_and ||
        _op == LexemeKind::logical_or ||
        _op == LexemeKind::less_than ||
        _op == LexemeKind::greater_than ||
        _op == LexemeKind::less_than_equal ||
        _op == LexemeKind::greater_than_equal ||
        _op == LexemeKind::equal_equal ||
        _op == LexemeKind::not_equal) {
        _return_type = AstReturnType::BOOLEAN;
    } else if (_lrt == _rrt) {
        _return_type = _lrt;
    } else {
        if (_lrt == AstReturnType::DOUBLE || _rrt == AstReturnType::DOUBLE) {
            _return_type = AstReturnType::DOUBLE;
        } else {
            _return_type = AstReturnType::INTEGER;
        }
    }
}

std::string Step::BinaryOpNode::to_string(int indent) const {
    std::ostringstream out;
    out << std::string(indent * 4, ' ') << "(BinaryOp\n"
        << _left->to_string(indent+1)
        << std::string((indent+1) * 4, ' ') << Step::to_string(_op) << '\n'
        << _right->to_string(indent+1)
        << std::string(indent * 4, ' ') << "BinaryOp)\n";
    return out.str();
}

void Step::BinaryOpNode::emit() {
    _left->emit();
    if (_right->_return_type == AstReturnType::DOUBLE && _left->_return_type == AstReturnType::INTEGER) {
        StackVirtualMachine::instance().push_back(Step::OpCode::itof);    
    }
    _right->emit();
    if (_left->_return_type == AstReturnType::DOUBLE && _right->_return_type == AstReturnType::INTEGER) {
        StackVirtualMachine::instance().push_back(Step::OpCode::itof);    
    }

    auto _lrt = _left->_return_type;
    auto _rrt = _right->_return_type;
    AstReturnType _stack_top_operand_types;
    if (_lrt == _rrt) {
        _stack_top_operand_types = _lrt;
    } else {
        if (_lrt == AstReturnType::DOUBLE || _rrt == AstReturnType::DOUBLE) {
            _stack_top_operand_types  = AstReturnType::DOUBLE;
        } else {
            _stack_top_operand_types  = AstReturnType::INTEGER;
        }
    }

    auto &vm = StackVirtualMachine::instance();
    switch (_op) {
        case Step::LexemeKind::plus:
            vm.push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::iadd : Step::OpCode::fadd);
            break;
        case Step::LexemeKind::hyphen:
            vm.push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::isub : Step::OpCode::fsub);
            break;
        case Step::LexemeKind::star:
            vm.push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::imul : Step::OpCode::fmul);
            break;
        case Step::LexemeKind::slash:
            vm.push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::idiv : Step::OpCode::fdiv);
            break;
        case Step::LexemeKind::modulous:
            vm.push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::imod : Step::OpCode::fmod);
            break;
        case Step::LexemeKind::less_than:
            vm.push_back(_stack_top_operand_types  == AstReturnType::INTEGER ? Step::OpCode::icmp : Step::OpCode::fcmp);
            vm.push_back(Step::OpCode::pushtn);
            break;
        case Step::LexemeKind::less_than_equal:
            vm.push_back(_stack_top_operand_types  == AstReturnType::INTEGER ? Step::OpCode::icmp : Step::OpCode::fcmp);
            vm.push_back(Step::OpCode::pushfp);
            break;
        case Step::LexemeKind::greater_than:
            vm.push_back(_stack_top_operand_types  == AstReturnType::INTEGER ? Step::OpCode::icmp : Step::OpCode::fcmp);
            vm.push_back(Step::OpCode::pushtp);
            break;
        case Step::LexemeKind::greater_than_equal:
            vm.push_back(_stack_top_operand_types  == AstReturnType::INTEGER ? Step::OpCode::icmp : Step::OpCode::fcmp);
            vm.push_back(Step::OpCode::pushfn);
            break;
        case Step::LexemeKind::equal_equal:
            vm.push_back(_stack_top_operand_types  == AstReturnType::INTEGER ? Step::OpCode::icmp : Step::OpCode::fcmp);
            vm.push_back(Step::OpCode::pushtz);
            break;
        case Step::LexemeKind::not_equal:
            vm.push_back(_stack_top_operand_types  == AstReturnType::INTEGER ? Step::OpCode::icmp : Step::OpCode::fcmp);
            vm.push_back(Step::OpCode::pushfz);
            break;
        default:
            break;
    } 
}
