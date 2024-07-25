/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __LEXEMEREADER_HPP__
#define __LEXEMEREADER_HPP__

#include "IReader.hpp"
#include "Lexeme.hpp"
#include <memory>
#include <vector>

namespace Step {
    class LexemeReader: public IReader {
    public:
        /* Constructors */
        LexemeReader(std::unique_ptr<IReader> &&reader,
                     std::string fname);

        /* Deleted constructors and special member functions */
        LexemeReader(LexemeReader const &freader) = delete;
        LexemeReader(LexemeReader &&freader) = delete;
        LexemeReader &operator=(LexemeReader const &other) = delete;
        LexemeReader &operator=(LexemeReader &&other) = delete;

        /* Destructor */
        virtual ~LexemeReader() override;

        /* Public Methods */
        Lexeme const &peek(std::size_t offset = 0);
        Lexeme const &next();

        /* Public overridden methods */
        virtual std::string read() override;
    private:
        /* Private methods */
        void skipws(std::string const &line, std::size_t &pos);
        bool is_space(char c);
        bool is_digit(char c);
        bool is_ident(char c); // is identifier
        void skip_to_newline(std::string_view line, std::size_t &pos);

        void process();
        std::size_t process_integer(std::string const &line, std::size_t &forward, std::size_t line_no);
        void process_float(std::string const &line, std::size_t &forward, std::size_t &lexeme_begin, std::size_t line_no);
    private:
        std::unique_ptr<IReader> _reader;
        std::vector<Lexeme> _lexemes;
        std::string _fname;
        std::size_t _cur_lexeme;
    };
}

#endif // __LEXEMEREADER_HPP__


