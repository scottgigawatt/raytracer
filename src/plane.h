/*
 * plane.h: This header file contains the implementation specifications for
 *          a plane.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#ifndef PLANE_H
#define PLANE_H

#include "object.h"

/* Infinite plane */
typedef struct plane_type {
    double normal[VEC_SIZE]; /* A normal vector to the plane       */
    double point[VEC_SIZE];  /* A point on the plane               */
    void   *priv;            /* Private data for specialized types */
} plane_t;

/* Allocates memory for, initializes, and returns a new plane */
obj_t *plane_init(FILE *in, int objtype);

/* Dumps the contents of the plane object to the specified file */
int plane_dump(FILE *out, obj_t *obj);

/* Determines if a ray hits a plane object */
double hits_plane(double *base, double *dir, obj_t *obj);

/* Destroys the specified plane object */
void plane_destroy(obj_t *obj);

#endif
