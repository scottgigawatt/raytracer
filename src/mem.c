/*
 * mem.c:   This file contain a wrapper functions for the C library functions
 *          malloc and free.  Malloc simply checks for any errors when
 *          allocating memory and Free ensures there are no dangling pointers
 *          associated with the freed memory.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 16 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

/*
 * Malloc:     A wrapper for the C library malloc function, which simply
 *             checks for any errors when allocating memory.
 *
 * Parameters: size - The size of the memory to allocate.
 */
void *Malloc(size_t size) {
    void *ptr = NULL;

    /* Check for valid size */
    if ( (int)size <= 0 ) {
        msg_exit(stderr, "Malloc: error: size must be > zero");
    /* Allocate memory and check for errors */
    } else if ( (ptr = malloc(size)) == NULL ) {
        msg_exit(stderr, "Malloc: error: memory allocation failed");
    }

    return ptr;
}

/*
 * Free:       A wrapper for the C library free function, which simply
 *             ensures there are no dangling pointers after freeing memory.
 *
 * Parameters: mem - A pointer to the memory to be freed.
 */
void Free(void *mem) {
    /* Free the specified memory */
    free(mem);

    /* Point the newly freed memory to NULL */
    mem = NULL;
}
