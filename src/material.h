/*
 * material.h: This header file contains the implementation specifications
 *             object reflectivity materials.
 *
 * Author:     Scott Gigawatt
 *
 * Version:    22 March 2011
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include "veclib3d.h"

/* The material of which the object is composed */
typedef struct material_type{
    double ambient [VEC_SIZE]; /* Ambient light information  */
    double diffuse [VEC_SIZE]; /* Diffuse light information  */
    double specular[VEC_SIZE]; /* Specular light information */
} material_t;

/* Consumes a line from the specified file */
void consume_line(FILE *in);

/* Reads in the ambient, diffuse, and specular reflectivity information */
int material_init(FILE *in, material_t *mat);

/* Dumps the material information to the specified file */
int material_dump(FILE *out, material_t *mat);

#endif
