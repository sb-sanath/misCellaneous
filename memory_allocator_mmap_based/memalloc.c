#include "memalloc.h"

/* A global variable for memory control block */
mem_ledger_t mem_ledger;
uint32_t hole_size;

void *book_keeping(header_t *header)
{
    mem_ledger.free_mem_size = mem_ledger.free_mem_size - header->size;
    //mem_ledger.last_allocated = header;
}

void *sort_and_add_hole_list(hole_head_t *hole){

    hole_head_t *head = mem_ledger.hole_list;
    hole_head_t *temp;
    hole_head_t *previous = mem_ledger.hole_list;

    if (head == NULL) {
        mem_ledger.hole_list = hole;
        mem_ledger.hole_list->next = NULL;
        return 0;
    }

    /* Check if the hole->size is smaller than first entry in hole_list itself */
    if (hole->size < mem_ledger.hole_list->size){
        temp = mem_ledger.hole_list;
        mem_ledger.hole_list = hole;
        hole->next = temp;
    }
    else {
        while (head != NULL) {
            if (hole->size < mem_ledger.hole_list->size) {
                previous->next = hole;
                hole->next = head;
            }
            previous = head;
            head = head->next;
        }
        previous->next = hole;
        hole->next = head;
    }
}

void * unalloc(void *to_be_freed) {

    header_t *header;
    hole_head_t *hole;
    uint32_t size;

    header = (header_t*)(to_be_freed - sizeof(header_t));

    size = header->size;

    mem_ledger.free_mem_size = mem_ledger.free_mem_size + size;

    hole = (hole_head_t *) header;
    hole->size = size;
    hole->next = NULL;

    sort_and_add_hole_list(hole);

}

void * __alloc(header_t *header, uint32_t size_in_bytes) {

    header->allocated = true;
    header->size = size_in_bytes;
    header->mem_start = (void *) (header + 1);
    header->mem_end = header->mem_start + size_in_bytes;
}


void *alloc (uint32_t size_in_bytes)
{
    header_t *header;
    hole_head_t *temp_hole;
    hole_head_t *previous;

    if (size_in_bytes > mem_ledger.max_size) {
        printf("requested mem siz is larger than heap. ALLOCATION FAILED !!\n");
        return ENOMEM;
    }

    temp_hole = mem_ledger.hole_list;

    while (temp_hole != NULL) {
        if (temp_hole->size > size_in_bytes) {
            printf("Hole found! allocating in hole\n");
            previous->next = temp_hole->next;
            header = (header_t *) temp_hole;
            __alloc(header, size_in_bytes);
            book_keeping(header);
            return header->mem_start;
        }
        previous = temp_hole;
        temp_hole = temp_hole->next;
    }

    printf("NO holes found which could fit the requested size. Allocaing from main mem.\n");
    header = (header_t *) mem_ledger.next_block_start;
    __alloc(header, size_in_bytes);

    book_keeping(header);
    mem_ledger.next_block_start = header->mem_end + 1;

    return header->mem_start;

}

void *re_alloc(void *to_be_relocated, uint32_t new_size_in_bytes) {

    void *new_mem_start;
    uint32_t data_bytes_to_be_copied;
    header_t *original_mem_header = (header_t*)(to_be_relocated - sizeof(header_t));

    new_mem_start =  alloc(new_size_in_bytes);

    printf("alloc successfull from realloc\n");

    data_bytes_to_be_copied = (new_size_in_bytes < original_mem_header->size) ? new_size_in_bytes : original_mem_header->size;

    memcpy(new_mem_start, (const void*)original_mem_header->mem_start, data_bytes_to_be_copied);

    printf("memcpy successfull from realloc\n");

    unalloc(to_be_relocated);

    printf("unalloc successfull from realloc\n");

    return new_mem_start;
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
    mem_ledger.free_mem_size = MAX_ALLOC_SIZE_BYTES;

    mem_ledger.hole_list = NULL;

    return SUCCESS;
}

int main (void)
{

    int ret;
    int *a, *b, *c, *d, *e;

    ret = memory_init();

    a = (int *) alloc(10);
    *a = 9;
    printf("%d\n", *a);

    unalloc(a);

    a = (int *) alloc(9);

    unalloc(a);

    a = (int *) alloc(100);
    b = (int *) alloc(90);
    c = (int *) alloc(80);

    unalloc(c);

    c = (int *) alloc(10);

    d = (int *) alloc(18);

    e = (int *) re_alloc(d, 2);




}