#include "../falsetype.h"
#include <stdio.h>
#define ARENA_IMPLEMENTATION
#include "../arena.h"

int main() {
    Arena arena = {0};
    FST_Font font = FST_LoadFont("./ttf/Hack-Regular.ttf", &arena);

    printf("---MAXP---\n\n");
    printf("version: %d.%d\n", font.tables.maxp.version.intager, font.tables.maxp.version.frac);
    printf("num_glyphs: %d\n", font.tables.maxp.num_glyphs);
    printf("max_points: %d\n", font.tables.maxp.max_points);
    printf("max_contours: %d\n", font.tables.maxp.max_contours);
    printf("max_component_points: %d\n", font.tables.maxp.max_component_points);
    printf("max_component_contours: %d\n", font.tables.maxp.max_component_contours);
    printf("max_zones: %d\n", font.tables.maxp.max_zones);
    printf("max_twilight_points: %d\n", font.tables.maxp.max_twilight_points);
    printf("max_storage: %d\n", font.tables.maxp.max_storage);
    printf("max_function_defs: %d\n", font.tables.maxp.max_function_defs);
    printf("max_instruction_defs: %d\n", font.tables.maxp.max_instruction_defs);
    printf("max_stack_elements: %d\n", font.tables.maxp.max_stack_elements);
    printf("max_size_of_instructions: %d\n", font.tables.maxp.max_size_of_instructions);
    printf("max_component_elements: %d\n", font.tables.maxp.max_component_elements);
    printf("max_component_depth: %d\n", font.tables.maxp.max_component_depth);

    printf("----LOCA---\n\n");
    for (int i = 0; i<font.tables.maxp.num_glyphs; i++) {
        if (font.tables.head.index_to_loc_format) {
            printf("%u\n", font.tables.loca.loca_long[i]);
        } else {
            printf("%u\n", font.tables.loca.loca_short[i]);
        }
    
    }

    arena_free(&arena);
    return 0;
}
