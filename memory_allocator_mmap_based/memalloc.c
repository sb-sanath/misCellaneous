#include "memalloc.h"

/* A global variable for memory control block */
mem_ledger_t mem_ledger;
uint32_t hole_size;

void *book_keeping(header_t *header)
{
    mem_ledger.smallest_free_mem_size = mem_ledger.max_size - header->size; //smallest/largest_free_mem_size TBD
    mem_ledger.next_block_start = header->mem_end + 1;

    header->previous = mem_ledger.last_allocated;
    mem_ledger.last_allocated = header;

    if (header->next != NULL) {
        hole_size = (uint32_t) ((void *)header - (header->previous->mem_end + 1));
    }
    else {
        //hole_size = 
    }

    if (hole_size < mem_ledger.smallest_free_mem_size) {
        mem_ledger.smallest_free_mem_size = hole_size;
        //mem_ledger.smalllest_free_start = 
    }

}

void * my_free(void *to_be_freed) {

    header_t *header;

    header = (header_t*)to_be_freed - sizeof(header_t);

}

void * __alloc(header_t *header, uint32_t size_in_bytes, void *free_mem_start) {

    header = (header_t *) free_mem_start;
    header->allocated = true;
    header->size = size_in_bytes;
    header->mem_start = (void *) (header + 1);
    header->mem_end = header->mem_start + size_in_bytes;
}


void *alloc (uint32_t size_in_bytes)
{
    header_t *header;

    if (size_in_bytes > mem_ledger.max_size) {
        printf("requested mem siz is larger than heap. ALLOCATION FAILED !!\n");
        return ENOMEM;
    }

    if (size_in_bytes < mem_ledger.smallest_free_mem_size) {
        __alloc(header, size_in_bytes, mem_ledger.smalllest_free_start);
    }

    else {
        __alloc(header, size_in_bytes, mem_ledger.largest_free_start);
    }
    

    book_keeping(header);

    return header->mem_start;

}

int memory_init()
{
    mem_ledger.max_size = MAX_ALLOC_SIZE_BYTES;

    void *ptr = mmap(NULL, mem_ledger.max_size,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);

    if (ptr == MAP_FAILED) {
        printf("mmap failed");
        return ERROR;
    }

    mem_ledger.next_block_start = ptr;

    /* Initially, all of the memory is available*/
    mem_ledger.smallest_free_mem_size = MAX_ALLOC_SIZE_BYTES;
    mem_ledger.largest_free_mem_size = MAX_ALLOC_SIZE_BYTES;

    return SUCCESS;
}

int main (void)
{

    int ret;
    int *a;

    ret = memory_init();

    a = (int *) alloc(10);
    *a = 9;

    printf("%d", *a);


}