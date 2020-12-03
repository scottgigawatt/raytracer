/*
 * list.h:  This header file contains implementation specifications for the 
 *          linked list data type. 
 * 
 * Author:  Scott Gigawatt
 *
 * Version: 16 March 2011
 */

#ifndef LIST_H
#define LIST_H

/* Structure for a link within the list */
typedef struct link_type {
    struct link_type *next;    /* Next link in the list   */
    void             *item;    /* Item owned by this link */
}  link_t;

/* Structure for the list */
typedef struct list_type {
    link_t  *head;             /* First link in the list */
    link_t  *tail;             /* Last link in the list  */
}  list_t;

/* Allocate a new list header on the heap and initialize it */
list_t *list_init(void);

/* Add an element to the end of a list */
void list_add(list_t *list, void *entity);

/* Delete all of the list control structures and the entities they control */
void list_del(list_t *list);

#endif
