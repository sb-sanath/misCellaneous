#include "alloc.h"

extern heap *memspace;

int main (int argc, char *argv[]) {

    int8 *p;
    p = memspace;

    *p = 'X';
    
    return 0;
}