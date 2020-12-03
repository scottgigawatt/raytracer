/*
 * object.c: This file contains the implementation specifications for
 *           a generic object.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#include "object.h"
#include "light.h"
#include "mem.h"

/* 
 * object_init: Allocates memory for, initializes, and returns a new object.
 *
 * Parameters:  in      - The file containing the sphere specifications.
 *              objtype - Represents the type of object to initialize.
 *
 * Return:      A pointer to the newly initialized object.
 */
obj_t *object_init(FILE *in, int objtype) {
    obj_t      *obj  = (obj_t *)Malloc(sizeof(obj_t)); /* The new object    */
    static int objid = 0;                              /* Unique identifier */

    /* Set the object type, id, and light fields */
    obj->objtype  = objtype;
    obj->objid    = objid++;
    obj->getamb   = default_getamb;
    obj->getdiff  = default_getdiff;
    obj->getspec  = default_getspec;
    obj->getemiss = default_getemiss;

    /* If the object is not a light, initialize the reflectivity materials */
    if (objtype != LIGHT) {
        material_init(in, &obj->material);
    }

    return obj;
}
