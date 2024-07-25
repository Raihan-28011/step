/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "LexemeReader.hpp"
#include "ErrorManager.hpp"
#include "ExpectationError.hpp"
#include "IReader.hpp"
#include "Lexeme.hpp"
#include "MisplacedNumSeparator.hpp"
#include "NumberTooLarge.hpp"
#include "NumberTooSmall.hpp"
#include "UnexpectedEoi.hpp"
#include "UnrecognizedToken.hpp"
#include <cmath>
#include <limits>
#include <memory>
#include <string>

Step::LexemeReader::LexemeReader(std::unique_ptr<IReader> &&reader,
                                 std::string fname)
    : _reader(std::move(reader)), _fname{fname}, _cur_lexeme{0}
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
                _lexemes.emplace_back(_fname, "\\n", line_no++, ++forward, LexemeKind::NEWLINE);
                break;
            case '+':
                _lexemes.emplace_back(_fname, "+", line_no, ++forward, LexemeKind::PLUS);
                break;
            case '-':
                _lexemes.emplace_back(_fname, "-", line_no, ++forward, LexemeKind::HYPHEN);
                break;
            case '*':
                _lexemes.emplace_back(_fname, "*", line_no, ++forward, LexemeKind::STAR);
                break;
            case '/':
                _lexemes.emplace_back(_fname, "/", line_no, ++forward, LexemeKind::SLASH);
                break;
            case '%':
                _lexemes.emplace_back(_fname, "%", line_no, ++forward, LexemeKind::MODULOUS);
                break;
            case '(':
                _lexemes.emplace_back(_fname, "(", line_no, ++forward, LexemeKind::LEFT_PAREN);
                break;
            case ')':
                _lexemes.emplace_back(_fname, ")", line_no, ++forward, LexemeKind::RIGHT_PAREN);
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
                    Step::ErrorManager::instance()
                        .add(std::make_unique<Step::UnrecognizedToken>(
                            _fname,
                            line,
                            line_no,
                            forward
                        ));
                    skip_to_newline(line, forward);
                    // _lexemes.emplace_back(_fname, std::string(1, c), line_no, ++forward, LexemeKind::MISCELLANEOUS);
                }
                break;
            }
        }
    }
    _lexemes.emplace_back(_fname, "E_OI", line_no, 1, LexemeKind::E_OI);
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

void Step::LexemeReader::skip_to_newline(std::string_view line, std::size_t &pos) {
    while (pos < line.length() && line.at(pos) != '\n')
        ++pos;
    if (pos < line.length())
        ++pos; // skip the newline also
}

void Step::LexemeReader::process_float(
        std::string const &line, 
        std::size_t &forward, 
        std::size_t &lexeme_begin, 
        std::size_t line_no
) {
    /* ++forward because at(forward) we already know we have a digit, so skip it */
    std::size_t digit_count = process_integer(line, ++forward, line_no);
    if (forward < line.length() && line.at(forward) == '.') {
        ++forward;
        if (forward >= line.length()) {
            // Error: unexpected end-of-input. expected digit after radix point
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::UnexpectedEoi>(
                    _fname,
                    line,
                    "expected digit after radix point",
                    line_no,
                    forward
                ));
            return;
        }

        if (forward < line.length() && !is_digit(line.at(forward))) {
            // Error: a digit must follow a radix point
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::ExpectationError>(
                    _fname,
                    line,
                    "expected digit after radix point: radix point must be followed by a digit",
                    line_no,
                    forward
                ));
            return;
        }

        /* Extract the part after radix point */
        process_integer(line, ++forward, line_no);

        /* If there is any exponent part */
        if (forward < line.length() && 
                (line.at(forward) == 'E' || line.at(forward) == 'e')) {
            ++forward; 

            if (forward >= line.length()) {
                // Error: unexpected end-of-input. exponent has no digits
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::UnexpectedEoi>(
                        _fname,
                        line,
                        "exponent has no digit",
                        line_no,
                        forward
                    ));
                return;
            }

            if (forward < line.length() &&
                    (line.at(forward) == '+' || line.at(forward) == '-')) {
                ++forward;
            }

            if (forward < line.length() && !is_digit(line.at(forward))) {
                // Error: expected digit after exponent
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::ExpectationError>(
                        _fname,
                        line,
                        "expected digit after exponent",
                        line_no,
                        forward
                    ));
                return;
            }

            std::size_t exponent_start = forward;

            /* Process the exponent digits */
            std::size_t exponent_digits = process_integer(line, ++forward, line_no);
            if (exponent_digits > std::size_t(std::log10(std::numeric_limits<double>::max_exponent10)+1)) {
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::NumberTooLarge>(
                        _fname,
                        line,
                        line_no,
                        exponent_start
                    ));
                return;    
            }

            if (exponent_digits > std::size_t(std::log10(std::numeric_limits<double>::min_exponent10)+1)) {
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::NumberTooSmall>(
                        _fname,
                        line,
                        line_no,
                        exponent_start
                    ));
                return;    
            }
        }
        _lexemes.emplace_back(
                _fname, 
                std::string(line.begin() + lexeme_begin, 
                            line.begin() + forward), 
                line_no, 
                lexeme_begin+1, 
                LexemeKind::FLOAT
        );
    } else {
        if (digit_count > 18) {
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::NumberTooLarge>(
                    _fname,
                    line,
                    line_no,
                    lexeme_begin
                ));
            return;
        }

        _lexemes.emplace_back(
                _fname, 
                std::string(line.begin() + lexeme_begin, 
                            line.begin() + forward), 
                line_no, 
                lexeme_begin+1, 
                LexemeKind::INTEGER
        );
    }
}


std::size_t Step::LexemeReader::process_integer(std::string const &line, std::size_t &forward, std::size_t line_no) {
    std::size_t digit_count = 0;
    char c;
    while (forward < line.length() && (is_digit(c = line.at(forward)) || c == '_')) {
        digit_count += (c != '_');
        ++forward;
    }
    if (!is_digit(line.at(forward-1))) {
        // Error: number can not finish with or have a '_' before radix point
        Step::ErrorManager::instance()
            .add(std::make_unique<Step::MisplacedNumSeparator>(
                _fname,
                line,
                line_no,
                forward
            ));
    }
    return digit_count;
}
