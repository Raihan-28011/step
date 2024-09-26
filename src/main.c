/*
 * created by: shunnobir
 * date: 19/09/2024
 * updated: 26/09/2024 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "typedefs.h"

/* Step memory management functions and macros */
#ifdef STEP_ARENA
#define Step_Alloc(type, n) (type*) Step_ArenaAlloc(sizeof(type) * n)
#define Step_Free(ptr) do { \
    if (!Step_IsNull(ptr)) { \
        Step_ArenaFree(ptr); \
        *ptr = NULL; \
    } \
} while(0)
#else
#define Step_Alloc(type, n) (type*) malloc(sizeof(type) * n)
#define Step_Free(ptr) do { \
    if (!Step_IsNull(ptr)) { \
        if (!Step_IsNull(*ptr)) { \
            free(*ptr); \
        } \
        *ptr = NULL; \
    } \
} while(0)
#endif

/* Important utility macros */
#define Step_TRUE 1
#define Step_FALSE 0

#define Step_GetGlobalError() strerror(errno)
#define Step_IsNull(ptr) (ptr == NULL)

#define Step_Fopen(name, mode) fopen(name, mode)
#define Step_Fclose(fp) (!Step_IsNull(fp) ? fclose(fp) : 0)

/* Step Status */
typedef enum {
    Step_Status_OK,
    Step_Status_ERROR,
} _Step_Status_Type;

typedef struct {
    _Step_Status_Type type;
    char const *error_msg;
    char const *header;
} Step_Status;

#define Step_StatusError(msg) \
    (Step_Status) { \
        .type = Step_Status_ERROR, \
        .error_msg = msg, \
        .header = "step:error", \
    }
#define Step_StatusOk()\
    (Step_Status) { \
        .type = Step_Status_OK, \
        .error_msg = NULL, \
        .header = "step", \
    }
#define Step_StatusIsError(status) status.type == Step_Status_ERROR
#define Step_StatusLog(status) fprintf(stderr, "%s: %s\n", status.header, status.error_msg)

/*
 * Static functions and global variables that are used for step's configurations,
 * version message, help message, and usage message.
 */
static char const *version_message =
"Step-" STEP_VERSION " Copyright (C) 2024-2024 <https://docs.step.vercel.app>\n\
For more information, visit: <https://github.com/shunnobir/step.git>\n\
";

static char const *usage = "\
Usage: step [OPTIONS...] FILE\n\
";

static char const *help = "\
  --help          show this help\n\
  --version       show version information\n\
";

static void Step_PrintHelp(void) {
    printf("%s%s\n%s", usage, help, version_message);
}

static void Step_PrintVersion(void) {
    printf("%s", version_message);
}

/* Command Line Options */
typedef struct {
    Step_Boolean show_help;
    Step_Boolean show_version;
    char *filename;
} Step_Args;

Step_Args Step_InitFromArgs(int argc, char **argv);
Step_Status Step_ExecuteOptions(Step_Args *args);

/* Step State */
typedef struct {
    Step_Args args;
    FILE *current_file;
} Step_State;

Step_State *Step_StateNew(void);
Step_State *Step_StateInitFromArgs(int argc, char **argv);
void Step_StateDelete(Step_State **state);

/* Functions that are used to read step files */
Step_Status Step_OpenFile(Step_File **file, char const *fname, char const *mode);
Step_Status Step_RunFile(Step_Args *args);

/* The Step main function */
int Step_Main(int argc, char **argv);

/* The C main functions */
int main(int argc, char **argv) {
    return Step_Main(argc, argv);
}

/* Command Line Options related function implementations */
Step_Args Step_InitFromArgs(int argc, char **argv) {
    Step_Args options = {
        .show_help = Step_FALSE,
        .show_version = Step_FALSE,
        .filename = NULL,
    };
    char **cur_option = argv + 1;
    while (cur_option - argv < argc) {
        if (strncmp(*cur_option, "--help", 6) == 0) {
            options.show_help = Step_TRUE;
            break;
        } else if (strncmp(*cur_option, "--version", 9) == 0) {
            options.show_version = Step_TRUE;
            break;
        } else {
            break;
        }
        ++cur_option;
    }
    
    if (cur_option - argv < argc) {
        options.filename = *cur_option;
    } else {
        options.filename = NULL;
    }
    return options;
}

Step_Status Step_ExecuteOptions(Step_Args *args) {
    if (args->show_help) {
        Step_PrintHelp();
        return Step_StatusOk();
    }

    if (args->show_version) {
        Step_PrintVersion();
        return Step_StatusOk();
    }

    if (Step_IsNull(args->filename)) {
        return Step_StatusError("noo file name provided");
    }

    return Step_StatusError("unknown uption");
}


/* Implementation of functions that are used to initialize and manage step state */
Step_State *Step_StateNew(void) {
    Step_State *new = Step_Alloc(Step_State, 1);
    new->current_file = NULL;
    return new;
}

Step_State *Step_StateInitFromArgs(int argc, char **argv) {
    Step_State *new_state = Step_StateNew();
    new_state->args = Step_InitFromArgs(argc, argv);
    Step_Status status = Step_ExecuteOptions(&new_state->args);

    if (Step_StatusIsError(status)) {
        Step_StatusLog(status);
        Step_PrintHelp();
        Step_StateDelete(&new_state);
        return NULL;
    }

    if (Step_IsNull(new_state->args.filename)) {
        Step_StateDelete(&new_state);
        return NULL;
    }

    status = Step_OpenFile(&new_state->current_file, new_state->args.filename, "r");

    if (Step_StatusIsError(status)) {
        Step_StateDelete(&new_state);
        Step_StatusLog(status);
        return NULL;
    }

    return new_state;
}

void Step_StateDelete(Step_State **state_ref) {
    if (Step_IsNull(state_ref)) {
        return;
    }

    Step_State *state = *state_ref;
    Step_Fclose(state->current_file);
    Step_Free(&state);
    *state_ref = NULL;
}

/* Implementation of functions that are used to read step functions */
Step_Status Step_OpenFile(Step_File **file, char const *fname, char const *mode) {
    *file = Step_Fopen(fname, mode);
    if (*file == NULL) {
        return Step_StatusError(Step_GetGlobalError());
    }
    return Step_StatusOk();
}

/* Implementation of Step Main function */
int Step_Main(int argc, char **argv) {
    Step_State *_state = Step_StateInitFromArgs(argc, argv);

    if (Step_IsNull(_state)) {
        Step_StatusLog(Step_StatusError("Could not initialize step state"));
        return -1;
    }

    Step_StateDelete(&_state);
    return 0;
}

