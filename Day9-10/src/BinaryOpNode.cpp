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

Step::BinaryOpNode::BinaryOpNode(std::unique_ptr<Step::IAst> left,
                                     std::unique_ptr<Step::IAst> right,
                                     Step::LexemeKind op)
    : _left{std::move(left)},
      _right{std::move(right)},
      _op{op}
{
    auto _lrt = _left->_return_type;
    auto _rrt = _right->_return_type;

    if (_lrt == _rrt) {
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
    switch (_op) {
        case Step::LexemeKind::PLUS:
            StackVirtualMachine::instance().push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::iadd : Step::OpCode::fadd);
            break;
        case Step::LexemeKind::HYPHEN:
            StackVirtualMachine::instance().push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::isub : Step::OpCode::fsub);
            break;
        case Step::LexemeKind::STAR:
            StackVirtualMachine::instance().push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::imul : Step::OpCode::fmul);
            break;
        case Step::LexemeKind::SLASH:
            StackVirtualMachine::instance().push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::idiv : Step::OpCode::fdiv);
            break;
        case Step::LexemeKind::MODULOUS:
            StackVirtualMachine::instance().push_back(_return_type == AstReturnType::INTEGER ? Step::OpCode::imod : Step::OpCode::fmod);
            break;
        default:
            break;
    } 
}
