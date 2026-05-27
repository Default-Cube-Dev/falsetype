#ifndef FALSETYPE_MAXP_H
#define FALSETYPE_MAXP_H

#include <stdio.h>
#include <stdint.h>

typedef struct FST_TableMaxp {
    Fixed version;
    uint16_t num_glyphs;
    uint16_t max_points;
    uint16_t max_contours;
    uint16_t max_component_points;
    uint16_t max_component_contours;
    uint16_t max_zones;
    uint16_t max_twilight_points;
    uint16_t max_storage;
    uint16_t max_function_defs;
    uint16_t max_instruction_defs;
    uint16_t max_stack_elements;
    uint16_t max_size_of_instructions;
    uint16_t max_component_elements;
    uint16_t max_component_depth;
} FST_TableMaxp;

bool FST_ParseTableMaxp(FST_TableDirectoryEntry* entry, FILE* file, FST_TableMaxp* font_maxp);
#endif // FALSETYPE_MAXP_H
