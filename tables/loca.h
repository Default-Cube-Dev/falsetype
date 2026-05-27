#ifndef FALSETYPE_LOCA_H
#define FALSETYPE_LOCA_H

#include <stdint.h>
#include <stdio.h>

typedef union FST_TableLoca {
    uint16_t* loca_short;
    uint32_t* loca_long;

} FST_TableLoca;

bool FST_ParseTableLoca(FST_TableDirectoryEntry* entry, FILE* file, FST_TableLoca* font_loca, uint16_t is_long);
#endif // !FALSETYPE_LOCA_H
