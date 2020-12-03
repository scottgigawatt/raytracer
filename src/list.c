/*
 * list.c:  This file contains function implementations for the linked list 
 *          data type. 
 *
 * Author:  Scott Gigawatt
 *
 * Version: 16 March 2011
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "mem.h"

/* 
 * list_init: Allocate a new list header on the heap and initialize it 
 *
 * Return:    A pointer to the newly initialized list.
 */
list_t *list_init(void) {
    list_t *list = (list_t *)Malloc(sizeof(list_t)); /* The new list */

    /* Make sure there are no dangling pointers in the list */
    list->head = NULL;
    list->tail = list->head;

    return list;
}

/* 
 * list_add:   Adds the given entity to the end of the specified list.
 *
 * Parameters: list   - A pointer to the list.
 *             entity - A pointer to the entity to be added to the list.
 */
void list_add(list_t *list, void *entity) {
    link_t *link = (link_t *)Malloc(sizeof(link_t)); /* The new link */

    /* Initialize the new link */
    link->next = NULL;
    link->item = entity;

    /* Special case for an empty list */
    if (list->head == NULL) {
        list->head = link;
        list->tail = list->head;
    /* Otherwise append to the end of the list */
    } else {
        list->tail->next = link;
        list->tail       = link;
    }
}

/* 
 * list_del:   Deletes all of the list control structures and the 
 *             entities they control.
 *
 * Parameters: list - A pointer to the list to delete.
 */
void list_del(list_t *list) {
    link_t *current = list->head; /* The current link in the list */
    link_t *link    = NULL;       /* The link to delete */

    if (list != NULL) {
        /* Traverse list and free memory associated with each link */
        while (current) {
            link    = current;
            current = current->next;
            
            Free(link->item); /* Free item memory */
            Free(link);       /* Free link memory */
        }

        /* Free the memory associated with the list */
        Free(list);
    }
}
