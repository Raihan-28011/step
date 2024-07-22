/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "FileReader.hpp"
#include "IReader.hpp"
#include <algorithm>
#include <ios>

Step::FileReader::FileReader(std::string fname)
    : _in(fname, std::ios::in)
{
    if (_in.is_open()) {
        _in.seekg(0, std::ios::end);
        _fsize = _in.tellg();
        _in.seekg(0, std::ios::beg);
    }
}

Step::FileReader::~FileReader() {
    if (_in.is_open()) {
        _in.close();
    }
}

std::string Step::FileReader::read() {
    if (_in.is_open() && _fsize > 0) {
        std::size_t read_size = std::min(BUFFER_SIZE, _fsize);
        _fsize -= read_size;

        /* Read read_size Bytes of data */
        std::string buffer(read_size, '\0');
        _in.read(&buffer[0], read_size);
        return buffer;
    }
    return IReader::E_OI;
}
