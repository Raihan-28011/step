/*
 * created by: shunnobir
 * date: 18/09/2024
 */

#ifndef _STEP_CMDOPTS_H_
#define _STEP_CMDOPTS_H_

#include "typedefs.h"

typedef struct {
    Step_Boolean show_help;
    Step_Boolean show_version;
    char *filename;
} Step_CmdOptions;

Step_CmdOptions Step_Read_Cmd_Options(int argc, char **argv);


#endif // _STEP_CMDOPTS_H_
