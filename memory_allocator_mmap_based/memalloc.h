#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define WORD_BITS 32
#define MAX_ALLOC_SIZE_BYTES 1024*1024*1024
#define MAX_ALLOC_SIZE_WORDS (MAX_ALLOC_SIZE_BYTES/4) // 4 bytes (32 bits) = 1 word 

enum errorcodes {
    SUCCESS = 0,
    ERROR
};

typedef struct hole_head {
    uint32_t size;
    struct hole_head *next;
}hole_head_t;

typedef struct header {
    bool allocated;
    uint32_t size;
    void *mem_start;
    void *mem_end;

}header_t;

typedef struct mem_ledger {
    uint32_t max_size;
    uint32_t free_mem_size;
    //header_t *last_allocated;
    void *next_block_start;
    struct  hole_head *hole_list;    
}mem_ledger_t;


int memory_init();
void *alloc (uint32_t size_in_bytes);
void * __alloc(header_t *header, uint32_t size_in_bytes);
void *sort_and_add_hole_list(hole_head_t *hole);
void * unalloc(void *to_be_freed);
void *re_alloc(void *to_be_relocated, uint32_t new_size_in_bytes);

/*
last_allocated for the main memory - i.e, the normal remaining memory

hole_list for holes. A linked list of all holes. These will be basically typecasted from header_t which is freed.

Befor allocating from the main memory, traverse the list of holes. If found a hole which can contain the new requested size,
allocate it. (Also, handle the hole list properly).
*/