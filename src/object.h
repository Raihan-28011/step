//
// Created by raihan on 7/12/22.
//


#pragma once

#ifndef STEP_OBJECT_H
#define STEP_OBJECT_H

#include "common.h"
#include "ast.h"

namespace step {

    enum ObjectDataType {
        dt_int,
        dt_float,
        dt_string,
        dt_bool,
        dt_class,
        dt_object,
        dt_parameter,
        dt_function,
        dt_arguments,
        dt_callable,
        dt_identifier,
        dt_array,
    };

    struct Argument;
    struct Boolean;

    struct Object {
        using ref_t = Object *;
        using smart_ref_t = ref_t;
        using args_ref_t = Argument *;
        using size_t = u64;
        using ref_count_t = size_t;

        explicit Object(ObjectDataType t)
            : type{t} { }

        virtual ~Object() = default;

        void inc_refcount() { ++refcount; }
        void dec_refcount() { (refcount > 0 ? --refcount : refcount); }
        ref_count_t get_refcount() { return refcount; }
        ObjectDataType get_type() const { return type; }
        virtual Object::size_t print(std::ostream &os) const = 0;
        virtual smart_ref_t call_object_specific_method(string name, args_ref_t args) = 0;

        virtual bool is_function() { return get_type() == dt_function; }
        virtual bool is_array() { return get_type() == dt_array; }
    protected:
        ObjectDataType type;
        ref_t _this{this};
        ref_count_t refcount{1};
    };

    struct Argument : public Object {
        using ref_t = Argument *;
        using arg_t = Object *;
        using index_t = u64;

    public:
        Argument()
                : Object{dt_arguments}
        {
        }

        Argument(std::initializer_list<arg_t> args);
        ~Argument() override {
            for (auto &i: args) {
                i->dec_refcount();
                if (i->get_refcount() == 0)
                    delete i;
            }
        }

        arg_t get_arg(index_t index);
        Object::size_t print(std::ostream &os) const override;
        Object::smart_ref_t call_object_specific_method(string name, Object::args_ref_t args) override;
    private:
        vector<arg_t> args;
    };

    struct String;
    struct StringMethods {
        using binary_operation = String *(String::*)(Argument::ref_t);

        umap<string, binary_operation> b_methods;
    };

    struct String : public Object {
        using size_t = u64;
        using index_t = u64;
        using content_t = char;
        using ref_t = char *;
        using smart_ref_t = String *;
        using str_methods_t = StringMethods *;

        constexpr static index_t invalid_index = index_t(-1);

        /* constructors */
        String();
        explicit String(std::string const &cont);
        String(step::String const &cp);
        explicit String(double num);
        explicit String(i64 num);

        /* destructor */
        ~String() override {
            delete[] content;
            delete op_methods;
        }

        /* copy constructor */
        String &operator=(std::string const &cp);
        String &operator=(step::String const &cp);
        bool operator==(step::String const &cp) const;
        bool operator!=(step::String const &cp) const;
        bool operator<(step::String const &cp) const;
        bool operator>(step::String const &cp) const;
        content_t operator[](index_t index) const { return *(_beg + index); }

        Object::size_t print(std::ostream &os) const override;
        content_t at(index_t index) const;
        size_t size() const { return size_t(_end - _beg); }
        size_t len() const { return size_t(_end - _beg); }
        size_t capacity() const;
        void add(content_t c);
        void add_beg(content_t c);
        void add(String const &cp);
        bool empty() const { return _beg == _end; }
        index_t find(content_t c) const;

        Object::smart_ref_t call_object_specific_method(string name, Object::args_ref_t args) override;

        ref_t begin() const { return _beg; }
        ref_t end() const { return _end; }

        static double stod(String const &num);
        static i64 stoi(String const &num);
    private:
        ref_t content = new char[4] { '\0', '\0', '\0', '\0' };
        ref_t _beg{content};
        ref_t _end{content};
        size_t cap{4};
        str_methods_t op_methods;

        void resize(size_t cap, bool cp);
        String::smart_ref_t concate_wrapper(Argument::ref_t args);
        String::smart_ref_t concate(String::smart_ref_t args);
    };

    struct Function : public Object {
        using func_t = FunctionDefStatement *;
    public:
        Function();
        explicit Function(func_t func);
        ~Function();

        string const &get_name() { return func->get_name().get_tok(); }
        func_t &get_logic() { return func; }
        Object::size_t print(std::ostream &os) const override;
        Object::smart_ref_t call_object_specific_method(string name, Object::args_ref_t args) override;
    private:
        func_t func;
    };

    struct Integer;
    struct NumberMethods {
        using binary_operation = Integer *(Integer::*)(Argument::ref_t);
        using bool_bin_op = Boolean *(Integer::*)(Argument::ref_t);

        umap<string, binary_operation> b_methods;
        umap<string, bool_bin_op> bool_b_methods;
    };


    struct Integer : public Object {
#ifdef __BIGNUM_IMPLEMENTED__
        using num_t = step::String;
#else
        using num_t = i64;
#endif
        using smart_ref_t = Integer *;
        using smart_bool_ref_t = Boolean *;
        using num_methods_t = NumberMethods *;
        using mask_t = u64;

        constexpr static mask_t HEX = (1 << 1);
        constexpr static mask_t OCT = (1 << 2);
        constexpr static mask_t BIN = (1 << 3);

        /* constructors */
        Integer();

        explicit Integer(std::string const &num);
        explicit Integer(step::String const &num);

        /* destructors */
        ~Integer() override {
            delete op_methods;
        }

        Object::size_t print(std::ostream &os) const override;
        Object::smart_ref_t call_object_specific_method(string name, Object::args_ref_t args) override;

        String to_string() const;
        Integer const &to_decimal() const;
        Integer const &to_hex() const;
        Integer const &to_oct() const;
        Integer const &to_bin() const;
        Integer const &from_hex_to_dec() const;
        Integer const &from_oct_to_dec() const;
        Integer const &from_bin_to_dec() const;
        num_t get_num() const { return _num; };

        bool operator==(Integer const &b) const;
        bool operator!=(Integer const &b) const;
        bool operator<(Integer const &b) const;
        bool operator>(Integer const &b) const;
        bool operator<=(Integer const &b) const;
        bool operator>=(Integer const &b) const;
    private:
        num_t _num;
        num_methods_t op_methods;
        mask_t mask{0};

        void set_masks(step::String const &num);

        Integer::smart_ref_t add_int_wrapper(Argument::ref_t args);
        Integer::smart_ref_t add_int(Integer::smart_ref_t b);
        Integer::smart_ref_t mult_int_wrapper(Argument::ref_t args);
        Integer::smart_ref_t mult_int(Integer::smart_ref_t b);
        Integer::smart_ref_t pow_int_wrapper(Argument::ref_t args);
        Integer::smart_ref_t pow_int(Integer::smart_ref_t b);
        Integer::smart_ref_t sub_int_wrapper(Argument::ref_t args);
        Integer::smart_ref_t sub_int(Integer::smart_ref_t b);
        Integer::smart_ref_t div_int_wrapper(Argument::ref_t args);
        Integer::smart_ref_t div_int(Integer::smart_ref_t b);
        Integer::smart_ref_t mod_int_wrapper(Argument::ref_t args);
        Integer::smart_ref_t mod_int(Integer::smart_ref_t b);
        Integer::smart_bool_ref_t eq_int_wrapper(Argument::ref_t args);
        Integer::smart_bool_ref_t eq_int(Integer::smart_ref_t b);
        Integer::smart_bool_ref_t neq_int_wrapper(Argument::ref_t args);
        Integer::smart_bool_ref_t neq_int(Integer::smart_ref_t b);
        Integer::smart_bool_ref_t lt_int_wrapper(Argument::ref_t args);
        Integer::smart_bool_ref_t lt_int(Integer::smart_ref_t b);
        Integer::smart_bool_ref_t lteq_int_wrapper(Argument::ref_t args);
        Integer::smart_bool_ref_t lteq_int(Integer::smart_ref_t b);
        Integer::smart_bool_ref_t gt_int_wrapper(Argument::ref_t args);
        Integer::smart_bool_ref_t gt_int(Integer::smart_ref_t b);
        Integer::smart_bool_ref_t gteq_int_wrapper(Argument::ref_t args);
        Integer::smart_bool_ref_t gteq_int(Integer::smart_ref_t b);
    };

    struct Boolean : public Object {
        using bool_t = bool;
    public:
        explicit Boolean(bool_t val);

        bool_t get_value() { return value; }
        Object::size_t print(std::ostream &os) const override;
        Object::smart_ref_t call_object_specific_method(string name, args_ref_t args) override;
    private:
        bool_t value;
    };

    struct Variable : public Object {
        using value_t = Object *;
    public:
        Variable();
        Variable(value_t obj);
        ~Variable() override {
            value->dec_refcount();
            if (value->get_refcount() == 0)
                delete value;
        }
         
        value_t const &get_value() { value->inc_refcount(); return value; }
        void set_new_value(value_t val);
        Object::size_t print(std::ostream &os) const override;
        Object::smart_ref_t call_object_specific_method(string name, args_ref_t args) override;
        bool is_function() override { return value->is_function(); }
        bool is_array() override { return value->is_array(); }
    private:
        value_t value;
        /*bool is_const = false;  */
    };


    struct Array;
    struct ArrayMethods {
        using self_ref_func_t = Object *(Array::*)(Argument::ref_t);

        umap<string, self_ref_func_t> self_methods;
    };

    struct Array : public Object {
        using elem_t = Object *;
        using ref_t = Object *;
        using self_ref_t = Array *;
        using index_t = Integer *;
        using size_t = u64;
    public:
        Array();
        ~Array() override {
            for (auto &i: elements) {
                i->dec_refcount();
                if (i->get_refcount() == 0)
                    delete i;
            }
        }

        Object::size_t print(std::ostream &os) const override;
        Object::smart_ref_t call_object_specific_method(string name, args_ref_t args) override;
        Array::ref_t append_wrapper(Argument::ref_t args); 
        Array::self_ref_t append(elem_t elem); 
        Array::ref_t at_wrapper(Argument::ref_t args); 
        Array::ref_t at(index_t index); 
        Array::ref_t set_wrapper(Argument::ref_t args); 
        Array::ref_t set(index_t index, elem_t elem); 
        size_t get_size() { return elements.size(); }
    private:
        vector<elem_t> elements;
        ArrayMethods methods;
    };

} // step

#endif //STEP_OBJECT_H
