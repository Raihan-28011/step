/*
 * created by: shunnobir
 * date: 18/09/2024
 */

#include "cmdopts.h"
#include <string.h>

Step_CmdOptions Step_Read_Cmd_Options(int argc, char **argv) {
    Step_CmdOptions options;
    char **cur_option = argv + 1;
    while (cur_option - argv < argc) {
        if (strncmp(*cur_option, "--help", 6) == 0) {
            options.show_help = 1;
            break;
        } else if (strncmp(*cur_option, "--version", 9) == 0) {
            options.show_version = 1;
            break;
        } else {
            break;
        }
        ++cur_option;
    }
    
    options.filename = *cur_option;
    return options;
}
