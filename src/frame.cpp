//
// Created by raihan on 7/12/22.
//

#include "frame.h"
#include "error.h"

namespace step {
    void Frame::push(Frame::ref_t obj) {
        eval_stack.push_back(obj);
    }

    Frame::ref_t Frame::pop() {
        ref_t const &ret = eval_stack.back();
        ret->dec_refcount();
        eval_stack.pop_back();
        return ret;
    }

    Frame::ref_t const &Frame::top() {
        eval_stack.back()->inc_refcount();
        return eval_stack.back();
    }

    Frame::ref_t const &Frame::at(Frame::size_t index) {
        eval_stack.at(index)->inc_refcount();
        return eval_stack.at(index);
    }

    bool Frame::is_defined_builtin(string const &name) {
        return _builtin_functions_[name];
    }

    bool Frame::is_defined_variable(string const &name) {
        return _defined_variables_.find(name) != _defined_variables_.end();
    }

    void Frame::add_function(Frame::func_ref_t func) {
        auto &tok = func->get_logic()->get_name();
        if (is_defined_builtin(func->get_name())) {
            errorManager->compilation_error("trying to redefine builtin function '" + func->get_name() + "'", tok.get_line(), tok.get_col());
        }

        if (is_user_defined(func->get_name())) {
            errorManager->compilation_error("multiple definition of function '" + func->get_name() + "'", tok.get_line(), tok.get_col());
        }

//        func->inc_refcount();
        _defined_functions_[func->get_name()] = func;
    }

    Frame::func_ref_t Frame::get_function(string const &name) {
        if (_defined_functions_.find(name) != _defined_functions_.end()) {
            _defined_functions_[name]->inc_refcount();
            return _defined_functions_[name];
        }
        return nullptr;
    }

    Frame::size_t Frame::get_variable(const string &name) {
        return _defined_variables_[name];
    }

    bool Frame::is_user_defined(string const &name) {
        return _defined_functions_.find(name) != _defined_functions_.end();
    }
    
    void Frame::add_variable(const string &name, ref_t obj) {
        eval_stack.push_back(obj);
        _defined_variables_[name] = eval_stack.size()-1;
    }
    
    void Frame::set_at(ref_t obj, size_t index) {
        eval_stack.at(index) = obj;
    }
} // step
