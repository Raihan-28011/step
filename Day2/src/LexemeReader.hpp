/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __LEXEMEREADER_HPP__
#define __LEXEMEREADER_HPP__

#include "IReader.hpp"
#include <memory>
#include <vector>

namespace Step {
    class LexemeReader: public IReader {
    public:
        /* Constructors */
        explicit LexemeReader(std::unique_ptr<IReader> &&reader);

        /* Deleted constructors and special member functions */
        LexemeReader(LexemeReader const &freader) = delete;
        LexemeReader(LexemeReader &&freader) = delete;
        LexemeReader &operator=(LexemeReader const &other) = delete;
        LexemeReader &operator=(LexemeReader &&other) = delete;

        /* Destructor */
        virtual ~LexemeReader() override;

        /* Public overridden methods */
        virtual std::string read() override;
    private:
        std::unique_ptr<IReader> _reader;
        std::vector<std::string> _lexemes;
        std::size_t _cur_lexeme;
    };
}

#endif // __LEXEMEREADER_HPP__

