#include "../falsetype.h"
#include "./head.h"
#include <stdbool.h>
#include <stdio.h>

bool FST_ParseTableHead(FST_TableDirectoryEntry* entry, FILE* file, FST_TableHead* font_head) {
    uint32_t index = entry->offset;
    fseek(file, index, SEEK_SET);
    FST_Fread(file, font_head, sizeof(FST_TableHead), &index);

    byteswap(font_head->version.intager, 16);
    byteswap(font_head->version.frac, 16);
    byteswap(font_head->font_revision.intager, 16);
    byteswap(font_head->font_revision.frac, 16);
    byteswap(font_head->checksum_ajustment, 32);
    byteswap(font_head->magic_number, 32);
    if (font_head->magic_number != 0x5F0F3CF5) {printf("[FALSETYPE] WARNING! magic number in head ain't doin' the magic\n");}
    byteswap(font_head->flags, 16);
    byteswap(font_head->units_per_em, 16);
    byteswap(font_head->created, 64);
    byteswap(font_head->modified, 64);
    byteswap(font_head->x_min, 16);
    byteswap(font_head->y_min, 16);
    byteswap(font_head->x_max, 16);
    byteswap(font_head->y_max, 16);
    byteswap(font_head->mac_style, 16);
    byteswap(font_head->lowest_rec_ppem, 16);
    byteswap(font_head->font_direction_hint, 16);
    byteswap(font_head->index_to_loc_format, 16);
    byteswap(font_head->glyph_data_format, 16);


    return true;
}
