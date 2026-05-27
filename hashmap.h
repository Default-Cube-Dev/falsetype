#ifndef HASHMAP_H
#define HASHMAP_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint32_t(*Hashfunc)(void* key);

typedef struct HashmapValue {
    bool is_occupied;
    void* data;
    void* key;
} HashmapValue;

typedef struct Hashmap {
    uint32_t element_count;
    HashmapValue* values;
    Hashfunc hashfunc;
    uint32_t key_size;
} Hashmap;

// initializes hashmp, if returns false, failed 
bool hashmap_init(Hashmap* hashmap, uint32_t element_count, uint32_t value_size, uint32_t key_size, Hashfunc hashfunc);
// sets a key to a value, if returns false. failed
bool hashmap_set(Hashmap* hashmap, void* key, void* value);
// you don't need docs for that :)
void* hashmap_get(Hashmap* hashmap, void* key);

#define HASHMAP_IMPLEMENTATION
#ifdef HASHMAP_IMPLEMENTATION
#ifndef HASHMAP_IMPL_H
#define HASHMAP_IMPL_H

bool hashmap_init(Hashmap* hashmap, uint32_t element_count, uint32_t value_size, uint32_t key_size, Hashfunc hashfunc) {
    if(hashmap == NULL) {
        printf("cannot initialize hashmap that is NULL");
        exit(1);
    }
    if (element_count == 0) {
        printf("cannot initialize hashmap with 0 elements");
        exit(1);
    }
    if (value_size == 0) {
        printf("cannot initialize hashmap with 0 value size");
        exit(1);
    }

    if (value_size == 0) {
        printf("cannot initialize hashmap with 0 key size");
        exit(1);
    }

    if (hashfunc == NULL) {
        printf("cannot initialize hashmap with no hashing function\n");
        exit(1);
    }

    hashmap->element_count = element_count;
    hashmap->key_size = key_size;
    hashmap->hashfunc = hashfunc;
    printf("valsize: %d\n", value_size*element_count);
    hashmap->values = (HashmapValue*)malloc(sizeof(HashmapValue)*element_count);
    if (hashmap->values == NULL) return false;
    for (uint32_t i = 0; i<element_count; i++) {
        hashmap->values[i].data = malloc(value_size);
        hashmap->values[i].is_occupied = 0;
        hashmap->values[i].key = malloc(key_size);
       hashmap->values[i].key  = NULL;
    }
    return true;
}

bool hashmap_set(Hashmap* hashmap, void* key, void* value) {
    if(key == NULL) {
        printf("cannot set key NULL to a value\n");
        return false;
    }

    uint32_t bucket_number = hashmap->hashfunc(key)%hashmap->element_count;
    if (bucket_number > (uint32_t)(hashmap->element_count-1)) {
        printf("[HASHMAP ERROR]: you just tried to set the keys array outside of the bounds, bad boi :), fix ur hashing function or pay me more RAM!\n");
        return false;
    }
    printf("bucket_number: %u\n", bucket_number);

    if (!hashmap->values[bucket_number].is_occupied) {
        if (hashmap->values[bucket_number].key != NULL) {
            if (memcmp(key, hashmap->values[bucket_number].key, hashmap->key_size) != 0) {

                printf("[HASHMAP ERROR] collided at 0x%x, linked lists are not implemented yet so get a better hashing function\n", bucket_number);
                return false;
            }
            hashmap->values[bucket_number].is_occupied = true;
            hashmap->values[bucket_number].key = key;
            hashmap->values[bucket_number].data = value;
            goto end;
        }

    hashmap->values[bucket_number].is_occupied = true;
    hashmap->values[bucket_number].key = key;
    hashmap->values[bucket_number].data = value;
    }     
    end:
        return true;
}

void* hashmap_get(Hashmap* hashmap, void* key) {
    if(key == NULL) {
        printf("cannot get key NULL\n");
        exit(1);
    }

    uint32_t bucket_number = hashmap->hashfunc(key)%hashmap->element_count;
    if (bucket_number > hashmap->element_count-1) {
        printf("[HASHMAP ERROR]: you just tried to get the keys array outside of the bounds, bad boi :), fix ur hashing function or pay me more RAM!\n");
        exit(1);
    }

    return hashmap->values[bucket_number].data;
}

#endif // !HASHMAP_IMPL_H
#endif // !HASHMAP_IMPLEMENTATION
#endif // HASHMAP_H
