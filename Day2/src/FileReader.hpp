/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __FILEREADER_HPP__
#define __FILEREADER_HPP__

#include "IReader.hpp"
#include <fstream>
#include <string>

namespace Step {
    class FileReader: public IReader {
    public:
        /* Constants */
        static constexpr std::size_t const BUFFER_SIZE = 2048; // 2KB or half disk block
    public:
        /* Constructors */
        explicit FileReader(std::string fname);

        /* Deleted constructors and special member functions */
        FileReader(FileReader const &freader) = delete;
        FileReader(FileReader &&freader) = delete;
        FileReader &operator=(FileReader const &other) = delete;
        FileReader &operator=(FileReader &&other) = delete;

        /* Destructor */
        virtual ~FileReader() override;

        /* Public overridden methods */
        virtual std::string read() override;

    private:
        std::ifstream _in;
        std::size_t _fsize;
    };
}

#endif // __FILEREADER_HPP__
