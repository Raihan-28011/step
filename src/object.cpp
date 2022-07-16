//
// Created by raihan on 7/12/22.
//

#include "object.h"
#include "error.h"

namespace step {

    /* -------- String ----------- */
    String::String()
            : Object{dt_string}
    {
        op_methods = new StringMethods{
            .b_methods = {
                {"add", &String::concate_wrapper}
            }
        };
    }


    String::String(std::string const &cont)
        : String()
    {
        resize(cont.length(), false);
        for (auto const &c: cont) {
            *_end = c;
            ++_end;
        }
        *_end = '\0';
    }

    String::String(step::String const &cp)
        : String()
    {
        resize(cp.len(), false);
        for (ref_t _b = cp.begin(); _b != cp.end(); ++_b) {
            *_end = *_b;
            ++_end;
        }
        *_end = '\0';
    }

    Object::size_t String::print(std::ostream &os) const {
        for (ref_t _b = begin(); _b != end(); ++_b) {
            os << *_b;
        }
        return len();
    }

    String::content_t String::at(String::index_t index) const {
        return (index >= size() ? '\0' : content[index]);
    }

    String::size_t String::capacity() const {
        return cap;
    }

    String &String::operator=(string const &cp) {
        resize(cp.length()+1, false);
        for (auto const &c: cp) {
            *_end = c;
            ++_end;
        }
        *_end = '\0';
        return *this;
    }

    String &String::operator=(String const &cp) {
        resize(cp.len()+1, false);
        for (ref_t _b = cp.begin(); _b != cp.end(); ++_b) {
            *_end = *_b;
            ++_end;
        }
        *_end = '\0';
        return *this;
    }

    void String::resize(size_t c, bool cp) {
        if (capacity() <= c) {
            this->cap = c*2;
            ref_t n = new content_t[capacity()];

            ref_t _f = n;
            if (cp) {
                ref_t _b = begin(), _l = end();
                while (_b != _l) {
                    *_f = char(*_b);
                    ++_f;
                    ++_b;
                }
            }

            delete[] content;
            content = n;
            _beg = content;
            _end = _f;
            *_end = '\0';
        }
    }

    void String::add(String::content_t c) {
        resize(len()+1, true);
        *_end = c;
        ++_end;
        *_end = '\0';
    }

    void String::add(String const &cp) {
        resize(len()+cp.len()+1, true);
        for (auto _b = cp.begin(); _b != cp.end(); ++_b) {
            *_end = *_b;
            ++_end;
        }
        *_end = '\0';
    }

    void String::add_beg(String::content_t c) {
        resize(len()+1, true);
        for (auto _b = end(); _b != begin(); --_b)
            *_b = *(_b-1);
        *_beg = c;
        ++_end;
        *_end = '\0';
    }

    Object::smart_ref_t String::call_object_specific_method(string name, Object::args_ref_t args) {
        if (op_methods->b_methods.find(name) != op_methods->b_methods.end())
            return (this->*op_methods->b_methods[name])(args);
        return nullptr;
    }

    String::index_t String::find(String::content_t c) const {
        auto _b = _beg;
        for (; *_b != c && _b != _end; ++_b)
            ;
        if (_b == _end)
            return invalid_index;
        return index_t(_b - _beg);
    }

    bool String::operator==(String const &cp) const {
        if (cp.len() != this->len())
            return false;

        for (auto _b = begin(), _b2 = cp.begin(); _b != end(); ++_b) {
            if (*_b != *_b2)
                return false;
        }

        return true;
    }

    bool String::operator!=(String const &cp) const {
        return !(*this == cp);
    }

    bool String::operator<(String const &cp) const {
        auto mn = std::min(len(), cp.len());
        auto _b = begin(), _c = cp.begin();
        for (; (_b - begin()) != mn && *_b == *_c; ++_b, ++_c)
            ;
        return ((_b - begin() == mn && len() < cp.len()) ||  (_b - begin() != mn && *_b < *_c));
    }

    bool String::operator>(String const &cp) const {
        return !(*this < cp);
    }

    String::String(double num)
        : String(std::to_string(num))
    {
    }

    String::String(i64 num)
        : String(std::to_string(num))
    {
    }

    double String::stod(String const &num) {
        return std::stod(string(num.content));
    }

    i64 String::stoi(String const &num) {
        return std::stoll(string(num.content));
    }

    String::smart_ref_t String::concate_wrapper(Argument::ref_t args) {
        return concate(dynamic_cast<String::smart_ref_t>(args->get_arg(0)));
    }

    String::smart_ref_t String::concate(String::smart_ref_t arg) {
        String::smart_ref_t ret = new String();
        for (auto &c: *this)
            ret->add(c);
        for (auto &c: *arg)
            ret->add(c);
        return ret;
    }

    /* ----------Integer--------- */

    Integer::Integer()
        : Object{dt_int}
    {
        op_methods = new NumberMethods {
            /*.addition = &Integer::add_int,*/
            .b_methods = {
                    {"add",  &Integer::add_int_wrapper},
                    {"mult", &Integer::mult_int_wrapper},
                    {"pow",  &Integer::pow_int_wrapper},
                    {"sub",  &Integer::sub_int_wrapper},
                    {"div",  &Integer::div_int_wrapper},
                    {"mod", &Integer::mod_int_wrapper},
            },
            .bool_b_methods = {
                    {"eq", &Integer::eq_int_wrapper},
                    {"neq", &Integer::neq_int_wrapper},
                    {"lt", &Integer::lt_int_wrapper},
                    {"gt", &Integer::gt_int_wrapper},
                    {"lteq", &Integer::lteq_int_wrapper},
                    {"gteq", &Integer::gteq_int_wrapper},
            }
        };
    }

    Integer::Integer(string const &num)
        : Integer()
    {
#ifdef __BIGNUM_IMPLEMENTED
        _num = num;
        set_masks(_num);
#else
        set_masks(step::String(num));
        _num = std::stoll(num);
#endif
    }

    Integer::Integer(String const &num)
        : Integer()
    {
#ifdef __BIGNUM_IMPLEMENTED
        _num = num;
        set_masks(_num);
#else
        set_masks(num);
        _num = step::String::stoi(num);
#endif
    }

    Object::size_t Integer::print(std::ostream &os) const {
        Object::size_t ret;
#ifdef __BIGNUM_IMPLEMENTED__
        _num.print(os);
        ret = _num.len();
#else
        os << _num;
        ret = to_string().len();
#endif
        return ret;
    }

    Integer::smart_ref_t Integer::add_int(Integer::smart_ref_t b) {
#ifdef __BIGNUM_IMPLEMENTED__
        auto lhs = _num, rhs = b->_num;
        auto len1 = lhs.len(), len2 = rhs.len();

        while (len2 != len1) {
            if (len1 > len2) {
                rhs.add_beg('0');
                ++len2;
            } else {
                lhs.add_beg('0');
                ++len1;
            }
        }

        String result;
        i64 carry = 0;
        for (auto _e = rhs.end()-1, _e2 = lhs.end()-1; _e >= rhs.begin(); --_e, --_e2) {
            i64 l_num = (*_e - '0') + (*_e2 - '0') + carry;
            carry = (l_num/10);
            result.add_beg(String::content_t(l_num%10 + '0'));
        }

        if (carry)
            result.add_beg(String::content_t(carry + '0'));

        return Integer::smart_ref_t(new Integer(result));
#else
        return Integer::smart_ref_t(new Integer(std::to_string(_num + b->_num)));
#endif
    }

    String Integer::to_string() const {
        return String(std::to_string(_num));
    }

    Object::smart_ref_t Integer::call_object_specific_method(string name, Object::args_ref_t args) {
        if (op_methods->b_methods.find(name) != op_methods->b_methods.end())
            return (this->*op_methods->b_methods[name])(args);
        else if (op_methods->bool_b_methods.find(name) != op_methods->bool_b_methods.end())
            return (this->*op_methods->bool_b_methods[name])(args);
        return nullptr;
    }

    Integer::smart_ref_t Integer::add_int_wrapper(Argument::ref_t args) {
        /*  add check for argument numbers */
        return add_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
        return nullptr;
    }

    Integer const &Integer::to_decimal() const {
        if (mask & HEX)
            return from_hex_to_dec();
        else if (mask & OCT)
            return from_oct_to_dec();
        else if (mask & BIN)
            return from_bin_to_dec();
        return *this;
    }

    Integer const &Integer::to_hex() const {
        return *this;
    }

    Integer const &Integer::to_oct() const {
        return *this;
    }

    Integer const &Integer::to_bin() const {
        return *this;
    }

    void Integer::set_masks(String const &num) {
        if (num.empty())
            return;

        if (num[0] == '0') {
            if (std::tolower(num[1]) == 'x')
                mask |= HEX;
            else if (std::tolower(num[1]) == 'b')
                mask |= BIN;
            else
                mask |= OCT;
        }
    }

    Integer const &Integer::from_hex_to_dec() const {
#ifdef __BIGNUM_IMPLEMENTED__
        // bignum code goes here
#else
        // code for integer goes here
#endif
        return *this;
    }

    Integer const &Integer::from_oct_to_dec() const {
        return *this;
    }

    Integer const &Integer::from_bin_to_dec() const {
        return *this;
    }

    Integer::smart_ref_t Integer::mult_int_wrapper(Argument::ref_t args) {
        return mult_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_ref_t Integer::mult_int(Integer::smart_ref_t b) {
#ifdef __BIGNUM__IMPLEMENTED__
        Integer::smart_ref_t ret = new Integer("0");

        Integer::smart_ref_t t1(new Integer("0")), t2(new Integer("1"));
        while (*t1 < *b) {
            auto g = ret->add_int(this);
            delete ret;
            ret = g;
            g = t1->add_int(t2); // Increment t1 till its equal to b
            delete t1;
            t1 = g;
        }
        return ret;
#else
        return Integer::smart_ref_t(new Integer(std::to_string(_num * b->_num)));
#endif
    }

    bool Integer::operator==(Integer const &b) const {
        return _num == b._num;
    }

    bool Integer::operator!=(Integer const &b) const {
        return _num != b._num;
    }

    Integer::smart_ref_t Integer::pow_int_wrapper(Argument::ref_t args) {
        return pow_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_ref_t Integer::pow_int(Integer::smart_ref_t b) {
#ifdef __BIGNUM__IMPLEMENTED__
        Integer::smart_ref_t ret = new Integer("1");

        Integer::smart_ref_t t1(new Integer("0")), t2(new Integer("1"));
        while (t1 != b) {
            auto g = t1->add_int(t2); // Increment t1 till its equal to b
            delete t1;
            t1 = g;
            g = this->mult_int(this);
            delete ret;
            ret = g;
        }
        return ret;
#else
        return Integer::smart_ref_t(new Integer(std::to_string(std::pow<i64>(_num, b->_num))));
#endif
    }

    bool Integer::operator<(Integer const &b) const {
#ifdef __BIGNUM_IMPLEMENTED__
        if (_num.len() < b._num.len())
            return true;
        else if (_num.len() == b._num.len()) {
            for (auto _b = _num.begin(), _b2 = b._num.begin(); _b != _num.end(); ++_b, ++_b2) {
                if (*_b < *_b2)
                    return true;
                if (*_b > *_b2)
                    return false;
            }
        }
        return false;
#else
        return _num < b._num;
#endif
    }

    bool Integer::operator>(Integer const &b) const {
#ifdef __BIGNUM__IMPLEMENTED__
        if (_num.len() > b._num.len())
            return true;
        else if (_num.len() == b._num.len()) {
            for (auto _b = _num.begin(), _b2 = b._num.begin(); _b != _num.end(); ++_b, ++_b2) {
                if (*_b > *_b2)
                    return true;
                if (*_b < *_b2)
                    return false;
            }
        }
        return false;
#else
        return _num > b._num;
#endif
    }

    bool Integer::operator<=(Integer const &b) const {
        return (_num < b._num) || (_num == b._num);
    }

    bool Integer::operator>=(Integer const &b) const {
        return (_num > b._num) || (_num == b._num);
    }

    Integer::smart_ref_t Integer::sub_int_wrapper(Argument::ref_t args) {
        return sub_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_ref_t Integer::sub_int(Integer::smart_ref_t b) {
        return Integer::smart_ref_t(new Integer(std::to_string(_num - b->_num)));
    }

    Integer::smart_ref_t Integer::div_int_wrapper(Argument::ref_t args) {
        return div_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_ref_t Integer::div_int(Integer::smart_ref_t b) {
        // TODO: divide by zero
        return Integer::smart_ref_t(new Integer(std::to_string(_num / b->_num)));
    }

    Integer::smart_bool_ref_t Integer::eq_int_wrapper(Argument::ref_t args) {
        return eq_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_bool_ref_t Integer::eq_int(Integer::smart_ref_t b) {
        return Integer::smart_bool_ref_t(new Boolean(_num == b->_num));
    }

    Integer::smart_bool_ref_t Integer::neq_int_wrapper(Argument::ref_t args) {
        return neq_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_bool_ref_t Integer::neq_int(Integer::smart_ref_t b) {
        return Integer::smart_bool_ref_t(new Boolean(_num != b->_num));
    }

    Integer::smart_bool_ref_t Integer::lt_int_wrapper(Argument::ref_t args) {
        return lt_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_bool_ref_t Integer::lt_int(Integer::smart_ref_t b) {
        return Integer::smart_bool_ref_t(new Boolean(_num < b->_num));
    }

    Integer::smart_bool_ref_t Integer::lteq_int_wrapper(Argument::ref_t args) {
        return lteq_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_bool_ref_t Integer::lteq_int(Integer::smart_ref_t b) {
        return Integer::smart_bool_ref_t(new Boolean(_num <= b->_num));
    }

    Integer::smart_bool_ref_t Integer::gteq_int_wrapper(Argument::ref_t args) {
        return gteq_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_bool_ref_t Integer::gteq_int(Integer::smart_ref_t b) {
        return Integer::smart_bool_ref_t(new Boolean(_num >= b->_num));
    }

    Integer::smart_bool_ref_t Integer::gt_int_wrapper(Argument::ref_t args) {
        return gt_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_bool_ref_t Integer::gt_int(Integer::smart_ref_t b) {
        return Integer::smart_bool_ref_t(new Boolean(_num > b->_num));
    }

    Integer::smart_ref_t Integer::mod_int_wrapper(Argument::ref_t args) {
        return mod_int(dynamic_cast<Integer::smart_ref_t>(args->get_arg(0)));
    }

    Integer::smart_ref_t Integer::mod_int(Integer::smart_ref_t b) {
        return Integer::smart_ref_t(new Integer(std::to_string(_num % b->_num)));
    }

    Argument::Argument(std::initializer_list<arg_t> ags)
        : Argument()
    {
        for (auto &i: args)
            i->inc_refcount();
        args.assign(ags);
    }

    Argument::arg_t Argument::get_arg(Argument::index_t index) {
        if (index >= args.size())
            return nullptr;
        args.at(index)->inc_refcount();
        return args.at(index);
    }

    Object::size_t Argument::print(std::ostream &os) const {
        return 0;
    }

/*    Object::smart_ref_t Argument::addition(Object::smart_ref_t obj) {
        return nullptr;
    }*/

    Object::smart_ref_t Argument::call_object_specific_method(string name, Object::args_ref_t args) {
        return nullptr;
    }

    Function::Function()
        : Object(dt_function)
    {
    }

    Function::~Function() {
        delete func;
    }

    Function::Function(Function::func_t fun)
        : Function()
    {
        this->func = fun;
    }

    Object::size_t Function::print(std::ostream &os) const {
        std::ostringstream s;
        s << "{Object::Function (" << std::hex << this << ")}";
        os << s.str();
        return s.str().length();
    }

    Object::smart_ref_t Function::call_object_specific_method(string name, Object::args_ref_t args) {
        return nullptr;
    }

    Boolean::Boolean(Boolean::bool_t val)
        : Object(dt_bool), value{val}
    {
    }

    Object::size_t Boolean::print(std::ostream &os) const {
        os << (value ? "true" : "false");
        return (value ? 4 : 5);
    }

    Object::smart_ref_t Boolean::call_object_specific_method(string name, Object::args_ref_t args) {
        return nullptr;
    }

    Variable::Variable()
        : Object(dt_identifier)
    {
    }

    Variable::Variable(Variable::value_t obj)
        : Variable()
    {
        value = obj;
        value->inc_refcount();
    }

    Object::size_t Variable::print(std::ostream &os) const {
        return value->print(os);
    }

    Object::smart_ref_t Variable::call_object_specific_method(string name, Object::args_ref_t args) {
        return nullptr;
    }

    void Variable::set_new_value(Variable::value_t val) {
        value->dec_refcount();
        if (value->get_refcount() == 0)
            delete value;
        value = val;
        value->inc_refcount();
    }

    Array::Array()
        : Object(dt_array)
    {
        methods = {
            .self_methods = {
                {"append", &Array::append_wrapper}
            }
        };
    }

    Object::size_t Array::print(std::ostream &os) const {
        Object::size_t ret = 1;
        os << "[";
        for (auto &i: elements) {
            ret += i->print(os);
            if (&i != &elements.back()) {
                os << ", ";
                ret += 2;
            }
        }
        os << "]";
        ++ret;
        return ret;
    }

    Object::smart_ref_t Array::call_object_specific_method(string name, args_ref_t args) {
        return (this->*methods.self_methods[name])(args);
    }

    Array::self_ref_t Array::append_wrapper(Argument::ref_t args) {
        return append(dynamic_cast<Array::elem_t>(args->get_arg(0)));
    }

    Array::self_ref_t Array::append(elem_t elem) {
        elem->inc_refcount();
        elements.push_back(elem);
        return this;
    }

} // step
