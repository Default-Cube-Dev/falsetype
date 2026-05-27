#ifndef FALSETYPE_HEAD_H
#define  FALSETYPE_HEAD_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct FST_TableHead {
    Fixed version;
    Fixed font_revision;
    uint32_t checksum_ajustment;
    uint32_t magic_number; // it's not me, blame on apple :)
    uint16_t flags;
    uint16_t units_per_em;
    int64_t created; // why would a font file store this info?
    int64_t modified; // why?
    int16_t x_min;
    int16_t y_min;
    int16_t x_max;
    int16_t y_max;
    uint16_t mac_style;
    uint16_t lowest_rec_ppem;
    int16_t font_direction_hint;
    int16_t index_to_loc_format; // why allocate 16 bits when you can get away with just 1?
    int16_t glyph_data_format; // why store a zero?
} FST_TableHead;
// geez man, this makes me wanna make my own file format for fonts

bool FST_ParseTableHead(FST_TableDirectoryEntry* entry, FILE* file, FST_TableHead* font_head);
#endif // !FALSETYPE_HEAD_H
