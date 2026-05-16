odin build . -build-mode:shared -no-entry-point -out:libfalsetype.so
clang -Wall -Wextra -ggdb -L . -rpath . -lfalsetype ./test/main.c -o ./test/main

