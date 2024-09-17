/*
 * created by: shunnobir
 * date: 13/09/2024
 */


#include <stdio.h>

#include "config.h"
#include "cmdopts.h"

static char const *version_message =
"Step " STEP_VERSION " Copyright (C) 2024-2024 <https://docs.step.vercel.app>\n\
For more information, visit: <https://github.com/shunnobir/step.git>\n\
";

static char const *usage = "\
Usage: step [OPTIONS...] FILE\n\
";

static char const *help = "\
  --help          show this help\n\
  --version       show version information\n\
";

static void Step_Print_Help(void) {
    printf("%s%s\n%s", usage, help, version_message);
}

static void Step_Print_Version(void) {
    printf("%s", version_message);
}

int main(int argc, char **argv) {
    Step_CmdOptions options = Step_Read_Cmd_Options(argc, argv);
    if (options.show_help) {
        Step_Print_Help();
        return 0;
    }

    if (options.show_version) {
        Step_Print_Version();
        return 0;
    }

    printf("file: %s\n", options.filename);
    return 0;
}
