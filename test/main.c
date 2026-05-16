#include "../../falsetype.h"
#include <stdio.h>

int main() {
    FST_Font font = FST_LoadFont("/usr/share/fonts/Adwaita/AdwaitaMono-Bold.ttf");

    printf("-----offset_table-----\n\n");
    printf("scaler_type: %d\n", font.offset_table.scaler_type);
    printf("num_tables: %d\n", font.offset_table.num_tables);
    printf("search_range: %d\n", font.offset_table.search_range);
    printf("entry_selector: %d\n", font.offset_table.entry_selector);
    printf("range_shift: %d\n", font.offset_table.range_shift);
    
    for (int i = 0; i<font.offset_table.num_tables-1; i++) {
        printf("-----tabledir #%d-----\n\n", i);

//        printf("tag: %s\n", font.tables[i].tag);
        printf("checksum: %u\n", font.tables[i].checksum);
        printf("offset: %u\n", font.tables[i].offset);
        printf("length: %u\n", font.tables[i].length);
    }
    return 0;
}
