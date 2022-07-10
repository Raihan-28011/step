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

    ExpressionNodePtr Parser::parse() {
        if (true) {
            auto t = parse_expression();

#ifdef __DEBUG_STEP__
            t->accept(this);
#endif
            return t;
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
        if (tIndex >= tokens.size())
            return tokens.back();
        return tokens.at(tIndex);
    }

    bool Parser::is_next(TokenKind kind) {
        return peek_token().get_kind() == kind;
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

    void Parser::print(NumberExpression *nexpr) {
        std::cout << "("
                  << nexpr->get_num().get_tok()
                  << ")";
    }

    ExpressionNodePtr Parser::parse_expression() {
        return parse_term(0);
    }

    ExpressionNodePtr Parser::parse_term(i32 precedence) {
        auto left = parse_factor(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_plus) && !is_eof() && eat_if({t_plus, t_minus})) {
            auto tok = tokens.at(cur_token).get_kind();
            right = parse_term(binary_precedence(tok));
            left = std::make_shared<BinaryExpression>(tok, left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_factor(i32 precedence) {
        auto left = parse_primary(precedence);
        ExpressionNodePtr right = nullptr;
        while (precedence < binary_precedence(t_star) && !is_eof() && eat_if({t_star, t_slash, t_modulus})) {
            auto tok = tokens.at(cur_token).get_kind();
            right = parse_factor(binary_precedence(tok));
            left = std::make_shared<BinaryExpression>(tok, left, right);
        }
        return left;
    }

    ExpressionNodePtr Parser::parse_primary(i32 precedence) {
        if (eat_if({t_num, t_string, t_true, t_false, t_ident, t_null, t_lparen})) {
            auto tok = tokens.at(cur_token);
            switch (tok.get_kind()) {
                case t_num:
                    return std::make_shared<NumberExpression>(tok);
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
} // step