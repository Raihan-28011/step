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

    ExpressionNodePtr Parser::parse_expression() {
        return parse_term(0);
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
        auto left = parse_primary(precedence);
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

    ExpressionNodePtr Parser::parse_primary(i32 precedence) {
        if (eat_if({t_num, t_string, t_true, t_false, t_ident, t_null, t_lparen})) {
            auto tok = tokens.at(cur_token);
            switch (tok.get_kind()) {
                case t_num:
                case t_string:
                case t_true:
                case t_false:
                case t_null:
                    return std::make_shared<ConstantExpression>(tok);
                case t_ident:
                {
                    if (is_next(t_lparen)) return parse_function_call(tok);
                    else return std::make_shared<ConstantExpression>(tok);
                }
                case t_lparen:
                {
                    auto expr = parse_expression();
                    if (!eat_if(t_rparen)) {
                        errorManager->compilation_error("'(' has no closing paren ')'", tok.get_line(), tok.get_col());
                    }
                    return expr;
                }
                default:
                    break;
            }
        }

        auto tok = tokens.at(cur_token);
        errorManager->compilation_error("expected expression", tok.get_line(), tok.get_col());
        return nullptr;
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
            case t_plus:
            case t_minus:
                return 1;
            case t_star:
            case t_slash:
            case t_modulus:
                return 2;
            case t_ident:
            case t_num:
            case t_string:
            case t_true:
            case t_false:
            case t_null:
            case t_lparen:
                return 3;
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
        if (!eat_if(t_lparen)) {
            errorManager->compilation_error("expected '('", name.get_line(), name.get_col());
        }

        vector<ExpressionNodePtr> args;
        while (!is_next(t_rparen)) {
            args.push_back(parse_expression());
            if (eat_if(t_comma)) {
                if (is_next_n(t_rparen, 0)) {
                    auto tok = tokens.at(cur_token);
                    errorManager->compilation_error("expected expression or ')'", tok.get_line(), tok.get_col());
                }
            }
        }

        if (!eat_if(t_rparen)) {
            auto tok = tokens.at(cur_token);
            errorManager->compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<FunctionCallExpression>(name, std::move(args)) ;
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
            case t_minus:
                cur_frame->push(l->call_object_specific_method("sub", args));
                break;
            case t_slash:
                cur_frame->push(l->call_object_specific_method("div", args));
                break;
            default:
                break;
        }

        if (op != t_equal) {
            delete l;
            delete r;
            delete args;
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
            auto func = cur_frame->get_function(name);
            evaluate(func->get_logic());
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

        for (auto &stmt: body) {
            stmt->accept_evaluator(this);
            if (return_statement_evaluated) {
                return_statement_evaluated = false;
                break;
            }
        }

        if (!stms->has_return_statement())
            cur_frame->push(new Integer(std::to_string(0)));
    }

    void Parser::evaluate(ExpressionStatement *stms) {
        auto expr = stms->get_expr();
        expr->accept_evaluator(this);
        auto top = cur_frame->top();
        cur_frame->pop();
        delete top;
    }

    void Parser::__builtin_print_function(i64 args) {
        auto t = args;
        Object::size_t printed_char = 0;
        while (t--) {
            auto top = cur_frame->top();
            cur_frame->pop();
            printed_char += top->print(std::cout);
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

        ExpressionNodePtr expr;
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
        stms->get_Expr()->accept_evaluator(this);
        return_statement_evaluated = true;
    }



} // step