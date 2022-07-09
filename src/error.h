//
// Created by raihan on 7/9/22.
//

//#ifndef STEP_ERROR_H
//#define STEP_ERROR_H

#pragma once

#include <cstdlib>
#include "common.h"

namespace step {
    class ErrorManager {
    public:
        ErrorManager(string const &cont, string const &fn)
            : content{cont}, fname{fn}
        {
            store_lines();
        }

        ~ErrorManager() = default;

        void compilation_error(string msg, int line, int col) {
            std::cerr << fname
                      << ":[" << line << ":" << col << "] error: "
                      << msg << '\n';
            std::cerr << string(content.begin()+lines[line-1].first, content.begin()+lines[line-1].second) << '\n'
                      << string(col-1, ' ') << "^\n";
            std::exit(-1);
        }

        static void precompile_error(string msg) {
            std::cerr << msg << '\n' << "compilation terminated\n";
            std::exit(-1);
        }

        static void help() {
            // TODO: add help messages
            std::cerr << "help\n";
            std::exit(-1);
        }

    private:
        vector<std::pair<int, int>> lines;
        string const &content;
        string fname;


        void store_lines() {
            if (content.empty())
                return;

            int prev_index = 0, cur_index = 0;
            for (auto const &c: content) {
                if (c == '\n') {
                    lines.push_back({prev_index, cur_index});
                    prev_index = cur_index+1;
                }

                ++cur_index;
            }

            lines.push_back({prev_index, cur_index});
        }
    };
}

//#endif //STEP_ERROR_H
