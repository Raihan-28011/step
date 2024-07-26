/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#include "FileReader.hpp"
#include "ErrorManager.hpp"
#include "IError.hpp"
#include "IReader.hpp"
#include "PreCompilationError.hpp"
#include <algorithm>
#include <cerrno>
#include <filesystem>
#include <ios>

Step::FileReader::FileReader(std::string fname)
{
    std::string absolute_path = std::filesystem::absolute(fname);
    if (!std::filesystem::exists(absolute_path)) {
        Step::ErrorManager::instance()
            .add(std::make_unique<Step::PreCompilationError>(IError::ErrorCode::E005, fname))
            .dump(true, ENOENT); 
    }

    _in.open(fname, std::ios::in);
    /* Determine the total file size */
    if (_in.is_open()) {
        _in.seekg(0, std::ios::end);
        _fsize = _in.tellg();
        _in.seekg(0, std::ios::beg);
    } else { 
        Step::ErrorManager::instance()
            .add(std::make_unique<Step::PreCompilationError>(IError::ErrorCode::E006, fname))
            .dump(true, EACCES);
    }
    std::filesystem::path path;
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
