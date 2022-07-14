//
// Created by raihan on 7/12/22.
//


#pragma once

#ifndef STEP_FRAME_H
#define STEP_FRAME_H

#include "object.h"

namespace step {

    class Frame {
    public:
        using ref_t = step::Object *;
        using func_ref_t = step::Function *;
    public:
        Frame() = default;
        ~Frame() {
            for (auto &i: _defined_functions_)
                delete i.second;
        }

        void push(ref_t obj);
        ref_t pop();
        ref_t const &top();
        bool is_defined_builtin(string const &name);
        bool is_user_defined(string const &name);
        void add_function(Frame::func_ref_t func);
        Frame::func_ref_t get_function(string const &name);
    private:
        stack<ref_t> eval_stack;
        umap<string, bool> _builtin_functions_ {
                {"print", true}
        };
        umap<string, func_ref_t> _defined_functions_;
    };

} // step

#endif //STEP_FRAME_H