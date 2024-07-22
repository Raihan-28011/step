/*
 * created by: shunnobir
 * date: 22/07/2024
 */

#include "cmd/ArgumentParser.hpp"

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
    }
}
