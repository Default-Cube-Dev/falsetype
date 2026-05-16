package falsetype

import "core:strings"
import "core:encoding/endian"
import "core:os"
import "base:runtime"
import "core:fmt"


u32_to_bytes :: proc(x: u32) -> []byte {
    b := make([]byte, 4);
    b[0] = byte(x & 0xFF);
    b[1] = byte((x >> 8) & 0xFF);
    b[2] = byte((x >> 16) & 0xFF);
    b[3] = byte((x >> 24) & 0xFF);
    return b;
}

@export
FST_LoadFont :: proc "c" (filepath: cstring) -> FST_Font {
    context = runtime.default_context()
    fmt.println("loading font: ", filepath)

    file, err := os.read_entire_file_from_path(string(filepath), context.allocator)
    if err != nil {
        delete(file)
        fmt.panicf("failed to load font: %s: %s", string(filepath), err)
    }
    defer delete(file)
    font: FST_Font
    ok: bool
    // need this to keep track of the location
    index: uint

    //----------OFFSET TABLE----------

    font.offset_table.scaler_type, ok = endian.get_u32(file[index:index+4], endian.Byte_Order.Big);
    if !ok {fmt.panicf("[FALSETYPE]: failed to get scaler_type from font: %s, the file may be currupt\n", string(filepath))}
    index += 4
    font.offset_table.num_tables, ok = endian.get_u16(file[index:index+2], endian.Byte_Order.Big);
    if !ok {fmt.panicf("[FALSETYPE]: failed to get num_tables from font's offset table: %s, the file may be currupt\n", string(filepath))}
    index += 2
    font.offset_table.search_range, ok = endian.get_u16(file[index:index+2], endian.Byte_Order.Big);
    if !ok {fmt.panicf("[FALSETYPE]: failed to get search_range from font's offset table: %s, the file may be currupt\n", string(filepath))}
    index += 2
    font.offset_table.entry_selector, ok = endian.get_u16(file[index:index+2], endian.Byte_Order.Big);
    if !ok {fmt.panicf("[FALSETYPE]: failed to get entry_selector from font's offset table: %s, the file may be currupt\n", string(filepath))}
    index += 2
    font.offset_table.range_shift, ok = endian.get_u16(file[index:index+2], endian.Byte_Order.Big);
    if !ok {fmt.panicf("[FALSETYPE]: failed to get range_shift from font's offset table: %s, the file may be currupt\n", string(filepath))}
    index += 2

    //----------TABLES----------
    
    font.tables = make([]FST_TableDirectoryEntry, font.offset_table.num_tables, context.allocator)
    for i in 0..=font.offset_table.num_tables-1 {
        // the tag doesn't work
        tag_u32, ok := endian.get_u32(file[index:index+4], endian.Byte_Order.Big)       
        fmt.println(tag_u32)
        if !ok {fmt.panicf("[FALSETYPE]: failed to get tag from font's table directory: %s, the file may be currupt\n", string(filepath))}
        tag_sb: strings.Builder = strings.builder_from_bytes(u32_to_bytes(tag_u32))
        font.tables[i].tag = strings.to_cstring(&tag_sb)
        index+=4

        font.tables[i].checksum, ok = endian.get_u32(file[index:index+4], endian.Byte_Order.Big)
        if !ok {fmt.panicf("[FALSETYPE]: failed to get checksum from font's table directory: %s, the file may be currupt\n", string(filepath))}
        index+=4
        font.tables[i].offset, ok = endian.get_u32(file[index:index+4], endian.Byte_Order.Big)
        if !ok {fmt.panicf("[FALSETYPE]: failed to get offset from font's table directory: %s, the file may be currupt\n", string(filepath))}
        index+=4
        font.tables[i].length, ok = endian.get_u32(file[index:index+4], endian.Byte_Order.Big)
        if !ok {fmt.panicf("[FALSETYPE]: failed to get length from font's table directory: %s, the file may be currupt\n", string(filepath))}
        index+=4
    }


    return font
}

