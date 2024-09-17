//
// Created by raihan on 7/8/22.
//

#include "main.h"

int main(int argc, char **argv) {
    std::cout << std::fixed << std::setprecision(4);
    std::cerr << std::fixed << std::setprecision(4);
    if (argc < 2) {
        step::ErrorManager::precompile_error("expected a file as an argument");
    } else if (argc >= 2) {
        string extension{};
        auto size = std::strlen(argv[1]);
        while (argv[1][size-1] != '.') {
            extension = argv[1][size-1] + extension;
            --size;
        }

        if (size == 0 || extension != "step") {
            step::ErrorManager::precompile_error("invalid file. Expected '.step' extension file");
        }
    }
    string cont{};
    step::read_file(argv[1], cont);
    step::initialize_step(cont, string(argv[1]));
    step::Parser parser(cont, argv[1]);
    parser.parse();
}