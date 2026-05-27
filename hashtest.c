#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t hashfunc(char* keystr) {
    uint32_t hash = 0;
    for (int i = 0; i<4; i++) {
        hash += ((keystr[i] >> hash) + (hash << i)) % 7;
    }

    return hash;
}

int comp(const void* a, const void* b) {
    return *(uint32_t*)a - *(uint32_t*)b;
}

int main(void) {
    
    char* tags[] = {
        "DSIG",
        "GSUB",
        "OS/2",
        "TTFA",
        "cmap",
        "cvt ",
        "fpgm",
        "gasp",
        "glyf",
        "head",
        "hhea",
        "hmtx",
        "loca",
        "maxp",
        "name",
        "post",
        "prep",
    };


//    char* tags[] = {
//        "acnt", 
//        "ankr", 
//        "avar", 
//        "bdat", 
//        "bhed", 
//        "bloc", 
//        "bsln", 
//        "cmap", 
//        "cvar", 
//        "cvt ", 
//        "EBSC", 
//        "fdsc", 
//        "feat", 
//        "fmtx", 
//        "fond", 
//        "fpgm", 
//        "fvar", 
//        "gasp", 
//        "gcid", 
//        "glyf", 
//        "gvar", 
//        "hdmx", 
//        "head", 
//        "hhea", 
//        "hmtx", 
//        "hvgl", 
//        "hvpm", 
//        "just", 
//        "kern", 
//        "kerx", 
//        "lcar", 
//        "loca", 
//        "ltag", 
//        "maxp", 
//        "meta", 
//        "mort", 
//        "morx", 
//        "name", 
//        "opbd", 
//        "OS/2", 
//        "post", 
//        "prep", 
//        "prop", 
//        "sbix", 
//        "trak", 
//        "vhea", 
//        "vmtx", 
//        "xref", 
//        "Zapf",
//        "DSIG",
//        "TTFA",
//        "GSUB",
//    };

    uint32_t tags_len = sizeof(tags)/sizeof(char*);
    printf("tags len %u\n", tags_len);
    uint32_t* hashes = malloc(tags_len*sizeof(uint32_t));

    for (int i = 0; i<tags_len-1; i++) {
        hashes[i] = hashfunc(tags[i]);
        uint32_t bucknum = hashes[i]%tags_len;
        if (bucknum > tags_len) {
            printf("buck is suboptimal!\n");
        }
        printf("%s: 0x%x (%d): buck: %d\n", tags[i], hashes[i], hashes[i], bucknum);
    }

    qsort(hashes, tags_len, sizeof(uint32_t), comp);

    for (int i = 0; i<tags_len-1; i++) {
        if (hashes[i] == hashes[i+1]) {
            printf("collided at: 0x%x\n", hashes[i]);
        }
    }


    return 0; 

}
