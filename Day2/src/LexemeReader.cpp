/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "LexemeReader.hpp"
#include "IReader.hpp"
#include "Lexeme.hpp"
#include <memory>
#include <string>

Step::LexemeReader::LexemeReader(std::unique_ptr<IReader> &&reader)
    : _reader(std::move(reader)), _cur_lexeme{0}
{
    process();
}

Step::LexemeReader::~LexemeReader() = default;

Step::Lexeme const &Step::LexemeReader::peek(std::size_t offset) {
    if (_cur_lexeme + offset >= _lexemes.size()) {
        return _lexemes.back();
    }
    return _lexemes.at(_cur_lexeme + offset);
}

Step::Lexeme const &Step::LexemeReader::next() {
    if (_cur_lexeme >= _lexemes.size()) return _lexemes.back();
    return _lexemes.at(_cur_lexeme++);  
}

std::string Step::LexemeReader::read() {
    return next()._lexeme;
}

void Step::LexemeReader::process() {
    std::string line;
    std::size_t line_no = 1;
    while ((line = _reader->read()) != IReader::E_OI) {
        std::size_t lexeme_begin = 0, forward = 0; 
        while (forward < line.length()) {
            skipws(line, forward);
            lexeme_begin = forward;
            char c = line.at(forward);
            switch (c) {
            case '\n':
                _lexemes.emplace_back("\\n", line_no++, ++forward, LexemeKind::NEWLINE);
                break;
            case '+':
                _lexemes.emplace_back("+", line_no, ++forward, LexemeKind::PLUS);
                break;
            case '-':
                _lexemes.emplace_back("-", line_no, ++forward, LexemeKind::HYPHEN);
                break;
            case '*':
                _lexemes.emplace_back("*", line_no, ++forward, LexemeKind::STAR);
                break;
            case '/':
                _lexemes.emplace_back("/", line_no, ++forward, LexemeKind::SLASH);
                break;
            case '%':
                _lexemes.emplace_back("%", line_no, ++forward, LexemeKind::MODULOUS);
                break;
            case '(':
                _lexemes.emplace_back("(", line_no, ++forward, LexemeKind::LEFT_PAREN);
                break;
            case ')':
                _lexemes.emplace_back(")", line_no, ++forward, LexemeKind::RIGHT_PAREN);
                break;
            // case '"':
                // process_string();
                // break;
            default:
                if (is_digit(c)) {
                    process_float(line, forward, lexeme_begin, line_no);
                // } else if (is_ident(c)) {
                    // process_identifier();
                } else {
                    _lexemes.emplace_back(std::string(1, c), line_no, ++forward, LexemeKind::MISCELLANEOUS);
                }
                break;
            }
        }
    }
    _lexemes.emplace_back("E_OI", line_no, 1, LexemeKind::E_OI);
}

void Step::LexemeReader::skipws(std::string const &line, std::size_t &pos) {
    while (pos < line.length() && is_space(line.at(pos)))
        ++pos;
}

bool Step::LexemeReader::is_space(char c) {
    return std::string(" \t\r\v\a\b").find_first_of(c) != std::string::npos;
}

bool Step::LexemeReader::is_digit(char c) {
    return c >= 48 && c <= 57;
}

bool Step::LexemeReader::is_ident(char c) {
    return (c >= 65 && c <= 91) || (c >= 97 && c <= 123);
}

void Step::LexemeReader::process_float(
        std::string const &line, 
        std::size_t &forward, 
        std::size_t &lexeme_begin, 
        std::size_t line_no
) {
    /* ++forward because at(forward) we already know we have a digit, so skip it */
    process_integer(line, ++forward);
    if (forward < line.length() && line.at(forward) == '.') {
        ++forward;
        if (forward >= line.length()) {
            // Error: unexpected end-of-input. expected digit after radix point
        }

        if (forward < line.length() && !is_digit(line.at(forward))) {
            // Error: a digit must follow a radix point
        }

        /* Extract the part after radix point */
        process_integer(line, ++forward);

        /* If there is any exponent part */
        if (forward < line.length() && 
                (line.at(forward) == 'E' || line.at(forward) == 'e')) {
            ++forward; 

            if (forward >= line.length()) {
                // Error: unexpected end-of-input. exponent has no digits
            }

            if (forward < line.length() &&
                    (line.at(forward) == '+' || line.at(forward) == '-')) {
                ++forward;
            }

            if (forward < line.length() && !is_digit(line.at(forward))) {
                // Error: expected digit after exponent
            }

            /* Process the exponent digits */
            process_integer(line, ++forward);
        }
        _lexemes.emplace_back(
                std::string(line.begin() + lexeme_begin, 
                            line.begin() + forward), 
                line_no, 
                lexeme_begin+1, 
                LexemeKind::FLOAT
        );
    } else {
        _lexemes.emplace_back(
                std::string(line.begin() + lexeme_begin, 
                            line.begin() + forward), 
                line_no, 
                lexeme_begin+1, 
                LexemeKind::INTEGER
        );
    }
}


void Step::LexemeReader::process_integer(std::string const &line, std::size_t &forward) {
   char c;
   while (forward < line.length() && (is_digit(c = line.at(forward)) || c == '_')) {
        ++forward;
   }
   if (!is_digit(line.at(forward-1))) {
        // Error: number can not finish with or have a '_' before radix point
   }
}
