#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../falsetype.h"
#include "./loca.h"

bool FST_ParseTableLoca(FST_TableDirectoryEntry* entry, FILE* file, FST_TableLoca* font_loca, uint16_t is_long) {
    fseek(file, entry->offset, SEEK_SET);
    if (is_long == 1) {
        uint16_t loca_len = 1573 / sizeof(uint32_t);
        font_loca->loca_long = malloc(sizeof(uint32_t) * 1573);

        if (!font_loca->loca_long) {printf("[FALSETYPE] failed to allocate loca\n"); return false;}
        FST_Fread(file, font_loca->loca_long, loca_len*sizeof(uint32_t), NULL);
        for (int i = 0; i<loca_len; i++) {
            byteswap(font_loca->loca_long[i], 32);
        }
    } else {
        uint16_t loca_len = entry->length / sizeof(uint16_t);
        font_loca->loca_short = malloc(sizeof(uint16_t) * loca_len);

        if (!font_loca->loca_short) {printf("[FALSETYPE] failed to allocate loca\n"); return false;}
        FST_Fread(file, font_loca->loca_short, 1573*sizeof(uint16_t), NULL);
        for (int i = 0; i<loca_len; i++) {
            byteswap(font_loca->loca_short[i], 16);
        }
    }

    return true;
}
