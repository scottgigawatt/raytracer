/*
 * tplane.h: This header file contains the implementation specifications for
 *           a tiled plane.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#ifndef TPLANE_H
#define TPLANE_H

#include "plane.h"

/* Represents a tiled plane */
typedef struct tplane_type {
    double     xdir[VEC_SIZE];             /* Direction of x axis  */
    double     size[VEC_SIZE - 1];         /* The width and height */
    double     rotmat[VEC_SIZE][VEC_SIZE]; /* Rotation matrix      */
    material_t background;                 /* Background color     */
} tplane_t;

/* Allocates memory for, initializes, and returns a new tiled plane */
obj_t *tplane_init(FILE *in, int objtype);

/* Dumps the contents of the tiled plane object to the specified file */
int tplane_dump(FILE *out, obj_t *obj);

/* Retrieves the ambient lighting information for the specified tiled plane */
void tp_amb(obj_t *obj, double *ambient);

/* Retrieves the diffuse lighting information for the specified tiled plane */
void tp_diff(obj_t *obj, double *diffuse);

/* Retrieves the specular lighting information for the specified tiled plane */
void tp_spec(obj_t *obj, double *specular);

/* Selects whether the hit location was background or foreground material */
int tp_select(obj_t *obj);

#endif
