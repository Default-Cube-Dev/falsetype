#include "../falsetype.h"
#include <stdbool.h>
#include <stdio.h>
#include "./maxp.h"

bool FST_ParseTableMaxp(FST_TableDirectoryEntry* entry, FILE* file, FST_TableMaxp* font_maxp) {
    uint32_t index = entry->offset;
    fseek(file, index, SEEK_SET);
    FST_Fread(file, font_maxp, sizeof(FST_TableMaxp), &index);

    byteswap(font_maxp->version.intager, 16);
    byteswap(font_maxp->version.frac, 16);
    byteswap(font_maxp->num_glyphs, 16);
    byteswap(font_maxp->max_points, 16);
    byteswap(font_maxp->max_contours, 16);
    byteswap(font_maxp->max_component_points, 16);
    byteswap(font_maxp->max_component_contours, 16);
    byteswap(font_maxp->max_zones, 16);
    byteswap(font_maxp->max_twilight_points, 16);
    byteswap(font_maxp->max_storage, 16);
    byteswap(font_maxp->max_function_defs, 16);
    byteswap(font_maxp->max_instruction_defs, 16);
    byteswap(font_maxp->max_stack_elements, 16);
    byteswap(font_maxp->max_size_of_instructions, 16);
    byteswap(font_maxp->max_component_elements, 16);
    byteswap(font_maxp->max_component_depth, 16);

    return true;
}
