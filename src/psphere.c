/*
 * psphere.c: This file contains the implementation details for a procedurally
 *            shaded sphere object.
 *
 * Author:    Scott Gigawatt
 *
 * Version:   22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "psphere.h"
#include "mem.h"
#include "veclib3d.h"

/* 
 * psphere_init: Allocates memory for, initializes, and returns a new sphere.
 *
 * Parameters:  in      - The file containing the sphere specifications.
 *              objtype - Represents the type of object to initialize.
 *
 * Return:      A pointer to the newly initialized sphere.
 */
obj_t *psphere_init(FILE *in, int objtype) {
    obj_t  *obj = sphere_init(in, objtype); /* The new procedural sphere     */
    int    rc   = 0;                        /* The read count                */
    double i;                               /* Index of the shader function  */

    /* Get the index of the shader function and check for errors */
    if (( rc = vec_get1(in, &i) ) != 1) {
        msg_exit(stderr,"psphere_init: error: invalid read count");
    }

    consume_line(in);

    /* Check to ensure index is valid */
    if ( (int)i >= NUM_SSHADERS ) {
        msg_exit(stderr,"psphere_init: error: shader index out of bounds");
    }

    /* Set the shader function pointer for the sphere */
    obj->getamb = sphere_shaders[(int)i];

    return obj;
}

/*
 * psphere0_amb: Procedurally shades a sphere by creating alternating
 *              bands of color.
 *
 * Parameters:  obj  - The sphere to procedurally shade.
 *              ivec - The (r, g, b) intensity values.
 */
void psphere0_amb(obj_t *obj, double *ivec) {
    sphere_t *sphere = (sphere_t *)(obj->priv); /* The sphere to shade    */
    double  dir[VEC_SIZE];                      /* Direction unit vector */
    double  sum;                                /* A weighted sum        */

    /* Get the ambient light information and the direction unit vector */
    vec_scale3(1.0, obj->material.ambient, ivec);
    vec_diff3(sphere->center, obj->hitloc, dir);

    /* Compute sum for color bands */
    sum = 1000 + dir[0] * dir[1] * dir[1] / 100 + dir[0] * dir[1] / 100;

    if (( (int)sum ) & 1) {
        *ivec = 0;       /* Zap red intensity  */
    } else {
        *(ivec + 2) = 0; /* Zap blue intensity */
    }
}

/*
 * psphere1_amb: Procedurally shades a sphere by creating alternating
 *              colored circles.
 *
 *
 * Parameters:  obj  - The sphere to procedurally shade.
 *              ivec - The (r, g, b) intensity values.
 */
void psphere1_amb(obj_t *obj, double *ivec) {
    double *loc = obj->hitloc; /* The hit location of the object */
    double sum;                /* A weighted sum                 */
    
    /* Compute the sum for color circles */
    sum = sqrt( *(loc) * *(loc) + *(loc + 1) * *(loc + 1) );

    /* Compute product for circle shading procedure */
    if (( (int)sum % 2 ) == 0) {
        /* Set intensity to red   */
        *ivec       = 1;
        *(ivec + 1) = 0;
        *(ivec + 2) = 0;
    } else {
        /* Set intensity to white */
        *ivec       = 1;
        *(ivec + 1) = 1;
        *(ivec + 2) = 1;
    }
}

/*
 * psphere2_amb: Procedurally shades a sphere by creating cazy looking
 *               asymtotic lines.
 *
 *
 * Parameters:  obj  - The sphere to procedurally shade.
 *              ivec - The (r, g, b) intensity values.
 */
void psphere2_amb(obj_t *obj, double *ivec) {
    double *loc = obj->hitloc; /* The hit location of the object */
    double sum;                /* A weighted sum                 */
    
    /* Compute the sum for asymtotic lines */
    sum = sin( *(loc) * *(loc + 1) *  *(loc + 2) * *(loc + 2) );

    /* Compute product for circle shading procedure */
    if ( sum < .5 ) {
        /* Set intensity to red   */
        *ivec       = 0;
        *(ivec + 1) = 4;
        *(ivec + 2) = 1;
    } else {
        /* Set intensity to white */
        *ivec       = 8;
        *(ivec + 1) = 8;
        *(ivec + 2) = 0;
    } 
}
