/*
 * created by: shunnobir
 * date: 27/07/2024
 */

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "IAst.hpp"
#include "Lexeme.hpp"
#include "LexemeReader.hpp"
#include <memory>
#include <vector>

namespace Step {
    class Parser {
    public:
        std::unique_ptr<Step::IAst> parse(std::unique_ptr<Step::LexemeReader> lexer);
    private:
        /* Private method */
        Parser &set_lexer(std::unique_ptr<Step::LexemeReader> lexer);
        std::unique_ptr<Step::IAst> parse_module();
        std::unique_ptr<Step::IAst> parse_statements();
        std::unique_ptr<Step::IAst> parse_expression_statement();
        std::unique_ptr<Step::IAst> parse_expression();
        std::unique_ptr<Step::IAst> parse_relational();
        std::unique_ptr<Step::IAst> parse_term();
        std::unique_ptr<Step::IAst> parse_factor();
        std::unique_ptr<Step::IAst> parse_literal();

        bool eat_if(Step::LexemeKind kind) const;
        bool eat_if_any(std::vector<Step::LexemeKind> kinds) const;
        bool match_any(std::vector<Step::LexemeKind> kinds) const;
        Step::Lexeme const &current() const;
    private:
        std::unique_ptr<Step::LexemeReader> _lexer;
    };
}

#endif // __PARSER_HPP__
