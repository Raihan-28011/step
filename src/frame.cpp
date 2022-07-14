//
// Created by raihan on 7/12/22.
//

#include "frame.h"
#include "error.h"

namespace step {
    void Frame::push(Frame::ref_t obj) {
        eval_stack.push(obj);
    }

    Frame::ref_t Frame::pop() {
        ref_t const &ret = eval_stack.top();
        eval_stack.pop();
        return ret;
    }

    Frame::ref_t const &Frame::top() {
        return eval_stack.top();
    }

    bool Frame::is_defined_builtin(string const &name) {
        return _builtin_functions_[name];
    }

    void Frame::add_function(Frame::func_ref_t func) {
        if (_defined_functions_.find(func->get_name()) != _defined_functions_.end()) {
            errorManager->runtime_error("multiple definition of function '" + func->get_name() + "'", *this);
        }

        _defined_functions_[func->get_name()] = func;
    }

    Frame::func_ref_t Frame::get_function(string const &name) {
        if (_defined_functions_.find(name) != _defined_functions_.end())
            return _defined_functions_[name];
        return nullptr;
    }

    bool Frame::is_user_defined(string const &name) {
        return _defined_functions_.find(name) != _defined_functions_.end();
    }
} // step