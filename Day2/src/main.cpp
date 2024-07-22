/*
 * created by: shunnobir
 * date: 22/07/2024
 */

#include "FileReader.hpp"
#include "IReader.hpp"
#include "LexemeReader.hpp"
#include "LineReader.hpp"
#include "cmd/ArgumentParser.hpp"
#include <iomanip>
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    Step::ArgumentParser arg_parser{argc, argv};
    arg_parser.add_rule("-in", 
                        "execute the <INPUT-FILE>. <INPUT-FILE> must be a step file.", 
                        true, 
                        "input-file", 
                        "--input-file");
    if (arg_parser.parse()) {
        /* If help argument was passed */
        if (arg_parser.get("-h") == "true") {
            arg_parser.help();
            return 0;
        }

        Step::LexemeReader reader(
                std::make_unique<Step::LineReader>(
                    std::make_unique<Step::FileReader>(
                        arg_parser.get("input-file")
                    )
                )
        );
        std::string lexeme(reader.read());
        while (lexeme != Step::IReader::E_OI) {
            std::cout << '<' << lexeme << "> ";
            lexeme.assign(reader.read());
        }
    }
}
