#ifndef FALSETYPE_CMAP_H
#define FALSETYPE_CMAP_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "../arena.h"

typedef struct FST_CmapIndex {
    uint16_t version;
    uint16_t num_subtables;
} FST_CmapIndex;

typedef struct FST_CmapSubtableEntry {
    uint16_t platform_id;
    uint16_t platform_specific_id;
    uint32_t offset;
} FST_CmapSubtableEntry;

typedef struct FST_CmapDataFormat4 {
    uint16_t length;
    uint16_t language;
    uint16_t seg_countx2;
    uint16_t search_range;
    uint16_t entry_selector;
    uint16_t range_shift;
    uint16_t* end_code;
    uint16_t reserved_pad;
    uint16_t* start_code;
    uint16_t* id_delta;
    uint16_t* id_range_offset;
    uint16_t* glyph_index_arr;
} FST_CmapDataFormat4;

typedef union FST_CmapDataFormat {
    FST_CmapDataFormat4 format4;
} FST_CmapDataFormat;

typedef struct FST_CmapData {
    uint16_t format;
    FST_CmapDataFormat data;
} FST_CmapData;

typedef struct FST_TableCmap {
    FST_CmapIndex index;
    FST_CmapSubtableEntry* subtable_entries;
    FST_CmapData* data;
} FST_TableCmap;

bool FST_ParseTableCmap(Arena* arena, FST_TableDirectoryEntry* entry, FILE* file, FST_TableCmap* font_cmap);
bool FST_CmapSubtableIsUnicodeBmp(FST_CmapSubtableEntry subtable_entry);

#endif // !FALSETYPE_CMAP_H
