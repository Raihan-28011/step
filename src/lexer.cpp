//
// Created by raihan on 7/9/22.
//

#include "lexer.h"

namespace step {
    Lexer::Lexer(string const &f, string const &fn)
        : file{f},
          fname{fn},
          text_size{as<i32>(f.size())},
          errorManger(::errorManager)
    {
        tokenize();
    }

    void Lexer::tokenize() {
        skip_whitespace();
        char c = next_char();
        while (!is_eof(c)) {
            switch (c) {
                case '(': add_t(t_lparen, "("); break;
                case ')': add_t(t_rparen, ")"); break;
                case '{': add_t(t_lbrace, "{"); break;
                case '}': add_t(t_rbrace, "}"); break;
                case '[': add_t(t_lsqbrace, "["); break;
                case ']': add_t(t_rsqbrace, "]"); break;
                case '.': add_t(t_dot, "."); break;
                case ';': add_t(t_semicolon, ";"); break;
                case ',': add_t(t_comma, ","); break;
                case '=':
                    if (peek_char() == '=') {
                        next_char();
                        add_t(t_equalequal, "==");
                        next_char();
                    } else {
                        add_t(t_equal, "=");
                    }
                    break;
                case '"': string_token(c); break;
                case '#': comment_token(c); break;
                case '!':
                    if (peek_char() == '=') {
                        next_char();
                        add_t(t_notequal, "!=");
                    } else {
                        add_t(t_bang, "!");
                    }
                    break;
                case '*':
                    if (peek_char() == '*') {
                        next_char();
                        add_t(t_starstar, "**");
                    } else {
                        add_t(t_star, "*");
                    }
                    break;
                case '/': add_t(t_slash, "/"); break;
                case '%': add_t(t_modulus, "%"); break;
                case '|':
                    if (peek_char() == '|') {
                        next_char();
                        add_t(t_logicalor, "||");
                    }
                    break;
                case '&':
                    if (peek_char() == '&') {
                        next_char();
                        add_t(t_logicaland, "&&");
                    }
                    break;
                case '<':
                    if (peek_char() == '=') {
                        next_char();
                        add_t(t_lessequal, "<=");
                    } else {
                        add_t(t_less, "<");
                    }
                    break;
                case '>':
                    if (peek_char() == '=') {
                        next_char();
                        add_t(t_greaterequal, ">=");
                    } else {
                        add_t(t_greater, ">");
                    }
                    break;
                case '-':
                    // TODO: fix this negative number
                    if (std::isdigit(peek_char()))
                        num_token(c);
                    else
                        add_t(t_minus, "-");
                    break;
                case '+':
                    if (std::isdigit(peek_char()))
                        num_token(c);
                    else
                        add_t(t_plus, "+");
                    break;
                default:
                    if (std::isdigit(c)) num_token(c);
                    else if (std::isalnum(c) || c == '_') indentifier_token(c);
                    else unknown_token(c);
                    break;
            }
            skip_whitespace();
            c = next_char();
        }
        add_t(t_eof, "eof");
    }

    void Lexer::skip_whitespace() {
        while (std::isspace(peek_char()) && !is_eof(peek_char())) {
            if (next_char() == '\n') {
                ++line;
                col = 0;
            }
        }
    }

    char Lexer::next_char() {
        if (text_index >= text_size)
            return eof_char();
        ++col;
        return file.at(text_index++);
    }

    char Lexer::peek_char() {
        if (text_index >= text_size)
            return eof_char();
        return file.at(text_index);
    }

    void Lexer::add_t(TokenKind tok, string &&s) {
        tokens.push_back(Token{tok, std::move(s), line, col});
    }

    void Lexer::num_token(char c) {
        string num{};
        if (c == '-' || c == '+') {
            num += (c == '-' ? "-" : "");
            c = next_char();
        }

        num += c;

        if (is_eof(c)) {
            errorManger->compilation_error("unexpected eof encounter", line, col);
        }

        bool octal = (c == '0' && std::isdigit(peek_char()));
        bool hex = false, bin = false;
        if (std::isalpha(peek_char())) {
            c = peek_char();
            char prefix = std::tolower(c);
            num += prefix;
            hex = ((num.front() == '-' ? num == "-0x" : num == "0x"));
            bin = ((num.front() == '-' ? num == "-0b" : num == "0b"));
            bool valid_num = (hex || bin);

            if (!valid_num) {
                errorManger->compilation_error("invalid number prefix", line, col);
            }

            num.pop_back();
            num += next_char();
        }

        bool has_period = false;
        c = peek_char();
        while ((hex && std::isalnum(c)) || (std::isdigit(c) || c == '.') && !is_eof(c)) {
            c = next_char();
            if (bin && c != '1' && c != '0') {
                errorManger->compilation_error("invalid binary number", line, col);
            }

            if (octal && c > '7') {
                errorManger->compilation_error("invalid octal number", line, col);
            }

            if (hex && std::tolower(c) > 'f') {
                errorManger->compilation_error("invalid hex number", line, col);
            }

            if (c == '.') {
                if (octal || hex || bin) {
                    errorManger->compilation_error((octal ? "octal floating point number"
                                                   :hex ? "hex floating point number"
                                                   :"binary floaing point number"), line, col);
                }
                if (has_period) {
                    errorManger->compilation_error("multiple periods in a number", line, col);
                }
                has_period = true;
            }

            num += c;
            c = peek_char();
        }

        add_t(t_num, std::move(num));
    }

    void Lexer::indentifier_token(char c) {
        string ident{};
        ident += c;

        c = peek_char();
        while ((std::isalnum(c) || c == '_') && !is_eof(c)) {
            ident += next_char();
            c = peek_char();
        }

        TokenKind kind{t_ident};
        if (ident == "def")
            kind = t_def;
        else if (ident == "true")
            kind = t_true;
        else if (ident == "false")
            kind = t_false;
        else if (ident == "return")
            kind = t_return;
        else if (ident == "if")
            kind = t_if;
        else if (ident == "else")
            kind = t_else;
        else if (ident == "elif")
            kind = t_elif;

        add_t(kind, std::move(ident));
    }

    void Lexer::unknown_token(char c) {
        errorManger->compilation_error("unknown token", line, col);
    }

    void Lexer::string_token(char c) {
        string str{};
        auto save_line = line;
        auto save_col = col;
        c = peek_char();
        while (c != '"' && !is_eof(c)) {
            c = next_char();
            if (c == '\n') {
                errorManger->compilation_error("multiline string is not supported", line, col);
            }
            if (c == '\\') {
                c = next_char();
                switch (c) {
                    case 'n': c = '\n'; break;
                    case 't': c = '\t'; break;
                    case 'v': c = '\v'; break;
                    case 'a': c = '\a'; break;
                    case '\\': c = '\\'; break;
                    case '\'': c = '\''; break;
                    case '\"': c = '"'; break;
                    case '\b': c = '\b'; break;
                    case '\r': c = '\r'; break;
                    default:
                        errorManger->compilation_error("invalid escape sequence", line, col);
                        break;
                }
            }
            str += c;
            c = peek_char();
        }

        if (is_eof(c))
            errorManger->compilation_error("unterminated string", save_line, save_col);

        if (c == '"')
            next_char();

        add_t(t_string, std::move(str));
    }

    void Lexer::comment_token(char c) {
        c = peek_char();
        while (c != '\n' && !is_eof(c)) {
            next_char();
            c = peek_char();
        }
    }
} // step