//
// Created by raihan on 7/10/22.
//

#include "parser.h"

namespace step {
    Parser::Parser(string const &content, string const &fname)
        : errorManager(ErrorManager::get_instance(content, fname))
    {
        Lexer *lexer = new Lexer(content, fname);
        tokens = lexer->get_tokens();
        delete lexer;
    }

    StatementNodePtr Parser::parse() {
        while (!is_eof()) {
            auto t = parse_statement();

#ifdef __DEBUG_STEP__
            t->accept(this);
#endif
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
                errorManager.compilation_error("expected expression", tok.get_line(), tok.get_col());
            }

            left = std::make_shared<BinaryExpression>(tok.get_kind(), left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_factor(i32 precedence) {
        auto left = parse_primary(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_star) && eat_if({t_star, t_slash, t_modulus})) {
            auto tok = tokens.at(cur_token);
            right = parse_factor(binary_precedence(tok.get_kind()));

            if (!right) {
                errorManager.compilation_error("expected expression", tok.get_line(), tok.get_col());
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
                        errorManager.compilation_error("'(' has no closing paren ')'", tok.get_line(), tok.get_col());
                    }
                    return expr;
                }
                default:
                    break;
            }
        }

        auto tok = tokens.at(cur_token);
        errorManager.compilation_error("expected expression", tok.get_line(), tok.get_col());
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
        } else if (is_next(t_print)) {
            return parse_print_statement();
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
            errorManager.compilation_error("expected '('", tok.get_line(), tok.get_col());
        }

        tok = tokens.at(cur_token);
        auto expr = parse_expression();

        if (!expr) {
            errorManager.compilation_error("expected expression", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_rparen)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_semicolon)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected ';'", tok.get_line(), tok.get_col());
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
                errorManager.compilation_error("expected ';'", tokens.at(cur_token).get_line(), tokens.at(cur_token).get_col());
            }

            return std::make_shared<ExpressionStatement>(t);
        }

        // TODO: parse 'ident.ident.ident = expression;'
        eat_if(t_ident);
        auto lhs = std::make_shared<ConstantExpression>(tokens.at(cur_token));

        eat_if(t_equal);
        auto rhs = parse_expression();

        if (!eat_if(t_semicolon)) {
            auto tok = tokens.at(cur_token);
            errorManager.compilation_error("expected ';'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<AssignmentStatement>(lhs, rhs);
    }

    ExpressionNodePtr Parser::parse_function_call(Token const &name) {
        if (!eat_if(t_lparen)) {
            errorManager.compilation_error("expected '('", name.get_line(), name.get_col());
        }

        vector<ExpressionNodePtr> args;
        while (!is_next(t_rparen)) {
            args.push_back(parse_expression());
            if (eat_if(t_comma)) {
                if (is_next_n(t_rparen, 0)) {
                    auto tok = tokens.at(cur_token);
                    errorManager.compilation_error("expected expression or ')'", tok.get_line(), tok.get_col());
                }
            }
        }

        if (!eat_if(t_rparen)) {
            auto tok = tokens.at(cur_token);
            errorManager.compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<FunctionCallExpression>(name, std::move(args)) ;
    }

    StatementNodePtr Parser::parse_function_def_statement() {
        eat_if(t_def);

        Token tok;
        if (!eat_if(t_ident)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected function identifier", tok.get_line(), tok.get_col());
        }

        Token name = tokens.at(cur_token);
        if (!eat_if(t_lparen)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected '('", tok.get_line(), tok.get_col());
        }

        vector<ExpressionNodePtr> params;
        while (is_next(t_ident) && !is_next(t_rparen)) {
            params.push_back(parse_expression());
            eat_if(t_comma);
            if (!is_next(t_rparen) && !is_next(t_ident)) {
                tok = next_token();
                errorManager.compilation_error("function arguments can only be identifiers", tok.get_line(), tok.get_col());
            }
        }

        if (!eat_if(t_rparen)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected ')'", tok.get_line(), tok.get_col());
        }

        if (!eat_if(t_lbrace)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected '{'", tok.get_line(), tok.get_col());
        }

        vector<StatementNodePtr> body;
        while (!is_next(t_rbrace)) {
            body.push_back(parse_statement());
        }

        if (!eat_if(t_rbrace)) {
            tok = tokens.at(cur_token);
            errorManager.compilation_error("expected '}'", tok.get_line(), tok.get_col());
        }

        return std::make_shared<FunctionDefStatement>(name, std::move(params), std::move(body));
    }
} // step