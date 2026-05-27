#define HASHMAP_IMPLEMENTATION
#include "falsetype.h"
#include "tables/cmap.h"
#include "tables/head.h"
#include "./hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

    
void FST_GetTagStr(uint32_t tag, char* str) {
    str[0] = (char)((tag      ) & 0xFF);
    str[1] = (char)((tag >>  8) & 0xFF);
    str[2] = (char)((tag >> 16) & 0xFF);
    str[3] = (char)((tag >> 24) & 0xFF);
    str[4] = '\0';
}

uint16_t FST_GetGlyphIndex(char glyph, FST_Font* font, uint16_t cmap_subtable) { // cmap_offset is just the offset from the begining of the cmap table
    switch (font->tables.cmap.data[cmap_subtable].format) {
        case 4:
            for (int i = 0; i<font->tables.cmap.data[cmap_subtable].data.format4.seg_countx2/2; i++) {
                if (font->tables.cmap.data[cmap_subtable].data.format4.end_code[i] >= glyph && font->tables.cmap.data[cmap_subtable].data.format4.start_code[i] <= glyph) {
                    printf("%c is in segment %d\n", glyph, i);

                    FST_TableDirectoryEntry* cmap_entry = (FST_TableDirectoryEntry*)hashmap_get(&font->table_entries, "cmap");
                    uint16_t glyph_offset = cmap_entry->offset + font->tables.cmap.data[cmap_subtable].data.format4.id_range_offset[i] + 2*(glyph - font->tables.cmap.data[cmap_subtable].data.format4.start_code[i]);
                    uint16_t glyph_index = 0;
                    fseek(font->raw_file, glyph_offset, SEEK_SET);
                    FST_Fread(font->raw_file, &glyph_index, sizeof(uint16_t), NULL);
                    byteswap(glyph_index, 16);
                    return glyph_index;
                }
            }
            break;

        default:
            printf("[FALSETYPE]: this font has an unsupported cmap format: %d\n", font->tables.cmap.data[cmap_subtable].format);
            exit(1);
    }
    return 0;
}
void FST_Fread(FILE* file, void* dest, int size, uint32_t* index) {
    fread(dest, size, 1, file);
    if (index != NULL) {
        *index += size;
        fseek(file, *index, SEEK_SET);
    }
}

uint32_t FST_TableEntriesHashfunc(void* key) {
    uint32_t hash = 0;
    char* keystr = (char*)key;
    for (int i = 0; i<4; i++) {
        hash += (keystr[i] >> i) + (hash << 2);
    }

    return hash;
}

FST_Font FST_LoadFont(char *filepath, Arena* arena) {
    printf("[FALSETYPE] loading font: %s\n", filepath);

    FILE* file = fopen(filepath, "rb");

    if (file == NULL) {
        perror("[FALSETYPE]: Error opening file");
        exit(1);
    }

    FST_Font font = {0};
    // need this because SEEK_CUR is weird
    uint32_t index = 0;

    font.raw_file = file;
    // OFFSET TABLE
    FST_Fread(file, &font.offset_table, sizeof(FST_OffsetTable), &index);
    printf("%u", index);
    // endian fix
    byteswap(font.offset_table.scaler_type, 32);
    byteswap(font.offset_table.num_tables, 16);
    byteswap(font.offset_table.search_range, 16);
    byteswap(font.offset_table.entry_selector, 16);
    byteswap(font.offset_table.range_shift, 16);

    // TABLE DIRECTORY ENTRY
    hashmap_init(&font.table_entries, font.offset_table.num_tables, sizeof(FST_TableDirectoryEntry), sizeof(char)*4, FST_TableEntriesHashfunc);

        char* tagstr = arena_alloc(arena, sizeof(char)*5); // 5th for the null terminator
    for (int i = 0; i < font.offset_table.num_tables; i++) {
        FST_TableDirectoryEntry tmp = {0};
        FST_Fread(file, &tmp, sizeof(FST_TableDirectoryEntry), &index);
        // endian fix
        //byteswap(tmp.tag, 32);
        byteswap(tmp.checksum, 32);
        byteswap(tmp.offset, 32);
        byteswap(tmp.length, 32);
        FST_GetTagStr(tmp.tag, tagstr);
        printf("getting %s into the hashmap\n", tagstr);
        printf("int: 0x%x\n", tmp.tag);
        FST_TableDirectoryEntry* real = arena_alloc(arena, sizeof(FST_TableDirectoryEntry)); // arena_alloc to make it presistant cuz the tmp is gonna get dealocated at the end
        memcpy(real, &tmp, sizeof(FST_TableDirectoryEntry));
        hashmap_set(&font.table_entries, tagstr, real);
    }

    // TABLES

    // CMAP
    if (!FST_ParseTableCmap(arena, hashmap_get(&font.table_entries, "cmap"), file, &font.tables.cmap)) {
        printf("[FALSETYPE]: Failed to parse cmap in font: %s\n", filepath);
        exit(1);
    }

    // HEAD
    if (!FST_ParseTableHead(hashmap_get(&font.table_entries, "head"), file, &font.tables.head)) {
        printf("[FALSETYPE]: Failed to parse head in font: %s\n", filepath);
        exit(1);
    }

    // MAXP
    if (!FST_ParseTableMaxp(hashmap_get(&font.table_entries, "maxp"), file, &font.tables.maxp)) {
        printf("[FALSETYPE]: Failed to parse maxp in font: %s\n", filepath);
        exit(1);
    }

    // LOCA
    if (!FST_ParseTableLoca(hashmap_get(&font.table_entries, "loca"), file, &font.tables.loca, font.tables.head.index_to_loc_format)) {
        printf("[FALSETYPE]: Failed to parse loca in font: %s\n", filepath);
        exit(1);
    }



    return font;
}


