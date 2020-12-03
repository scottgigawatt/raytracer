/*
 * sphere.h: This header file contains the implementation specifications for
 *           a sphere.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

/* Sphere */
typedef struct sphere_type {
    double center[VEC_SIZE]; /* The center of the sphere */
    double radius;           /* The radius of the sphere */
} sphere_t;

/* Allocates memory for, initializes, and returns a new sphere */
obj_t *sphere_init(FILE *in, int objtype);

/* Dumps the contents of the sphere object to the specified file */
int sphere_dump(FILE *out, obj_t *obj);

/* Determines if a ray hits a sphere object */
double hits_sphere(double *base, double *dir, obj_t *obj);

/* Destroys the specified sphere object */
void sphere_destroy(obj_t *obj);

#endif
