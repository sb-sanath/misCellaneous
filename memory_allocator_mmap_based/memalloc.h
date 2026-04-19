#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>


#define WORD_BITS 32
#define MAX_ALLOC_SIZE_BYTES 1024*1024*1024
#define MAX_ALLOC_SIZE_WORDS (MAX_ALLOC_SIZE_BYTES/4) // 4 bytes (32 bits) = 1 word 

enum errorcodes {
    SUCCESS = 0,
    ERROR
};

typedef struct mem_ledger {
    uint32_t max_size;
    uint32_t free_mem_size;
    void *block_start;

}mem_ledger_t;

typedef struct header {
    bool allocated;
    uint32_t size;
    void *mem_start;
    void *mem_end;

}header_t;


int memory_init();
void *alloc (uint32_t size_in_bytes);
