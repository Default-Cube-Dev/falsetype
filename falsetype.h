#ifndef FALSETYPE_H
#define FALSETYPE_H

#include <stdint.h>
#include <stdio.h>
#include "./hashmap.h"

#define byteswap(val, size) val = __builtin_bswap ## size (val);

typedef struct Fixed {
    int16_t intager;
    int16_t frac;
} Fixed;

typedef struct FST_OffsetTable {
    uint32_t scaler_type;
    uint16_t num_tables;
    uint16_t search_range; 
    uint16_t entry_selector;
    uint16_t range_shift;
} FST_OffsetTable;

typedef struct FST_TableDirectoryEntry {
    uint32_t tag;
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;
} FST_TableDirectoryEntry;

#include "tables/cmap.h"
#include "tables/head.h"
#include "tables/maxp.h"
#include "tables/loca.h"

typedef struct FST_TableData {
    FST_TableCmap cmap;
    FST_TableHead head;
    FST_TableMaxp maxp;
    FST_TableLoca loca;
} FST_TableData;

typedef struct FST_Font {
    FST_OffsetTable offset_table;
    Hashmap table_entries;
    FST_TableData tables;
    FILE* raw_file;
} FST_Font;

FST_Font FST_LoadFont(char* filepath, Arena* arena);
void FST_Fread(FILE* file, void* dest, int size, uint32_t* index);
void FST_GetTagStr(uint32_t tag, char* str); 
uint16_t FST_GetGlyphIndex(char glyph, FST_Font* font, uint16_t cmap_subtable); // cmap_offset is just the offset from the begining of the cmap table

#endif // !FALSETYPE_H

