package falsetype

import "core:c"

FST_OffsetTable :: struct #packed {
    scaler_type: c.uint32_t,
    num_tables: c.uint16_t,
    search_range: c.uint16_t,
    entry_selector: c.uint16_t,
    range_shift: c.uint16_t,
}

FST_TableDirectoryEntry :: struct #packed {
    tag: cstring,
    checksum: c.uint32_t,
    offset: c.uint32_t,
    length: c.uint32_t,
}

FST_Font :: struct #packed {
    offset_table: FST_OffsetTable,
    tables: []FST_TableDirectoryEntry,
}

