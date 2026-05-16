#include <string.h>
#define NOB_IMPLEMENTATION
#include "./nob.h"

int main(int argc, char *argv[]) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "odin", "build");
    nob_cmd_append(&cmd, ".");
    nob_cmd_append(&cmd, "-build-mode:shared", "-no-entry-point");
    nob_cmd_append(&cmd,  "-out:libfalsetype.so");

    if (!nob_cmd_run(&cmd)) return 1;

    if (argc > 0) {
        if (strcmp(argv[1], "-e") == 0) {
            nob_cmd_append(&cmd, "clang");
            nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb" );
            nob_cmd_append(&cmd, "-L", ".", "-rpath", ".");
            nob_cmd_append(&cmd, "-lfalsetype");
            nob_cmd_append(&cmd, "./test/main.c");
            nob_cmd_append(&cmd, "-o", "./test/main");

            if (!nob_cmd_run(&cmd)) return 1;
        }
    }


    return 0;
}
