/*
 * object.h: This header file contains the implementation specifications for
 *           a generic object.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#ifndef OBJECT_H
#define OBJECT_H

#define MISS -1

#include "material.h"
#include "model.h"
#include "veclib3d.h"
#include <stdio.h>

/* Structure for elements that are common to all objects */
typedef struct obj_type {
    struct obj_type *next;       /* Next object in list         */
    int    objid;                /* Numeric serial # for debug  */
    int    objtype;              /* Type code (14 -> Plane)     */
    void   *priv;                /* Private type-dependent data */

    /* Hits function */
    double (*hits)(double *base, double *dir, struct obj_type *);

    /* Plugins for retrieval of reflectivity (e.g. tiled floor) */
    void (*getamb) (struct obj_type *, double *);
    void (*getdiff)(struct obj_type *, double *);
    void (*getspec)(struct obj_type *, double *);

    /* Reflectivity for reflective objects */
    material_t material;

    /* These fields used only in illuminating objects (lights)  */
    void   (*getemiss)(struct obj_type *, double *);
    double emissivity[VEC_SIZE]; /* For lights          */
    double hitloc[VEC_SIZE];     /* Last hit point      */
    double normal[VEC_SIZE];     /* Normal at hit point */

    /* For memory management */
    void (*destroy)(struct obj_type *);

    /* For dumping object information */
    int (*dump)(FILE *, struct obj_type *);
} obj_t;

/* Allocates memory for, initializes, and returns a new object */
obj_t *object_init(FILE *in, int objtype);

#endif
