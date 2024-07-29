/*
 * created by: shunnobir
 * date: 28/07/2024
 */

#include "Parser.hpp"
#include "GroupNode.hpp"
#include "IAst.hpp"
#include "Lexeme.hpp"
#include "LexemeReader.hpp"
#include "BinaryOpNode.hpp"
#include "LiteralNode.hpp"
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
    auto expr = parse_expression();
    if (!eat_if_any({Step::LexemeKind::NEWLINE, Step::LexemeKind::E_OI})) {
        // Error: expected newline found ---
    }
    return expr;
}

std::unique_ptr<Step::IAst> Step::Parser::parse_expression() {
    /*
     * expression ::= term + expression
     *              | term - expression
     *              | term 
     */
    auto left = parse_term();
    if (!eat_if_any({Step::LexemeKind::PLUS, Step::LexemeKind::HYPHEN})) {
        return left;
    }
    Step::LexemeKind op = current()._kind;
    std::unique_ptr<Step::IAst> right = parse_expression();
    return std::make_unique<Step::BinaryOpNode>(std::move(left), std::move(right), op);
}

std::unique_ptr<Step::IAst> Step::Parser::parse_term() {
    /*
     * term ::= factor * term
     *       | factor / term
     *       | factor % term
     *       | factor
     */
    auto left = parse_factor();
    if (!eat_if_any({Step::LexemeKind::STAR, Step::LexemeKind::SLASH, Step::LexemeKind::MODULOUS})) {
        return left;
    }
    Step::LexemeKind op = current()._kind;
    std::unique_ptr<Step::IAst> right = parse_term();
    return std::make_unique<Step::BinaryOpNode>(std::move(left), std::move(right), op);
}

std::unique_ptr<Step::IAst> Step::Parser::parse_factor() {
    /*
     * factor ::= literal
     *          | '(' expression ')'
     */
    if (eat_if(Step::LexemeKind::LEFT_PAREN)) {
        std::unique_ptr<Step::IAst> expr = parse_expression();
        if (!eat_if(Step::LexemeKind::RIGHT_PAREN)) {
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
    if (match_any({Step::LexemeKind::RIGHT_PAREN, Step::LexemeKind::E_OI, Step::LexemeKind::NEWLINE})) {
        return nullptr;
    }
    if (!eat_if_any({Step::LexemeKind::INTEGER, Step::LexemeKind::FLOAT})) {
        // Error: expected literals found ---
    }
    return std::make_unique<Step::LiteralNode>(current()._lexeme, current()._kind);
}
