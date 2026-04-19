#include "memalloc.h"

/* A global variable for memory control block */
mem_ledger_t mem_ledger;

void *book_keeping(header_t *header)
{
    mem_ledger.free_mem_size = mem_ledger.max_size - header->size;
    mem_ledger.block_start = header->mem_end + 1;
}

void *alloc (uint32_t size_in_bytes)
{
    header_t *header;

    if (size_in_bytes > mem_ledger.max_size) {
        printf("requested mem siz is larger than heap. ALLOCATION FAILED !!\n");
        return ENOMEM;
    }

    header = (header_t *) mem_ledger.block_start;

    header->allocated = true;
    header->size = size_in_bytes;
    header->mem_start = (void *) (header + 1);
    header->mem_end = header->mem_start + size_in_bytes;

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

    a = (int *) alloc(10);
    *a = 9;

    printf("%d", *a);


}