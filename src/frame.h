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
        using size_t = u64;
    public:
        Frame() = default;
        Frame(Frame const &fm);
        ~Frame() {
            for (auto &i: _defined_functions_) {
                i.second->dec_refcount();
                if (i.second->get_refcount() == 0)
                    delete i.second;
            }

            for (auto &i: eval_stack) {
                i->dec_refcount();
                if (i->get_refcount() == 0)
                    delete i;
            }
        }

        void push(ref_t obj);
        ref_t pop();
        ref_t const &top();
        ref_t const &at(size_t index);
        bool is_defined_builtin(string const &name);
        bool is_user_defined(string const &name);
        bool is_defined_variable(string const &name);
        void add_function(Frame::func_ref_t func);
        void add_variable(string const &name, ref_t obj);
        void add_variable(string const &name, size_t index);
        void set_at(ref_t obj, size_t index);
        Frame::func_ref_t get_function(string const &name);
        size_t get_variable(string const &name);
        void remove_variable(string const &name);
    private:
        vector<ref_t> eval_stack;
        umap<string, bool> _builtin_functions_ {
                {"print", true}
        };
        umap<string, func_ref_t> _defined_functions_{};
        umap<string, size_t> _defined_variables_{};
    };

} // step

#endif //STEP_FRAME_H
