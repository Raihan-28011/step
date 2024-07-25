/*
 * created by: shunnobir
 * date: 22/07/2024
 */

#include "ErrorManager.hpp"
#include "FileReader.hpp"
#include "Lexeme.hpp"
#include "LexemeReader.hpp"
#include "LineReader.hpp"
#include "NoInputFile.hpp"
#include "NotValidFile.hpp"
#include "cmd/ArgumentParser.hpp"
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    Step::ArgumentParser arg_parser{argc, argv};
    // TODO: add rule validators in ArgumentParser
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

        if (arg_parser.get("input-file").empty()) {
            Step::ErrorManager::instance()
                .add(std::make_unique<Step::NoInputFile>())
                .dump();
            arg_parser.help();
            return -1;
        }

        /* Check file extension validity */
        {
            std::string fname = arg_parser.get("input-file");
            std::size_t index = fname.find_last_of(".");
            if (index == std::string::npos || 
                fname.substr(index+1) != "step") {
                Step::ErrorManager::instance()
                    .add(std::make_unique<Step::NotValidFile>(fname))
                    .dump();
                return -1;
            }
        }

        Step::LexemeReader reader(
            std::make_unique<Step::LineReader>(
                std::make_unique<Step::FileReader>(
                    arg_parser.get("input-file")
                )
            ),
            arg_parser.get("input-file")
        );

        if (Step::ErrorManager::error_occured) {
            Step::ErrorManager::dump();
            return -1;
        }

        Step::Lexeme lexeme(reader.next());
        while (lexeme._kind != Step::LexemeKind::E_OI) {
            std::cout << "Lexeme(" << lexeme._lexeme 
                      << ", " << lexeme._line 
                      << ":" << lexeme._col 
                      << ", " << Step::to_string(lexeme._kind) << ")\n";
            lexeme = reader.next();
        }
    }
}
