/*
 * fplane.h: This header file contains the implementation specifications for
 *           a finite plane.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#ifndef FPLANE_H
#define FPLANE_H

#include "plane.h"

/* Represents a finite plane */
typedef struct fplane_type {
    double xdir[VEC_SIZE];             /* Direction of x axis  */
    double size[VEC_SIZE - 1];         /* The width and height */
    double rotmat[VEC_SIZE][VEC_SIZE]; /* Rotation matrix      */
    double lasthit[VEC_SIZE - 1];      /* Used for textures    */
} fplane_t;

/* Allocates memory for, initializes, and returns a new finite plane */
obj_t *fplane_init(FILE *in, int objtype);

/* Dumps the contents of the finite plane object to the specified file */
int fplane_dump(FILE *out, obj_t *obj);

/* Determines if a ray hits a finite plane object */
double hits_fplane(double *base, double *dir, obj_t *obj);

#endif
