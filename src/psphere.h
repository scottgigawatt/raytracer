/*
 * psphere.h: This header file contains the implementation specifications for
 *            a procedurally shaded sphere.
 *
 * Author:    Scott Gigawatt
 *
 * Version:   22 March 2011
 */

#ifndef PSPHERE_H
#define PSPHERE_H

#include "sphere.h"

/* Allocates memory for, initializes, and returns a new procedural sphere */
obj_t *psphere_init(FILE *in, int objtype);

/* Shader function for creating alternating bands of color */
void psphere0_amb(obj_t *obj, double *ivec);

/* Shader function for creating alternating colored circles */
void psphere1_amb(obj_t *obj, double *ivec);

/* Shader function for creating asymtotic color bands */
void psphere2_amb(obj_t *obj, double *ivec);

/* Function pointers to sphere shading procedures */
static void (*sphere_shaders[])(obj_t *obj, double *ivec) = {
    psphere0_amb, /* Alternating bands of color  */
    psphere1_amb, /* Alternating colored circles */
    psphere2_amb  /* Asymtotic color bands       */
};

/* The number of sphere shading functions */
#define NUM_SSHADERS sizeof(sphere_shaders) / sizeof(void *)

#endif
