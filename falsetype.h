#ifndef FST_FONT_H
#define FST_FONT_H

#include <stdint.h>

typedef struct FST_Offset_table {
    uint32_t scaler_type;
    uint16_t num_tables;
    uint16_t search_range; 
    uint16_t entry_selector;
    uint16_t range_shift;
} FST_Offset_table;

typedef struct FST_TableDirectoryEntry {
    char* tag;
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;
} FST_TableDirectoryEntry;

typedef struct FST_Font {
    FST_Offset_table offset_table;
    FST_TableDirectoryEntry tables[];
} FST_Font;

FST_Font FST_LoadFont(char* filepath);

#endif // !FST_FONT_H

