/*
 * mem.h:   This file contain a wrapper function prototypes for the C
 *          library functions malloc and free.  Malloc simply checks for any
 *          errors when allocating memory and Free ensures there are no
 *          dangling pointers associated with the freed memory.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 16 March 2011
 */

#ifndef MEM_H
#define MEM_H

#include "veclib3d.h"

/* Allocates and returns memory of the specified size */
void *Malloc(size_t size);

/* Frees the specified memory and ensures no dangling pointers */
void Free(void *mem);

#endif
