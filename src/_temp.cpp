#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <filesystem>
#include <variant>
#include <vector>

/*
 * Typedefs
 */
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;
using f128 = long double;

using step_int_type = i64;
using step_float_type = f64;
using literal_type = std::variant<std::string, step_int_type, step_float_type>;


/* 
 * Constants
 */
template <typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
constexpr T const MAX_V = std::numeric_limits<T>::max();

template <typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
constexpr T const MIN_V = std::numeric_limits<T>::lowest();

/* E_OI = End Of Input*/
std::string E_OI = "<<EOI>>";

enum: size_t {
    STR_VARIANT_INDEX,
    INT_VARIANT_INDEX,
    FLT_VARIANT_INDEX,
};


/* 
 * Global utility functions
 * functions with '_i' prefix are internal private functions
 */

void _i_print(std::ostream &out, char const *fmt) {
    out << fmt;
}

/*
 * Actual formatter function that formats the arguments based on the flags passed
 */
template <typename T>
i32 _i_format_print(std::ostream &out, char const *fmt, T arg) {
    char const *_start = fmt, 
         *_end = fmt;

    while (*_end != '}' && *_end != '\0')
        ++_end;

    if (*_end != '}') {
        return 0;
    }

    if (_start == _end) {
        out << arg;
        return 1;
    }

    std::ios::fmtflags flags = out.flags(),
        _prev_flags = out.flags();
    i32 _width = out.width(),
        _precision = out.precision();
    while (_start != _end) {
        switch (*_start) {
            case '#':
                flags |= std::ios::showbase;
                break;
            case 'd':
                flags &= ~std::ios::basefield;
                flags |= std::ios::dec;
                break;
            case 'O':
                flags |= std::ios::uppercase;
            case 'o':
                flags &= ~std::ios::basefield;
                flags |= std::ios::oct;
                break;
            case 'X':
                flags |= std::ios::uppercase;
            case 'x':
                flags &= ~std::ios::basefield;
                flags |= std::ios::hex;
                flags &= ~std::ios::floatfield;
                flags |= std::ios::scientific;
                flags |= std::ios::fixed;
                break;
            default:
                if (std::isdigit(*_start)) {
                    char const *_digit = _start;
                    while (std::isdigit(*_digit) && *_digit != '}' && *_digit != '.')
                        ++_digit;
                    _width = std::stoi(std::string(_start, _digit));
                    _start = --_digit;
                } else if (*_start == '.') {
                    char const *_digit = ++_start;
                    while (std::isdigit(*_digit) && *_digit != '}')
                        ++_digit;
                    _precision = std::stoi(std::string(_start, _digit));
                    flags &= ~std::ios::floatfield;
                    flags |= std::ios::fixed;
                    _start = --_digit;
                } else {
                    return 0;
                }
                break;
        }
        ++_start;
    }

    out.flags(flags);
    out << std::setw(_width) << std::setprecision(_precision) << arg;
    out.flags(_prev_flags);
    return _end - fmt + 1;
}

/*
 * fmt -> similar to c++ format library, but only supports following flags,
 * 1. width
 * 2. pecision
 * 3. show base
 * 4. hex, dec, oct
 */
template <typename T, typename... Ts>
void _i_print(std::ostream &out, char const *fmt, T arg1, Ts... args) {
    for (; *fmt != '\0'; ++fmt) {
        switch (*fmt) {
            case '%':
                ++fmt;
                out << *fmt;
                break;
            case '{':
                {
                    ++fmt;
                    i32 char_count = _i_format_print(out, fmt, arg1);
                    if (char_count == 0) {
                        out << '{';
                        break;
                    }
                    _i_print(out, fmt + char_count, args...);
                    return;
                }
            default:
                out << *fmt;
                break;
        }
    }
}

/*
 * fmt -> { format },
 * where format can include,
 *  1. base (d | x | o | X | O)
 *  2. show base (#)
 *  3. width (any number at any place. e.g. "{#100x}" or "{10#X}")
 *  4. precision (any number at any place)
 */
template <typename... Ts>
void print(char const *fmt, Ts... args) {
    _i_print(std::cout, fmt, args...);
}

template <typename... Ts>
std::string format(char const *fmt, Ts... args) {
    std::ostringstream out;
    _i_print(out, fmt, args...);
    return out.str();
}


namespace step {
class IReader {
public:
    IReader() = default;
    virtual ~IReader() = default;
    virtual std::string read() = 0;

    IReader(IReader const &freader) = delete;
    IReader(IReader &&freader) = delete;
    IReader &operator=(IReader const &other) = delete;
    IReader &operator=(IReader &&other) = delete;
};

class FileReader: public IReader {
public:
    static constexpr size_t const BUFFER_SIZE = 2048; // 2KB or half disk block
public:
    explicit FileReader(std::string fname);
    virtual ~FileReader() override;

    virtual std::string read() override;
private:
    std::ifstream _in;
    std::size_t _available_chars;
};
} // namespace step

/* Constructor */
step::FileReader::FileReader(std::string fname)
    : IReader() {
    std::string absolute_path = std::filesystem::absolute(fname);
    if (!std::filesystem::exists(absolute_path)) {
        // Error: E005
    }

    _in.open(fname, std::ios::in);
    /* Determine the total number of characters to read */
    if (_in.is_open()) {
        _in.seekg(0, std::ios::end);
        _available_chars = _in.tellg();
        _in.seekg(0, std::ios::beg);
    } else { 
        // Error: E006
    }
}

step::FileReader::~FileReader() {
    if (_in.is_open()) {
        _in.close();
    }
}

std::string step::FileReader::read() {
    if (_in.is_open() && _available_chars > 0) {
        std::size_t read_size = std::min(BUFFER_SIZE, _available_chars);

        /* Read read_size Bytes of data */
        std::string buffer(read_size, '\0');
        _in.read(&buffer[0], read_size);

        /* adjusting available character count */
        _available_chars -= read_size;
        return buffer;
    }
    return E_OI;
}

namespace step {
class LineReader: public IReader {
public:
    explicit LineReader(std::unique_ptr<IReader> &&reader);
    virtual ~LineReader() override = default;

    virtual std::string read() override;
private:
    std::unique_ptr<IReader> _reader;
    std::vector<std::string> _lines;
    std::size_t _cur_line;
};
} // namespace step

step::LineReader::LineReader(std::unique_ptr<IReader> &&reader)
    : _reader(std::move(reader)), _cur_line{0}
{
    /* Read 4KB or 1 disk block at a time (2 * read()) */
    std::string buffer1(_reader->read());
    std::string buffer2(_reader->read());
    while (buffer1 != E_OI) {
        size_t index = 0;
        /* If the read operation ended at the middle of a line for buffer1 then complete the line in buffer1 */
        if (buffer1.back() != '\n' && (buffer2 != E_OI && buffer2.front() != '\n')) {
            while (buffer2.at(index) != '\n' && index < buffer2.length()) {
                buffer1.push_back(buffer2.at(index));
                ++index;
            }
            buffer1.append("\n");
            ++index;
        }

        std::istringstream in(buffer1);
        std::string line;
        while (in.good()) {
            std::getline(in, line);
            if (in.eof() && line.empty()) 
                break;
            if (line.empty()) 
                continue; // Skip empty lines
            _lines.emplace_back(line + "\n");
        }

        /* If we have copied all the contents of buffer2 to buffer1,
         * then read 2 new buffers
         * else copy rest of the contents of buffer2 to buffer1 and read another 2KB in buffer2 */
        if (buffer2.length() - index == 0) {
            buffer1.assign(_reader->read());
            buffer2.assign(_reader->read());
        } else {
            buffer1.assign(buffer2.begin() + index, buffer2.end());
            buffer2.assign(_reader->read());
        }
    }
}

std::string step::LineReader::read() {
    if (_cur_line >= _lines.size()) return E_OI;
    return _lines.at(_cur_line++);
}


namespace step {
enum class TokenType: u8 {
    miscellaneous, e_io,
    integer, FLOAT,
    // STRING, IDENTIFIER,
    plus, hyphen,
    star, slash,
    modulous,
    lparen, rparen,
    less_than, greater_than,
    less_than_equal, greater_than_equal,
    equal_equal, not_equal,
    logical_or, logical_and, logical_not,
    newline,

    total_tokens,
};

std::string tokentype_to_string[size_t(TokenType::total_tokens)] = {
    "miscellaneous", "e_io",
    "integer", "FLOAT",
    // "STRING", "IDENTIFIER",
    "plus", "hyphen",
    "star", "slash",
    "modulous",
    "lparen", "rparen",
    "less_than", "greater_than",
    "less_than_equal", "greater_than_equal",
    "equal_equal", "not_equal",
    "logical_or", "logical_and", "logical_not",
    "newline", 
};

struct Token {
    using value_type = literal_type;

    std::string _fname;
    value_type _token;
    size_t _line;
    size_t _col;
    TokenType  _kind;
};

std::string to_string(Token const &token);
} // namespace step

std::string step::to_string(Token const &token) {
    auto const &tok = token._token;
    if (tok.index() == STR_VARIANT_INDEX) {
        return std::get<STR_VARIANT_INDEX>(tok);
    } else if (tok.index() == INT_VARIANT_INDEX) {
        return format("{}", std::get<INT_VARIANT_INDEX>(tok));
    } else {
        return format("{.6}", std::get<FLT_VARIANT_INDEX>(tok));
    }
}

namespace step {
class Tokenizer {
public:
    Tokenizer(std::unique_ptr<IReader> &&reader,
              std::string fname);
    ~Tokenizer() = default;

    Token const &peek(std::size_t offset = 0);
    Token const &next();
private:
    Token make_token(TokenType  kind,
                     step::Token::value_type token,
                     std::size_t col);
    void skipws();
    bool is_space(char c);
    bool is_digit(char c);
    bool is_ident(char c); // is identifier
    bool is_greater(std::string const &s, long long i);
    void skip_to_newline();
    char nextc();
    char peekc();

    void process();
    std::size_t process_integer();
    void process_number(bool positive);
private:
    std::unique_ptr<IReader> _reader;
    std::vector<Token> _tokens;
    std::string _fname;
    std::size_t _cur_token;

    std::string line;
    std::size_t line_no;
    std::size_t token_begin;
    std::size_t forward;

    step::Token LEXEME_E_IO;
};
} // namespace step

step::Tokenizer::Tokenizer(std::unique_ptr<IReader> &&reader,
                           std::string fname)
    : _reader(std::move(reader)), 
      _fname{fname}, 
      _cur_token{0},
      LEXEME_E_IO{_fname, E_OI, 0, 1, step::TokenType::e_io}
{
    process();
    LEXEME_E_IO._line = line_no;
}

step::Token const &step::Tokenizer::peek(std::size_t offset) {
    if (_cur_token + offset >= _tokens.size() || _cur_token + offset < 0) {
        return step::Tokenizer::LEXEME_E_IO;
    }
    return _tokens.at(_cur_token + offset);
}

step::Token const &step::Tokenizer::next() {
    if (_cur_token >= _tokens.size()) return _tokens.back();
    return _tokens.at(_cur_token++);  
}

char step::Tokenizer::nextc() {
    if (forward >= line.length()) return '\0';
    line_no += (line.at(forward) == '\n');
    return line.at(forward++);
}

char step::Tokenizer::peekc() {
    if (forward >= line.length()) return '\0';
    return line.at(forward);
}

step::Token step::Tokenizer::make_token(step::TokenType kind,
                                        step::Token::value_type token,
                                        std::size_t col) {
    return Token{
        _fname,
        token,
        line_no,
        col,
        kind,
    };
}

void step::Tokenizer::process() {
    line_no = 1;
    while ((line = _reader->read()) != E_OI) {
        token_begin = forward = 0;
        while (peekc() != '\0') {
            skipws();
            token_begin = forward; // If spaces were skipped, then reset the token_begin
            char c = nextc();
            switch (c) {
            case '\0':
                break;
            case '\n':
                _tokens.emplace_back(make_token(TokenType::newline, 
                                                "\\n", 
                                                token_begin));
                break;
            case '+':
                _tokens.emplace_back(make_token(TokenType::plus, 
                                                "+", 
                                                token_begin));
                break;
            case '-':
                /* If this is a negative number */
                if (is_digit(peekc())) {
                    process_number(false);
                } else {
                    _tokens.emplace_back(make_token(TokenType::hyphen, 
                                                    "-", 
                                                    token_begin));
                }
                break;
            case '*':
                _tokens.emplace_back(make_token(TokenType::star, 
                                                "*", 
                                                token_begin));
                break;
            case '/':
                _tokens.emplace_back(make_token(TokenType::slash, 
                                                "/", 
                                                token_begin));
                break;
            case '%':
                _tokens.emplace_back(make_token(TokenType::modulous, 
                                                "%", 
                                                token_begin));
                break;
            case '(':
                _tokens.emplace_back(make_token(TokenType::lparen, 
                                                "(", 
                                                token_begin));
                break;
            case ')':
                _tokens.emplace_back(make_token(TokenType::rparen, 
                                                ")", 
                                                token_begin));
                break;
            case '<':
                if (peekc() == '=') {
                    nextc();
                    _tokens.emplace_back(make_token(TokenType::less_than_equal, 
                                                    "<=", 
                                                    token_begin));
                } else {
                    _tokens.emplace_back(make_token(TokenType::less_than, 
                                                    "<", 
                                                    token_begin));
                }
                break;
            case '>':
                if (peekc() == '=') {
                    nextc();
                    _tokens.emplace_back(make_token(TokenType::greater_than_equal, 
                                                    ">=", 
                                                    token_begin));
                } else {
                    _tokens.emplace_back(make_token(TokenType::greater_than, 
                                                    ">", 
                                                    token_begin));
                }
                break;
            case '=':
                if (peekc() == '=') {
                    nextc();
                    _tokens.emplace_back(make_token(TokenType::equal_equal, 
                                                    "==", 
                                                    token_begin));
                }
                break;
            case '!':
                if (peekc() == '=') {
                    nextc();
                    _tokens.emplace_back(make_token(TokenType::not_equal, 
                                                    "!=", 
                                                    token_begin));
                }
                break;
            // case '"':
                // process_string();
                // break;
            default:
                if (is_digit(c)) {
                    process_number(true);
                // } else if (is_ident(c)) {
                    // process_identifier();
                } else {
                    // Error: E007
                    skip_to_newline();
                }
                break;
            }
        }
    }
    _tokens.emplace_back(make_token(TokenType::e_io, 
                                      E_OI, 
                                      token_begin));
}

void step::Tokenizer::skipws() {
    while (is_space(peekc()))
        nextc();
}

bool step::Tokenizer::is_space(char c) {
    return std::string(" \t\r\v\a\b").find_first_of(c) != std::string::npos;
}

bool step::Tokenizer::is_digit(char c) {
    return c >= 48 && c <= 57;
}

bool step::Tokenizer::is_ident(char c) {
    return (c >= 65 && c <= 91) || (c >= 97 && c <= 123);
}

bool step::Tokenizer::is_greater(std::string const &s, long long i) {
    std::string t{std::to_string(i)};
    return s.length() > t.length() || (s.length() == t.length() && s > t);
}

void step::Tokenizer::skip_to_newline() {
    while (peekc() != '\n' && peekc() != '\0')
        nextc();
    if (peekc() == '\n')
        nextc(); // skip the newline also
}

void step::Tokenizer::process_number(bool positive) {
    process_integer();
    if (peekc() == '.') {
        nextc();
        if (peekc() == '\0') {
            // Error: unexpected end-of-input. expected digit after radix point
            // Error: E008
            return;
        }

        if (!is_digit(peekc())) {
            // Error: a digit must follow a radix point
            // Error: E009
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
                // Error: E010
                return;
            }

            std::size_t exponent_begin = forward;
            bool positive_exponent = true;
            if (peekc() == '+' || peekc() == '-') {
                positive_exponent = (nextc() == '+');
            }

            if (!is_digit(peekc())) {
                // Error: expected digit
                // Error: E011
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
            if ((positive_exponent && is_greater(exponent, std::numeric_limits<step_float_type>::max_exponent10)) ||
                (!positive_exponent && is_greater(exponent, std::numeric_limits<step_float_type>::min_exponent10))) {
                // Error: E015
                skip_to_newline();
                return;
            }
        }

        std::string number;
        /* Extract the float part without the underscores */
        std::copy_if(line.begin() + token_begin, 
                     line.begin() + forward, 
                     std::back_inserter(number),
                     [](char c) { return c != '_'; });
        _tokens.emplace_back(
            make_token(
                TokenType::FLOAT,
                std::stod(number),
                token_begin+1
            )
        );
    } else {
        std::string integer;
        /* Extract the integer part without the underscores */
        std::copy_if(line.begin() + token_begin, 
                     line.begin() + forward, 
                     std::back_inserter(integer),
                     [](char c) { return c != '_'; });
        
        /* Check if the integer exceed the maximum digit count or 
         * if the integer is greater than the maximum integer (for positive) or
         * if the integer is smaller than the maximum integer (for negative)
         * if so, then generate error */
        if ((integer.length() - (integer.front() == '+' || integer.front() == '-')) > std::numeric_limits<step_int_type>::digits10 ||
            (positive && is_greater(integer, MAX_V<step_int_type>)) ||
            (!positive && is_greater(integer, MIN_V<step_int_type>))) {
            // Error: E014
            skip_to_newline();
            return;
        }

        _tokens.emplace_back(make_token(
                TokenType::integer,
                std::stoll(integer), 
                token_begin+1 
            )
        );
    }
}


std::size_t step::Tokenizer::process_integer() {
    if (peekc() == '+' || peekc() == '-')
        nextc();

    std::size_t digit_count = 0;
    char c;
    while ((is_digit(c = peekc()) || c == '_')) {
        nextc();
        digit_count += (c != '_');
    }

    /* If the number finished with a digit or not */
    if (!is_digit(line.at(forward-1))) {
        // Error: misplaced separator
        // Error: E016
    }
    return digit_count;
}

int main() {
}
