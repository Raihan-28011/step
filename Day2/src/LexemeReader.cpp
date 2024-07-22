/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "LexemeReader.hpp"
#include "IReader.hpp"
#include <memory>
#include <sstream>
#include <string>

Step::LexemeReader::LexemeReader(std::unique_ptr<IReader> &&reader)
    : _reader(std::move(reader)), _cur_lexeme{0}
{
    std::string buffer(_reader->read());
    while (buffer != IReader::E_OI) {
        std::istringstream in(buffer);
        std::string lexeme;
        while (in.good()) {
            in >> lexeme;
            if (lexeme.empty()) continue;
            _lexemes.emplace_back(std::move(lexeme));
        }
        _lexemes.emplace_back("\n");
        buffer = _reader->read();
    }
}

Step::LexemeReader::~LexemeReader() = default;

std::string Step::LexemeReader::read() {
    if (_cur_lexeme >= _lexemes.size()) return IReader::E_OI;
    return _lexemes.at(_cur_lexeme++);
}


