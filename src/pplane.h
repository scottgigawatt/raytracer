/*
 * pplane.h: This header file contains the implementation specifications for
 *           a procedurally shaded plane.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#ifndef PPLANE_H
#define PPLANE_H

#include "plane.h"

/* Allocates memory for, initializes, and returns a new procedural plane */
obj_t *pplane_init(FILE *in, int objtype);

/* Shader function for creating alternating bands of color */
void pplane0_amb(obj_t *obj, double *ivec);

/* Shader function for creating alternating colored circles */
void pplane1_amb(obj_t *obj, double *ivec);

/* Shader function for creating asymtotic color bands */
void pplane2_amb(obj_t *obj, double *ivec);

/* Function pointers to plane shading procedures */
static void (*plane_shaders[])(obj_t *obj, double *ivec) = {
    pplane0_amb, /* Alternating bands of color  */
    pplane1_amb, /* Alternating colored circles */
    pplane2_amb  /* Asymtotic color bands       */
};

/* The number of plane shading functions */
#define NUM_PSHADERS sizeof(plane_shaders) / sizeof(void *)

#endif
