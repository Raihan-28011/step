//
// Created by raihan on 7/9/22.
//

//#ifndef STEP_MAIN_H
//#define STEP_MAIN_H
#pragma once

#include "lexer.h"

namespace step {
    void read_file(char const *fname, string &cont) {
        std::ifstream file(fname, std::ios::in | std::ios::binary);

        if (!file.is_open()) {
            ErrorManager::precompile_error("can not open file '" + string(fname) + "'");
        }

        file.seekg(0, std::ios::end);
        auto fileSize = as<i64>(file.tellg());
        file.seekg(0, std::ios::beg);

        char *content = new char[fileSize+1];
        file.read(content, fileSize);
        content[fileSize] = '\0';

        cont = string(content);
        delete[] content;
        file.close();
    }
}


//#endif //STEP_MAIN_H
