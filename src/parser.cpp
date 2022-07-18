//
// Created by raihan on 7/10/22.
//

#include "parser.h"

namespace step {
    Parser::Parser(string const &content, string const &fname)
        : errorManager(::errorManager)
    {
        Lexer *lexer = new Lexer(content, fname);
        tokens = lexer->get_tokens();
        delete lexer;

        call_stack.push(std::make_shared<Frame>());
        cur_frame = call_stack.top();
    }


    Parser::~Parser() = default;

    StatementNodePtr Parser::parse() {
        while (!is_eof()) {
            auto t = parse_statement();

#ifdef __DEBUG_STEP__
            if (t)
                t->accept(this);
#endif
            if (t)
                t->accept_evaluator(this);
        }

        return nullptr;
    }

    Token const &Parser::next_token() {
        if (tIndex >= tokens.size())
            return tokens.back();
        cur_token = tIndex;
        return tokens.at(tIndex++);
    }

    Token const &Parser::peek_token(i32 peek) {
        if (tIndex+peek >= tokens.size())
            return tokens.back();
        return tokens.at(tIndex+peek);
    }

    bool Parser::is_next(TokenKind kind) {
        return peek_token().get_kind() == kind;
    }

    bool Parser::is_next_n(TokenKind kind, i32 peek) {
        return peek_token(peek).get_kind() == kind;
    }

    bool Parser::is_eof() {
        return peek_token().get_kind() == t_eof;
    }

    void Parser::print(BinaryExpression *bexpr) {
        std::cout << "(";
        bexpr->get_left()->accept(this);
        std::cout << ' ' << Token::tok_s[bexpr->get_op()] << ' ';
        bexpr->get_right()->accept(this);
        std::cout << ")";
    }

    void Parser::print(ConstantExpression *nexpr) {
        auto tok = nexpr->get_constant();
        std::cout << "(" << (tok.get_kind() == t_string ? "'" : "")
                  << tok.get_tok()
                  << (tok.get_kind() == t_string ? "'" : "") << ")";
    }

    void Parser::print(IdentifierExpression *nexpr) {
        std::cout << "(" << nexpr->get_ident().get_tok() << ")";
    }

    void Parser::print(FunctionCallExpression *nexpr) {
        std::cout << "(" << nexpr->get_name().get_tok()
                  << "(";

        auto &args = nexpr->get_args();
        for (auto const &arg: args) {
            arg->accept(this);
            if (args.size() > 1 && arg != args.back())
                std::cout << ", ";
        }

        std::cout << "))";
    }

    void Parser::print(ArrayExpression *stmt) {
        std::cout << "[";
        auto const &elems = stmt->get_elements();
        for (auto const &i: elems) {
            i->accept(this);
            
            if (i != elems.back())
                std::cout << ", ";
        }
        std::cout << " ']";
    }

    void Parser::print(SubscriptExpression *expr) {
        std::cout << "[ ";
        expr->get_expression()->accept(this);
        std::cout << " ]";
    }

    void Parser::print(ChainedExpression *expr) {
        for (auto &i: expr->get_expressions())
            i->accept(this);
    }

    void Parser::print(PrintStatement *stmt) {
        std::cout << "('print' ";
        stmt->get_expr()->accept(this);
        std::cout << " ';')\n";
    }

    void Parser::print(AssignmentStatement *stmt) {
        std::cout << "(";
        stmt->get_left()->accept(this);
        std::cout << " '=' ";
        stmt->get_right()->accept(this);
        std::cout << " ';')\n";
    }

    void Parser::print(ExpressionStatement *stmt) {
        std::cout << "(";
        stmt->get_expr()->accept(this);
        std::cout << " ';')\n";
    }



    void Parser::print(FunctionDefStatement *stmt) {
        std::cout << "( 'def' " << stmt->get_name().get_tok()
                  << " '(' ";
        auto params = stmt->get_params();
        for (auto const &param: params) {
            param->accept(this);
            if (params.size() > 1 && param != params.back())
                std::cout << ", ";
        }

        std::cout << " ')' '{'\n";
        auto body = stmt->get_body();
        for (auto const &stm: body) {
            stm->accept(this);
        }
        std::cout << "'}' )\n";
    }

    void Parser::print(ReturnStatement *stmt) {
        std::cout << "( 'return' ";
        stmt->accept(this);
        std::cout << " ';' )";
    }

    void Parser::print(IfStatement *stmt) {
        std::cout << "( 'if' '(' ";
        stmt->get_condition()->accept(this);
        std::cout << "')'" <<  (stmt->get_body().size() > 1 ? " '{'\n" : "\n");
        for (auto const &i: stmt->get_body())
            i->accept(this);
        std::cout << (stmt->get_body().size() > 1 ? "\n'}'\n" : "\n")
                  << "')'\n";
        for (auto const &i: stmt->get_elifs())
            i->accept(this);
    }

    void Parser::print(ElseStatement *stmt) {
        std::cout << "( 'else'";
        std::cout << (stmt->get_body().size() > 1 ? " '{'\n" : "");
        for (auto const &i: stmt->get_body())
            i->accept(this);
        std::cout << (stmt->get_body().size() > 1 ? "\n'}'\n" : "\n")
                  << "')'\n";
    }

    void Parser::print(ElifStatement *stmt) {
        std::cout << "( 'elif' '(' ";
        stmt->get_condition()->accept(this);
        std::cout << "')'" <<  (stmt->get_body().size() > 1 ? " '{'\n" : "");
        for (auto const &i: stmt->get_body())
            i->accept(this);
        std::cout << (stmt->get_body().size() > 1 ? "\n'}'\n" : "\n")
                  << "')'\n";
    }

    ExpressionNodePtr Parser::parse_expression() {
        return parse_logicalOr(0);
    }


    ExpressionNodePtr Parser::parse_logicalOr(i32 precedence) {
        auto left = parse_equality(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_logicalor) && eat_if(t_logicalor)) {
            auto tok = tokens.at(cur_token);
            right = parse_term(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_logicalAnd(i32 precedence) {
        auto left = parse_equality(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_logicaland) && eat_if(t_logicaland)) {
            auto tok = tokens.at(cur_token);
            right = parse_term(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_equality(i32 precedence) {
        auto left = parse_comparison(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_equalequal) && eat_if({t_equalequal, t_notequal})) {
            auto tok = tokens.at(cur_token);
            right = parse_term(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_comparison(i32 precedence) {
        auto left = parse_term(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_less) && eat_if({t_less, t_lessequal, t_greater, t_greaterequal})) {
            auto tok = tokens.at(cur_token);
            right = parse_term(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }


    ExpressionNodePtr Parser::parse_term(i32 precedence) {
        auto left = parse_factor(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_plus) && eat_if({t_plus, t_minus})) {
            auto tok = tokens.at(cur_token);
            right = parse_term(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_factor(i32 precedence) {
        auto left = parse_unary();
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_star) && eat_if({t_star, t_slash, t_modulus, t_starstar})) {
            auto tok = tokens.at(cur_token);
            right = parse_factor(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_unary() {
        vector<ExpressionNodePtr> exprs;
        exprs.push_back(parse_primary());
        while (eat_if({t_lparen, t_lsqbrace, t_dot})) {
            auto tok = tokens.at(cur_token);
            ExpressionNodePtr exp = nullptr;
            switch (tok.get_kind()) {
                case t_lbrace:
                     exp = parse_function_call(Token{});
                     break;
                case t_lsqbrace:
                     exp = parse_subscript_operator();
                     break;
                case t_dot:
                    break;
                default:
                    break;
            }

            exprs.push_back(exp);
        }

        if (exprs.size() == 1)
            return exprs.front();
        return std::make_shared<ChainedExpression>(std::move(exprs));
    }

    ExpressionNodePtr Parser::parse_primary() {
        if (eat_if({t_num, t_string, t_true, t_false,
                    t_ident, t_null, t_lparen, t_lsqbrace})) {
            auto tok = tokens.at(cur_token);
            switch (tok.get_kind()) {
                case t_num:
                case t_string:
                case t_true:
                case t_false:
                case t_null:
                    return std::make_shared<ConstantExpression>(tok);
                case t_ident:
                    return std::make_shared<IdentifierExpression>(tok);
                case t_lparen:
                    return parse_group_expression(tok);
                case t_lsqbrace:
                    return parse_array_expression();
                default:
                    break;
            }
        }

        auto tok = tokens.at(cur_token);
        errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
        return nullptr;
    }

    ExpressionNodePtr Parser::parse_array_expression() {
        vector<ExpressionNodePtr> elems;

        while (!is_next(t_rsqbrace) && !is_next(t_eof)) {
            elems.push_back(parse_expression());
            eat_if(t_comma);
        }

        if (!is_next(t_rsqbrace)) {
            auto tok = next_token();
            errorManager->compilation_error("expected expression or ']'", 
                    tok.get_line(), 
                    tok.get_col());
        }


        eat_if(t_rsqbrace);
        return std::make_shared<ArrayExpression>(std::move(elems));
    }

    bool Parser::eat_if(TokenKind kind) {
        if (is_next(kind)) {
            next_token();
            return true;
        }

        return false;
    }

    bool Parser::eat_if(std::initializer_list<TokenKind> kinds) {
        for (auto &i: kinds) {
            if (eat_if(i))
                return true;
        }

        return false;
    }

    i32 Parser::binary_precedence(TokenKind op) {
        switch (op) {
            case t_logicalor:
                return 1;
            case t_logicaland:
                return 2;
            case t_equalequal:
            case t_notequal:
                return 3;
            case t_less:
            case t_lessequal:
            case t_greater:
            case t_greaterequal:
                return 4;
            case t_plus:
            case t_minus:
                return 5;
            case t_star:
            case t_slash:
            case t_modulus:
                return 6;
            case t_ident:
            case t_num:
            case t_string:
            case t_true:
            case t_false:
            case t_null:
            case t_lparen:
                return 7;
            default:
                return 0;
        }
    }

    i32 Parser::unary_precedence(TokenKind op) {
        switch (op) {
            // TODO: implement unary operator precedence
            default:
                return 0;
        }
    }

    StatementNodePtr Parser::parse_statement() {
        if (is_next(t_def)) {
            // TODO: implement parse_def_statement
            // TODO DONE
            return parse_function_def_statement();
        } else if (is_next(t_return)) {
            return parse_return_statement();
        } else if (is_next(t_if)) {
            return parse_if_statement();
        } else if (is_next(t_else)) {
            return parse_else_statement();
        } else if (is_next(t_elif)) {
            return parse_elif_statement();
        } else if (is_next(t_input)) {
            // TODO: implement parse_input_statement
        } else {
            return parse_assignment_statement();
        }

        return nullptr;
    }

    StatementNodePtr Parser::parse_print_statement() {
        eat_if(t_print);

        Token tok = tokens.at(cur_token);
        if (!eat_if(t_lparen)) {
            errorManager->compilation_error("expected '('", tok.get_line(), tok.get_col());
        }

        tok = tokens.at(cur_token);
        auto expr = parse_expression();

        if (!expr) {
            errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_rparen)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_semicolon)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ';'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<PrintStatement>(expr);
    }

    StatementNodePtr Parser::parse_assignment_statement() {
        bool is_assignment = false;
        i32 peek_ahead = 0;
        // TODO: parse 'ident.ident() = expression;'
        while (is_next_n(t_ident, peek_ahead) && !is_next_n(t_eof, peek_ahead)) {
            ++peek_ahead;
            if (is_next_n(t_equal, peek_ahead)) {
                is_assignment = true;
                break;
            }

            if (is_next_n(t_dot, peek_ahead)) {
                ++peek_ahead;
            }
        }

        if (!is_assignment) {
            auto t = parse_expression();
            if (!eat_if(t_semicolon)) {
                errorManager->compilation_error("expected ';'", tokens.at(cur_token).get_line(), tokens.at(cur_token).get_col());
            }

            return std::make_shared<ExpressionStatement>(t);
        }

        // TODO: parse 'ident.ident.ident = expression;'
        eat_if(t_ident);
        auto lhs = std::make_shared<IdentifierExpression>(tokens.at(cur_token));

        eat_if(t_equal);
        auto rhs = parse_expression();

        if (!eat_if(t_semicolon)) {
            auto tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ';'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<AssignmentStatement>(lhs, rhs);
    } 

    ExpressionNodePtr Parser::parse_function_call(Token const &name) {
        vector<ExpressionNodePtr> args;
        while (!is_next(t_rparen)) {
            args.push_back(parse_expression());
            eat_if(t_comma);
        }

        if (!eat_if(t_rparen)) {
            auto tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<FunctionCallExpression>(name, std::move(args)) ;
    }

    ExpressionNodePtr Parser::parse_group_expression(Token const &tok) {
        auto expr = parse_expression();
        if (!eat_if(t_rparen)) {
            errorManager->compilation_error("'(' has no closing paren ')'", tok.get_line(), tok.get_col());
        }
        return expr;
    }

    ExpressionNodePtr Parser::parse_subscript_operator() {
        if (is_next(t_rsqbrace)) {
            auto tok = next_token();
            errorManager->compilation_error("expected expression", 
                    tok.get_line(), 
                    tok.get_col());
        }
        auto expr = parse_expression();

       if (!eat_if(t_rsqbrace)) {
            auto tok = next_token();
            errorManager->compilation_error("expected ']'", tok.get_line(), tok.get_col());
       } 

       return std::make_shared<SubscriptExpression>(expr);
    }

    StatementNodePtr Parser::parse_function_def_statement() {
        inside_function = true;
        eat_if(t_def);

        Token tok;
        if (!eat_if(t_ident)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected function identifier", tok.get_line(), tok.get_col());
        }

        Token name = tokens.at(cur_token);
        if (!eat_if(t_lparen)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected '('", tok.get_line(), tok.get_col());
        }

        vector<ExpressionNodePtr> params;
        while (is_next(t_ident) && !is_next(t_rparen)) {
            params.push_back(parse_identifier());
            eat_if(t_comma);
            if (!is_next(t_rparen) && !is_next(t_ident) && !is_next(t_comma)) {
                tok = next_token();
                errorManager->compilation_error("unexpected token. expected ')'  or ','", tok.get_line(), tok.get_col());
            }
        }

        if (!eat_if(t_rparen)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_lbrace)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected '{'", tok.get_line(), tok.get_col());
        }

        vector<StatementNodePtr> body;
        bool has_return = false;
        while (!is_next(t_rbrace)) {
            auto stmt = parse_statement();
            body.push_back(stmt);
            if (!has_return)
                has_return = stmt->get_type() == step::StatementKind::s_ret;
        }

        if (!eat_if(t_rbrace)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected '}'", tok.get_line(), tok.get_col());
        }

        auto ret = new FunctionDefStatement(name, std::move(params), std::move(body));
        ret->set_return_statement(has_return);
        cur_frame->add_function(new Function(ret));
        inside_function = false;
        return nullptr;
    }

    void Parser::evaluate(PrintStatement *stms) {
        auto &expr = stms->get_expr();
        expr->accept_evaluator(this);
        auto const &top = cur_frame->pop();
        top->print(std::cout);
        delete top;
    }

    void Parser::evaluate(ConstantExpression *expr) {
        auto const &constant = expr->get_constant();

        switch (constant.get_kind()) {
            case t_num:
                cur_frame->push(new Integer(constant.get_tok()));
                break;
            case t_string:
                cur_frame->push(new String(constant.get_tok()));
                break;
            case t_true:
                cur_frame->push(new Boolean(true));
                break;
            case t_false:
                cur_frame->push(new Boolean(false));
                break;
            default:
                break;
        }
    }

    void Parser::evaluate(BinaryExpression *expr) {
        auto op = expr->get_op();
        auto const &lhs = expr->get_left();
        lhs->accept_evaluator(this);
        auto const &rhs = expr->get_right();
        rhs->accept_evaluator(this);

        auto r = cur_frame->pop(), l = cur_frame->pop();
        r->inc_refcount();
        l->inc_refcount();
        if (l->get_type() != r->get_type()) {
            delete l;
            delete r;
            errorManager->runtime_error("invalid binary operation", *cur_frame);
        }

        Argument::ref_t args = new Argument({r});
        switch (op) {
            case t_plus:
                cur_frame->push(l->call_object_specific_method("add", args));
                break;
            case t_star:
                cur_frame->push(l->call_object_specific_method("mult", args));
                break;
            case t_starstar:
                cur_frame->push(l->call_object_specific_method("pow", args));
                break;
            case t_modulus:
                cur_frame->push(l->call_object_specific_method("mod", args));
                break;
            case t_minus:
                cur_frame->push(l->call_object_specific_method("sub", args));
                break;
            case t_slash:
                cur_frame->push(l->call_object_specific_method("div", args));
                break;
            case t_equalequal:
                cur_frame->push(l->call_object_specific_method("eq", args));
                break;
            case t_notequal:
                cur_frame->push(l->call_object_specific_method("neq", args));
                break;
            case t_less:
                cur_frame->push(l->call_object_specific_method("lt", args));
                break;
            case t_greater:
                cur_frame->push(l->call_object_specific_method("gt", args));
                break;
            case t_lessequal:
                cur_frame->push(l->call_object_specific_method("lteq", args));
                break;
            case t_greaterequal:
                cur_frame->push(l->call_object_specific_method("gteq", args));
                break;
            default:
                break;
        }

        if (op != t_equal) {
            delete args;

            l->dec_refcount();
            r->dec_refcount();
            if (l->get_refcount() == 0)
                delete l;
            if (r->get_refcount() == 0)
                delete r;
        }
    }

    void Parser::evaluate(FunctionCallExpression *expr) {
        auto const &name = expr->get_name().get_tok();
        if (cur_frame->is_defined_builtin(name)) {
            if (name == "print") {
                auto &params = expr->get_args();
                for (auto &param: params)
                    param->accept_evaluator(this);
                __builtin_print_function(params.size());
            }
        } else if (cur_frame->is_user_defined(name)) {
            auto const &func = cur_frame->get_function(name);
            call_stack.push(std::make_shared<Frame>(*cur_frame));
            cur_frame = call_stack.top();
            auto const &params = expr->get_args();
            auto const &logic = func->get_logic();
            auto const &args = logic->get_params();
            if (params.size() != args.size()) {
                errorManager->compilation_error("call to function '" + name + "' "
                        "missing " + std::to_string(args.size()) + 
                        " argument(s)", expr->get_name().get_line(),
                        expr->get_name().get_col());
            }
            
            for (auto const &i: params) {
                i->accept_evaluator(this);
            }
            evaluate(func->get_logic());
            func->dec_refcount();
            auto top = cur_frame->top();
            call_stack.pop();
            cur_frame.reset();
            cur_frame = call_stack.top();
            cur_frame->push(top);
        } else if (cur_frame->is_defined_variable(name)) {
            auto var = cur_frame->at(cur_frame->get_variable(name));
            if (!dynamic_cast<Variable*>(var)->is_function()) {
                errorManager->compilation_error("'" + name + "' is not a function", 
                        expr->get_name().get_line(), expr->get_name().get_col());
            }

            auto func = dynamic_cast<Function*>(dynamic_cast<Variable*>(var)->get_value());
            call_stack.push(std::make_shared<Frame>(*cur_frame));
            cur_frame = call_stack.top();
            auto const &params = expr->get_args();
            auto const &logic = func->get_logic();
            auto const &args = logic->get_params();
            if (params.size() != args.size()) {
                errorManager->compilation_error("call to function '" + name + 
                        "[" + logic->get_name().get_tok() + "]' "
                        "missing " + std::to_string(args.size()) + " argument(s)", 
                        expr->get_name().get_line(),
                        expr->get_name().get_col());
            }
            
            for (auto const &i: params) {
                i->accept_evaluator(this);
            }
            evaluate(func->get_logic());
            func->dec_refcount();
            auto top = cur_frame->top();
            call_stack.pop();
            cur_frame.reset();
            cur_frame = call_stack.top();
            cur_frame->push(top); 
            var->dec_refcount();
        } else {
            errorManager->compilation_error("undefined function '" + name + "'", expr->get_name().get_line(), expr->get_name().get_col());
        } 
    }

    ExpressionNodePtr Parser::parse_identifier() {
        eat_if(t_ident);
        return std::make_shared<IdentifierExpression>(tokens.at(cur_token));
    }

    void Parser::evaluate(FunctionDefStatement *stms) {
        auto const &name = stms->get_name().get_tok();
        auto &params = stms->get_params();
        auto &body = stms->get_body();

        stack<Frame::ref_t> objs;
        for (i64 i = 0; i < params.size(); ++i) {
            auto top = cur_frame->pop();
            objs.push(top);
        }
        i64 i = 0;
        for (auto const &param: params) {
            auto obj = objs.top();
            auto var = new Variable(obj);
            objs.pop();
            cur_frame->add_variable(std::dynamic_pointer_cast<IdentifierExpression>(param)->get_ident().get_tok(),
                    var);
            ++i;
        } 

        for (auto &stmt: body) {
            stmt->accept_evaluator(this);
            if (return_statement_evaluated) {
                return_statement_evaluated = false;
                break;
            }
        }

        for (i64 i = 0; i < params.size(); ++i) {
            cur_frame->remove_variable(std::dynamic_pointer_cast<IdentifierExpression>(params[i])->get_ident().get_tok());   
            auto val = cur_frame->pop();
            if (val->get_refcount() == 0)
                delete val;

        }

        if (returned_value) {
            cur_frame->push(returned_value);
            returned_value = nullptr;
        } else {
            cur_frame->push(new Integer(std::to_string(0)));
        }
    }

    void Parser::evaluate(ExpressionStatement *stms) {
        auto expr = stms->get_expr();
        expr->accept_evaluator(this);
        auto top = cur_frame->top();
        cur_frame->pop();
        top->dec_refcount();
        if (top->get_refcount() == 0)
            delete top;
    }

    void Parser::__builtin_print_function(i64 args) {
        auto t = args;
        Object::size_t printed_char = 0;
        stack<Frame::ref_t> arguments;
        while (t--) {
            auto top = cur_frame->top();
            cur_frame->pop();
            arguments.push(top);
        }

        while (arguments.size() > 0) {
            auto top = arguments.top();
            arguments.pop();
            printed_char += top->print(std::cout);
            top->dec_refcount();
            if (top->get_refcount() == 0)
                delete top;
        }
        cur_frame->push(new Integer(std::to_string(printed_char)));
    }

    StatementNodePtr Parser::parse_return_statement() {
        eat_if(t_return);
        if (!inside_function) {
            auto tok = tokens.at(cur_token);
            errorManager->compilation_error("'return' statement outside function", tok.get_line(), tok.get_col());
        }

        ExpressionNodePtr expr = nullptr;
        if (!is_next(t_semicolon)) {
            expr = parse_expression();
        }

        if (!eat_if(t_semicolon)) {
            auto t = tokens.at(cur_token);
            errorManager->compilation_error("epxpected ';'", t.get_line(), t.get_col());
        }

        return std::make_shared<ReturnStatement>(expr);
    }

    void Parser::evaluate(ReturnStatement *stms) {
        if (stms->get_Expr()) {
            stms->get_Expr()->accept_evaluator(this);
            returned_value = cur_frame->pop();
            returned_value->inc_refcount();
        }
        return_statement_evaluated = true;
    }

    StatementNodePtr Parser::parse_if_statement() {
        eat_if(t_if);
        Token tok = tokens.at(cur_token);
        if (!eat_if(t_lparen)) {
            errorManager->compilation_error("expected '('", tok.get_line(), tok.get_col());
        }

        if (is_next(t_rparen)) {
            next_token();
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
        }

        auto condition = parse_expression();
        if (!eat_if(t_rparen)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        vector<StatementNodePtr> body;
        if (eat_if(t_lbrace)) {
            while (!is_next(t_rbrace)) {
                body.push_back(parse_statement());
            }

            if (!eat_if(t_rbrace)) {
                tok = next_token();
                errorManager->compilation_error("expected '}'", tok.get_line(), tok.get_col());
            }
        } else {
            body.push_back(parse_statement());
        }

        parsed_if = true;
        vector<StatementNodePtr> elifs;
        while (is_next(t_elif)) {
            elifs.push_back(parse_statement());
        }

        if (is_next(t_else)) {
            elifs.push_back(parse_statement());
        }
        
        parsed_if = false;
        return std::make_shared<IfStatement>(condition, std::move(body), std::move(elifs));
    }

    StatementNodePtr Parser::parse_else_statement() {
        eat_if(t_else);
        if (!parsed_if) {
            parsed_if = false;
            auto tok = tokens.at(cur_token);
            errorManager->compilation_error("else has no if block", tok.get_line(), tok.get_col());
        }

        vector<StatementNodePtr> body;
        if (eat_if(t_lbrace)) {
            while (!is_next(t_rbrace)) {
                body.push_back(parse_statement());
            }

            if (!eat_if(t_rbrace)) {
                auto tok = next_token();
                errorManager->compilation_error("expected '}'", tok.get_line(), tok.get_col());
            }
        } else {
            body.push_back(parse_statement());
        }

        parsed_if = false;

        return std::make_shared<ElseStatement>(std::move(body));
    }

    StatementNodePtr Parser::parse_elif_statement() {
        eat_if(t_elif);
        Token tok = tokens.at(cur_token);
        if (!parsed_if) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("elif has no if block", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_lparen)) {
            errorManager->compilation_error("expected '('", tok.get_line(), tok.get_col());
        }

        if (is_next(t_rparen)) {
            next_token();
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
        }

        auto condition = parse_expression();
        if (!eat_if(t_rparen)) {
            tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        vector<StatementNodePtr> body;
        if (eat_if(t_lbrace)) {
            while (!is_next(t_rbrace)) {
                body.push_back(parse_statement());
            }

            if (!eat_if(t_rbrace)) {
                tok = next_token();
                errorManager->compilation_error("expected '}'", tok.get_line(), tok.get_col());
            }
        } else {
            body.push_back(parse_statement());
        }

        if (!is_next(t_elif) && !is_next(t_else))
            parsed_if = false;

        return std::make_shared<ElifStatement>(condition, std::move(body));

    }

    void Parser::evaluate(IfStatement *stms) {
        auto cond = stms->get_condition();
        auto const &body = stms->get_body();
        auto const &elifs = stms->get_elifs();

        cond->accept_evaluator(this);
        auto val = cur_frame->top();
        cur_frame->pop();
        auto bool_value = dynamic_cast<Boolean*>(val)->get_value();
        val->dec_refcount();
        if (val->get_refcount() == 0)
            delete val;
        if (bool_value) {
            for (auto &stm: body)
                stm->accept_evaluator(this);
        } else {
            for (auto &elif: elifs) {
                elif->accept_evaluator(this);
                val = cur_frame->top();
                cur_frame->pop();
                // check the top value which was put by elif or else statement
                if (dynamic_cast<Boolean*>(val)->get_value()) {
                    delete val;
                    break;
                }

                delete val;
            }
        }
    }

    void Parser::evaluate(ElseStatement *stms) {
        auto const &body = stms->get_body();
        for (auto &stm: body)
            stm->accept_evaluator(this);
        cur_frame->push(new Boolean(true));
    }

    void Parser::evaluate(ElifStatement *stms) {
        auto cond = stms->get_condition();
        auto const &body = stms->get_body();

        cond->accept_evaluator(this);
        auto val = cur_frame->top();
        auto bool_value = dynamic_cast<Boolean*>(val)->get_value();
        if (bool_value) {
            for (auto &stm: body)
                stm->accept_evaluator(this);
        }

        // Do not delete val. Because this val will be on top of the stack and if
        // statement will check this val, if it was true, then if statement will stop
        // executing rest of the elif and else blocks, otherwise if will continue
    }

    void Parser::evaluate(IdentifierExpression *expr) {
        auto const &name = expr->get_ident();
        if (cur_frame->is_defined_variable(name.get_tok())) {
            auto val = cur_frame->at(cur_frame->get_variable(name.get_tok()));
            cur_frame->push(dynamic_cast<Variable*>(val)->get_value());
            val->dec_refcount();
        } else if (cur_frame->is_defined_builtin(name.get_tok())) {
            // TODO: implement assigning builtin functions
        } else if (cur_frame->is_user_defined(name.get_tok())) {
            auto val = cur_frame->get_function(name.get_tok());
            cur_frame->push(val);
        } else {
            errorManager->compilation_error("Undefined variable '" + name.get_tok() + "'", name.get_line(), name.get_col());
        }
    }

    void Parser::evaluate(AssignmentStatement *stms) {
        auto const &left = stms->get_left();
        auto const &right = stms->get_right();
        
        right->accept_evaluator(this);

        Token const name = left->get_ident();
        if (cur_frame->is_defined_variable(name.get_tok())) {
            auto index = cur_frame->get_variable(name.get_tok());
            auto val = cur_frame->at(index);
            dynamic_cast<Variable*>(val)->set_new_value(cur_frame->pop());
            val->dec_refcount();
        } else {
            auto val = cur_frame->pop();
            auto var = new Variable(val);
            cur_frame->add_variable(name.get_tok(), var);
        }
    }

    void Parser::evaluate(ArrayExpression *expr) {
        vector<Array::elem_t> elems;
        auto const &t = expr->get_elements();
        for (auto &i: t) {
            i->accept_evaluator(this);
            auto top = cur_frame->pop();
            elems.push_back(top);
        }
    
        auto arr = new Array();
        for (auto &i: elems) {
            auto args = new Argument({i});
            arr->call_object_specific_method("append", args);
            delete args;
        }
        cur_frame->push(arr);
    }

    void Parser::evaluate(SubscriptExpression *expr) {
        auto exp = expr->get_expression();
        auto top = cur_frame->pop();
        top->inc_refcount();
        
        exp->accept_evaluator(this);
        auto index = cur_frame->pop();

        if (index->get_type() != dt_int) {
            delete index;
            delete top;
            errorManager->runtime_error("array indices must be integers", *cur_frame);
        }

        if (!top->is_array()) {
            delete top;
            errorManager->runtime_error("invalid subscript operand", *cur_frame);
        }

        if (dynamic_cast<Integer*>(index)->get_num() >= dynamic_cast<Array*>(top)->get_size()) {
            delete top;
            delete index;
            errorManager->runtime_error("array index out of bounds", *cur_frame);
        }
        Argument *arg = new Argument({index});
        cur_frame->push(dynamic_cast<Array*>(top)->call_object_specific_method("at", arg));
        delete arg;
        top->dec_refcount();
    }

    void Parser::evaluate(ChainedExpression *expr) {
        auto exprs = expr->get_expressions();
        for (auto &i: exprs)
            i->accept_evaluator(this);
    }

} // step
