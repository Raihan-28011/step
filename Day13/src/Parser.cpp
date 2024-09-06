/*
 * created by: shunnobir
 * date: 28/07/2024
 */

#include "Parser.hpp"
#include "ExpressionStatementNode.hpp"
#include "GroupNode.hpp"
#include "IAst.hpp"
#include "Lexeme.hpp"
#include "LexemeReader.hpp"
#include "BinaryOpNode.hpp"
#include "LiteralNode.hpp"
#include "ModuleNode.hpp"
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

Step::Parser &Step::Parser::set_lexer(std::unique_ptr<Step::LexemeReader> lexer) {
    _lexer = std::move(lexer);
    return *this;
}

Step::Lexeme const &Step::Parser::current() const {
    return _lexer->peek(-1);
}

bool Step::Parser::eat_if(Step::LexemeKind kind) const {
    return eat_if_any({kind});
}

bool Step::Parser::eat_if_any(std::vector<Step::LexemeKind> kinds) const {
    bool matched = std::any_of(kinds.begin(), kinds.end(), 
            [this](Step::LexemeKind kind) { return kind == this->_lexer->peek()._kind; });
    if (matched) _lexer->next();
    return matched;
}

bool Step::Parser::match_any(std::vector<Step::LexemeKind> kinds) const {
    return std::any_of(kinds.begin(), kinds.end(), 
            [this](Step::LexemeKind kind) { return kind == this->_lexer->peek()._kind; });
}

std::unique_ptr<Step::IAst> Step::Parser::parse(std::unique_ptr<Step::LexemeReader> lexer) {
    set_lexer(std::forward<std::unique_ptr<Step::LexemeReader>>(lexer)); 
    auto expr = parse_module();
    return expr;
}

std::unique_ptr<Step::IAst> Step::Parser::parse_module() {
    /*
     * module ::= statements
     */
    std::unique_ptr<Step::IAst> module = nullptr;
    if (!match_any({Step::LexemeKind::e_io})) {
         module = parse_statements();
        if (!eat_if(Step::LexemeKind::e_io)) {
            // Error: expected end-of-input found ---
        }
    }

    return module;
}

std::unique_ptr<Step::IAst> Step::Parser::parse_statements() {
    /*
     * statements ::= expression_statement+
     */
    std::vector<std::unique_ptr<Step::IAst>> stmts;
    while (!match_any({Step::LexemeKind::e_io})) {
        auto stmt = parse_expression_statement();
        if (!stmt) {
            // Error: unexpecyed end-of-input
            return nullptr;
        }
        stmts.emplace_back(std::move(stmt));
    }

    if (stmts.size() == 0) {
        return nullptr;
    }

    return std::make_unique<Step::ModuleNode>(std::move(stmts));
}

std::unique_ptr<Step::IAst> Step::Parser::parse_expression_statement() {
    /*
     * expression_statement ::= expression NEWLINE
     */
    while (eat_if(Step::LexemeKind::newline)) 
        ;

    std::unique_ptr<Step::IAst> expr = nullptr;
    if (!match_any({Step::LexemeKind::newline})) {
        expr = parse_expression();
        if (!eat_if(Step::LexemeKind::newline) || !match_any({Step::LexemeKind::e_io})) {
            // Error: expected newline, found ---
        }
    }

    if (!expr) {
        return nullptr;
    }

    return std::make_unique<Step::ExpressionStatementNode>(std::move(expr));
}

std::unique_ptr<Step::IAst> Step::Parser::parse_expression() {
    /*
     * expression ::= (expression ('<' | '>' | '<=' | '>=' | '==' | '!='))* expression
     */
    auto left = parse_relational();
    while (eat_if_any({Step::LexemeKind::less_than, Step::LexemeKind::less_than_equal, Step::LexemeKind::greater_than, Step::LexemeKind::greater_than_equal, Step::LexemeKind::equal_equal, Step::LexemeKind::not_equal})) {
        Step::LexemeKind op = current()._kind;
        std::unique_ptr<Step::IAst> right = parse_relational();
        left = std::make_unique<Step::BinaryOpNode>(std::move(left), std::move(right), op);
    }
    return left;
}


std::unique_ptr<Step::IAst> Step::Parser::parse_relational() {
    /*
     * relational ::= (term ('+' | '-'))* term
     */
    auto left = parse_term();
    while (eat_if_any({Step::LexemeKind::plus, Step::LexemeKind::hyphen})) {
        Step::LexemeKind op = current()._kind;
        std::unique_ptr<Step::IAst> right = parse_term();
        left = std::make_unique<Step::BinaryOpNode>(std::move(left), std::move(right), op);
    }
    return left;
}

std::unique_ptr<Step::IAst> Step::Parser::parse_term() {
    /*
     * term ::= factor (('*' | '/' | '%') factor)*
     */
    auto left = parse_factor();
    while (eat_if_any({Step::LexemeKind::star, Step::LexemeKind::slash, Step::LexemeKind::modulous})) {
        Step::LexemeKind op = current()._kind;
        std::unique_ptr<Step::IAst> right = parse_factor();
        left = std::make_unique<Step::BinaryOpNode>(std::move(left), std::move(right), op);
    }
    return left;
}

std::unique_ptr<Step::IAst> Step::Parser::parse_factor() {
    /*
     * factor ::= literal
     *          | '(' expression ')'
     */
    if (eat_if(Step::LexemeKind::lparen)) {
        std::unique_ptr<Step::IAst> expr = parse_expression();
        if (!eat_if(Step::LexemeKind::rparen)) {
            // Error: expected ')' found ---
        }
        return std::make_unique<Step::GroupNode>(std::move(expr));
    } else {
        return parse_literal();
    }
}

std::unique_ptr<Step::IAst> Step::Parser::parse_literal() {
    /*
     * literal ::= integer
     *           | float
     */
    if (match_any({Step::LexemeKind::rparen, Step::LexemeKind::e_io, Step::LexemeKind::newline})) {
        return nullptr;
    }
    if (!eat_if_any({Step::LexemeKind::integer, Step::LexemeKind::FLOAT})) {
        // Error: expected literals found ---
    }
    return std::make_unique<Step::LiteralNode>(current()._lexeme, current()._kind);
}
