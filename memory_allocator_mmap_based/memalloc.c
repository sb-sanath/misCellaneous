#include "memalloc.h"

/* A global variable for memory control block */
mem_ledger_t mem_ledger;

int memory_init()
{
    mem_ledger.max_size = MAX_ALLOC_SIZE_BYTES;
    //size_t size = mem_ledger.size;

    void *ptr = mmap(NULL, mem_ledger.max_size,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);

    if (ptr == MAP_FAILED) {
        printf("mmap failed");
        return ERROR;
    }

    mem_ledger.block_start = ptr;

    /* Initially, all of the memory is available*/
    mem_ledger.free_mem_size = MAX_ALLOC_SIZE_BYTES;

    return SUCCESS;
}

int main (void)
{

    int ret;
    int *a;
    ret = memory_init();

    a = (int *) mem_ledger.block_start;

    *a = 1;

    printf("&a = %p, a = %d", a, *a);
    //(int *) mem_ledger.block_start = 1;

    
/*
    // Use memory
    memset(ptr, 0xAA, size);

    printf("Allocated at %p\n", ptr);

    munmap(ptr, size);
    return 0;
*/
}