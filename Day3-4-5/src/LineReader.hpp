/*
 * created by: shunnobir
 * date: 23/07/2024
 */

#ifndef __LINEREADER_HPP__
#define __LINEREADER_HPP__

#include "IReader.hpp"
#include <memory>
#include <vector>

namespace Step {
    class LineReader: public IReader {
    public:
        /* Constructors */
        explicit LineReader(std::unique_ptr<IReader> &&reader);

        /* Deleted constructors and special member functions */
        LineReader(LineReader const &freader) = delete;
        LineReader(LineReader &&freader) = delete;
        LineReader &operator=(LineReader const &other) = delete;
        LineReader &operator=(LineReader &&other) = delete;

        /* Destructor */
        virtual ~LineReader() override;

        /* Public overridden methods */
        virtual std::string read() override;
    private:
        std::unique_ptr<IReader> _reader;
        std::vector<std::string> _lines;
        std::size_t _cur_line;
    };
}

#endif // __LINEREADER_HPP__
