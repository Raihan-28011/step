/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "LineReader.hpp"
#include "IReader.hpp"
#include <memory>
#include <sstream>
#include <string>

Step::LineReader::LineReader(std::unique_ptr<IReader> &&reader)
    : _reader(std::move(reader)), _cur_line{0}
{
    std::string buffer1(_reader->read());
    std::string buffer2(_reader->read());
    while (buffer1 != IReader::E_OI) {
        std::size_t index = 0;
        /* If the read operation ended at the middle of a line then complete the line in buffer1 */
        if (buffer1.back() != '\n' && (buffer2 != IReader::E_OI && buffer2.front() != '\n')) {
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
            if (in.eof() && line.empty()) break;
            _lines.emplace_back(line + "\n");
        }

        if (buffer2.length() - index == 0) {
            buffer1.assign(_reader->read());
            buffer2.assign(_reader->read());
        } else {
            buffer1.assign(buffer2.begin() + index, buffer2.end());
            buffer2.assign(_reader->read());
        }
    }
}

Step::LineReader::~LineReader() = default;

std::string Step::LineReader::read() {
    if (_cur_line >= _lines.size()) return IReader::E_OI;
    return _lines.at(_cur_line++);
}

