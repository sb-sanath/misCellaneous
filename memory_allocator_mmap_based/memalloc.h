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


typedef struct header {
    bool allocated;
    uint32_t size;
    void *mem_start;
    void *mem_end;
    struct header *next;
    struct header *previous;

}header_t;

typedef struct mem_ledger {
    uint32_t max_size;
    uint32_t largest_free_mem_size;
    uint32_t smallest_free_mem_size;
    void *largest_free_start;
    void *smalllest_free_start;
    header_t *last_allocated;
    void *next_block_start;
}mem_ledger_t;


int memory_init();
void *alloc (uint32_t size_in_bytes);
void * __alloc(header_t *header, uint32_t size_in_bytes, void *free_mem_start)
