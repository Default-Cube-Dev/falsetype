#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../falsetype.h"
#include "cmap.h"

bool FST_CmapSubtableIsUnicodeBmp(FST_CmapSubtableEntry subtable_entry) {
    return (subtable_entry.platform_id == 0 && subtable_entry.platform_specific_id == 3) /*Unicode+bmp*/ ||
        (subtable_entry.platform_id == 3 && subtable_entry.platform_specific_id == 1 /*Microslop + unicode ucs 2*/);
}


bool FST_DumpCmapData(Arena* arena, FST_CmapData* cmap_data, int cmap_end, FILE* file, uint32_t* index) {
    switch (cmap_data->format) {
        case 4:
            FST_Fread(file, &cmap_data->data.format4.length, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.length, 16);

            FST_Fread(file, &cmap_data->data.format4.language, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.language, 16);

            FST_Fread(file, &cmap_data->data.format4.seg_countx2, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.seg_countx2, 16);

            FST_Fread(file, &cmap_data->data.format4.search_range, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.search_range, 16);

            FST_Fread(file, &cmap_data->data.format4.entry_selector, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.entry_selector, 16);

            FST_Fread(file, &cmap_data->data.format4.range_shift, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.range_shift, 16);

            cmap_data->data.format4.end_code = arena_alloc(arena, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2));
            cmap_data->data.format4.start_code = arena_alloc(arena, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2));
            cmap_data->data.format4.id_delta = arena_alloc(arena, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2));
            cmap_data->data.format4.id_range_offset = arena_alloc(arena, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2));

            FST_Fread(file, cmap_data->data.format4.end_code, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2), index);
            for (int i = 0; i < cmap_data->data.format4.seg_countx2/2; i++) {
                byteswap(cmap_data->data.format4.end_code[i], 16)
            }

            FST_Fread(file, &cmap_data->data.format4.reserved_pad, sizeof(uint16_t), index);
            byteswap(cmap_data->data.format4.reserved_pad, 16);
            if (cmap_data->data.format4.reserved_pad != 0) {
                printf("[FALSETYPE]: WARNING! cmap reserved_pad non-zero: %d\n", cmap_data->data.format4.reserved_pad);
            }

            FST_Fread(file, cmap_data->data.format4.start_code, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2), index);
            for (int i = 0; i < cmap_data->data.format4.seg_countx2/2; i++) {
                byteswap(cmap_data->data.format4.start_code[i], 16)
            }

            FST_Fread(file, cmap_data->data.format4.id_delta, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2), index);
            for (int i = 0; i < cmap_data->data.format4.seg_countx2/2; i++) {
                byteswap(cmap_data->data.format4.id_delta[i], 16)
            }

            FST_Fread(file, cmap_data->data.format4.id_range_offset, sizeof(uint16_t) * (cmap_data->data.format4.seg_countx2/2), index);
            for (int i = 0; i < cmap_data->data.format4.seg_countx2/2; i++) {
                byteswap(cmap_data->data.format4.id_range_offset[i], 16)
            }

            cmap_data->data.format4.glyph_index_arr = arena_alloc(arena, cmap_data->data.format4.length - *index);
            FST_Fread(file, cmap_data->data.format4.glyph_index_arr, sizeof(*cmap_data->data.format4.glyph_index_arr), index);
            for (int i = 0; i < cmap_data->data.format4.seg_countx2/2; i++) {
                byteswap(cmap_data->data.format4.glyph_index_arr[i], 16)
            }


            break;

        default:
            printf("[FALSETYPE]: Error: this font has an unsuported cmap format: %d\n", cmap_data->format);
            return false;
            break;
    }
    return true;
}
bool FST_ParseTableCmap(Arena* arena, FST_TableDirectoryEntry* entry, FILE* file, FST_TableCmap* font_cmap) {
    uint32_t index = entry->offset;
    fseek(file, index, SEEK_SET);

    FST_Fread(file, &font_cmap->index, sizeof(FST_CmapIndex), &index);
    // endian fix
    byteswap(font_cmap->index.num_subtables, 16);
    byteswap(font_cmap->index.version, 16);

    font_cmap->subtable_entries = arena_alloc(arena, sizeof(FST_CmapSubtableEntry)* font_cmap->index.num_subtables);
    font_cmap->data = arena_alloc(arena, sizeof(FST_CmapData) * font_cmap->index.num_subtables);

    for (int i = 0; i < font_cmap->index.num_subtables; i++) {
        FST_Fread(file, &font_cmap->subtable_entries[i], sizeof(FST_CmapSubtableEntry), &index);
        // fix endian
        byteswap(font_cmap->subtable_entries[i].offset, 32);
        byteswap(font_cmap->subtable_entries[i].platform_id, 16);
        byteswap(font_cmap->subtable_entries[i].platform_specific_id, 16);

    }

    for (int i = 0; i < font_cmap->index.num_subtables; i++) {
        if (FST_CmapSubtableIsUnicodeBmp(font_cmap->subtable_entries[i])) {
            index = entry->offset + font_cmap->subtable_entries[i].offset;
            fseek(file, index, SEEK_SET);
            FST_Fread(file, &font_cmap->data[i].format, sizeof(uint16_t), &index);
            byteswap(font_cmap->data[i].format, 16);
            if(!FST_DumpCmapData(arena, &font_cmap->data[i], entry->offset+entry->length, file, &index)) {printf("[FALSETYPE] failed to dump cmap data\n"); return false;}
        } else {
            printf("[FALSETYPE]: WARNING! this font contains non-unicode cmap subtables that are not supported\n");
        }
    }


    return true;
}

