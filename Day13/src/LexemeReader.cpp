/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "LexemeReader.hpp"
#include "ErrorManager.hpp"
#include "IError.hpp"
#include "IReader.hpp"
#include "Lexeme.hpp"
#include "LexicalError.hpp"
#include <algorithm>
#include <limits>
#include <memory>
#include <string>

Step::LexemeReader::LexemeReader(std::unique_ptr<IReader> &&reader,
                                 std::string fname)
    : _reader(std::move(reader)), 
      _fname{fname}, 
      _cur_lexeme{0},
      LEXEME_E_IO{_fname, IReader::E_OI, 0, 1, Step::LexemeKind::e_io}
{
    process();
    LEXEME_E_IO._line = line_no;
}

Step::LexemeReader::~LexemeReader() = default;

Step::Lexeme const &Step::LexemeReader::peek(std::size_t offset) {
    if (_cur_lexeme + offset >= _lexemes.size() || _cur_lexeme + offset < 0) {
        return Step::LexemeReader::LEXEME_E_IO;
    }
    return _lexemes.at(_cur_lexeme + offset);
}

Step::Lexeme const &Step::LexemeReader::next() {
    if (_cur_lexeme >= _lexemes.size()) return _lexemes.back();
    return _lexemes.at(_cur_lexeme++);  
}

std::string Step::LexemeReader::read() {
    return Step::to_string(next());
}

char Step::LexemeReader::nextc() {
    if (forward >= line.length()) return '\0';
    line_no += (line.at(forward) == '\n');
    return line.at(forward++);
}

char Step::LexemeReader::peekc() {
    if (forward >= line.length()) return '\0';
    return line.at(forward);
}

Step::Lexeme Step::LexemeReader::make_lexeme(Step::LexemeKind kind,
                                            Step::Lexeme::lexeme_type lexeme,
                                            std::size_t col) {
    return Lexeme{
        _fname,
        lexeme,
        line_no,
        col,
        kind,
    };
}

void Step::LexemeReader::process() {
    line_no = 1;
    while ((line = _reader->read()) != IReader::E_OI) {
        lexeme_begin = 0, forward = 0;
        while (peekc() != '\0') {
            skipws();
            lexeme_begin = forward;
            char c = nextc();
            switch (c) {
            case '\0':
                break;
            case '\n':
                _lexemes.emplace_back(make_lexeme(LexemeKind::newline, 
                                                  "\\n", 
                                                  lexeme_begin));
                break;
            case '+':
                _lexemes.emplace_back(make_lexeme(LexemeKind::plus, 
                                                  "+", 
                                                  lexeme_begin));
                break;
            case '-':
                /* If this is a negative number */
                if (is_digit(peekc())) {
                    process_float(false);
                } else {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::hyphen, 
                                                      "-", 
                                                      lexeme_begin));
                }
                break;
            case '*':
                _lexemes.emplace_back(make_lexeme(LexemeKind::star, 
                                                  "*", 
                                                  lexeme_begin));
                break;
            case '/':
                _lexemes.emplace_back(make_lexeme(LexemeKind::slash, 
                                                  "/", 
                                                  lexeme_begin));
                break;
            case '%':
                _lexemes.emplace_back(make_lexeme(LexemeKind::modulous, 
                                                  "%", 
                                                  lexeme_begin));
                break;
            case '(':
                _lexemes.emplace_back(make_lexeme(LexemeKind::lparen, 
                                                  "(", 
                                                  lexeme_begin));
                break;
            case ')':
                _lexemes.emplace_back(make_lexeme(LexemeKind::rparen, 
                                                  ")", 
                                                  lexeme_begin));
                break;
            case '<':
                if (peekc() == '=') {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::less_than_equal, 
                                                      "<=", 
                                                      lexeme_begin));
                } else {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::less_than, 
                                                      "<", 
                                                      lexeme_begin));
                }
                break;
            case '>':
                if (peekc() == '=') {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::greater_than_equal, 
                                                      ">=", 
                                                      lexeme_begin));
                } else {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::greater_than, 
                                                      ">", 
                                                      lexeme_begin));
                }
                break;
            case '=':
                if (peekc() == '=') {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::equal_equal, 
                                                      "==", 
                                                      lexeme_begin));
                }
                break;
            case '!':
                if (peekc() == '=') {
                    _lexemes.emplace_back(make_lexeme(LexemeKind::not_equal, 
                                                      "!=", 
                                                      lexeme_begin));
                }
                break;
            // case '"':
                // process_string();
                // break;
            default:
                if (is_digit(c)) {
                    process_float(true);
                // } else if (is_ident(c)) {
                    // process_identifier();
                } else {
                    Step::ErrorManager::instance()
                        .add(std::make_unique<Step::LexicalError>(
                            IError::ErrorCode::E007,
                            _fname,
                            line,
                            line_no,
                            forward
                        ));
                    skip_to_newline();
                }
                break;
            }
        }
    }
    _lexemes.emplace_back(make_lexeme(LexemeKind::e_io, 
                                      IReader::E_OI, 
                                      lexeme_begin));
}

void Step::LexemeReader::skipws() {
    while (is_space(peekc()))
        nextc();
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

bool Step::LexemeReader::is_greater(std::string const &s, long long i) {
    std::string t{std::to_string(i)};
    return s.length() > t.length() || (s.length() == t.length() && s > t);
}

void Step::LexemeReader::skip_to_newline() {
    while (peekc() != '\n' && peekc() != '\0')
        nextc();
    if (peekc() == '\n')
        nextc(); // skip the newline also
}

void Step::LexemeReader::process_float(bool positive) {
    process_integer();
    if (peekc() == '.') {
        nextc();
        if (peekc() == '\0') {
            // Error: unexpected end-of-input. expected digit after radix point
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::LexicalError>(
                    IError::ErrorCode::E008,
                    _fname,
                    line,
                    line_no,
                    forward
                ));
            return;
        }

        if (!is_digit(peekc())) {
            // Error: a digit must follow a radix point
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::LexicalError>(
                    IError::ErrorCode::E009,
                    _fname,
                    line,
                    line_no,
                    forward
                ));
            skip_to_newline();
            return;
        }

        /* Extract the part after radix point */
        process_integer();

        /* If there is any exponent part */
        if (peekc() == 'E' || peekc() == 'e') {
            nextc();

            if (peekc() == '\0') {
                // Error: unexpected end-of-input. exponent has no digits
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::LexicalError>(
                        IError::ErrorCode::E010,
                        _fname,
                        line,
                        line_no,
                        forward
                    ));
                return;
            }

            std::size_t exponent_begin = forward;
            bool positive_exponent = true;
            if (peekc() == '+' || peekc() == '-') {
                positive_exponent = (nextc() == '+');
            }

            if (!is_digit(peekc())) {
                // Error: expected digit after exponent
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::LexicalError>(
                        IError::ErrorCode::E011,
                        _fname,
                        line,
                        line_no,
                        forward
                    ));
                skip_to_newline();
                return;
            }


            /* Process the exponent digits */
            process_integer();

            std::string exponent;
            std::copy_if(line.begin() + exponent_begin, 
                         line.begin() + forward, 
                         std::back_inserter(exponent),
                         [](char c) { return c != '_'; });
            // TODO: Convert the exponent to a number and then generate exponent specific error
            if ((positive_exponent && is_greater(exponent, std::numeric_limits<double>::max_exponent10)) ||
                (!positive_exponent && is_greater(exponent, std::numeric_limits<double>::min_exponent10))) {
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::LexicalError>(
                        positive_exponent ? IError::ErrorCode::E013 : IError::ErrorCode::E015,
                        _fname,
                        line,
                        line_no,
                        lexeme_begin
                    ));
                skip_to_newline();
                return;
                return;    
            }
        }

        std::string float_;
        /* Extract the float part without the underscores */
        std::copy_if(line.begin() + lexeme_begin, 
                     line.begin() + forward, 
                     std::back_inserter(float_),
                     [](char c) { return c != '_'; });
        _lexemes.emplace_back(make_lexeme(
                LexemeKind::FLOAT,
                std::stod(float_),
                lexeme_begin+1
            )
        );
    } else {
        std::string integer;
        /* Extract the integer part without the underscores */
        std::copy_if(line.begin() + lexeme_begin, 
                     line.begin() + forward, 
                     std::back_inserter(integer),
                     [](char c) { return c != '_'; });
        
        /* Check if the integer exceed the maximum digit count or 
         * if the integer is greater than the maximum integer (for positive) or
         * if the integer is smaller than the maximum integer (for negative)
         * if so, then generate error */
        if ((integer.length() - (integer.front() == '+' || integer.front() == '-')) > std::numeric_limits<Step::Lexeme::lexeme_int_type>::digits10 ||
            (positive && is_greater(integer, std::numeric_limits<Step::Lexeme::lexeme_int_type>::max())) ||
            (!positive && is_greater(integer, std::numeric_limits<Step::Lexeme::lexeme_int_type>::min()))) {
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::LexicalError>(
                    positive ? IError::ErrorCode::E012 : IError::ErrorCode::E014,
                    _fname,
                    line,
                    line_no,
                    lexeme_begin
                ));
            skip_to_newline();
            return;
        }

        _lexemes.emplace_back(make_lexeme(
                LexemeKind::integer,
                std::stoll(integer), 
                lexeme_begin+1 
            )
        );
    }
}


std::size_t Step::LexemeReader::process_integer() {
    std::size_t digit_count = 0;
    char c;
    while ((is_digit(c = peekc()) || c == '_')) {
        nextc();
        digit_count += (c != '_');
    }

    /* If the number finished with a digit or not */
    if (!is_digit(line.at(forward-1))) {
        // Error: misplaced separator
        Step::ErrorManager::instance()
            .add(std::make_unique<Step::LexicalError>(
                IError::ErrorCode::E016,
                _fname,
                line,
                line_no,
                forward-1
            ));
    }
    return digit_count;
}
