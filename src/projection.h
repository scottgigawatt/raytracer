/*
 * projection.h: This header file contains the implementation specifications
 *               for the projection information.
 *
 * Author:       Scott Gigawatt
 *
 * Version:      22 March 2011
 */

#ifndef PROJECTION_H
#define PROJECTION_H

#include <stdlib.h>
#include <stdio.h>
#include "veclib3d.h"

/* A structure to contain the projection information */
typedef struct projection_type {
    int    win_size_pixel[VEC_SIZE - 1]; /* Screen size in pixels            */
    double win_size_world[VEC_SIZE - 1]; /* Screen size in world coordinates */
    double view_point[VEC_SIZE];         /* Viewpoint point coordinates      */
} proj_t;

/* Consumes a line from the specified file */
void consume_line(FILE *in);

/* Initializes the projection information */
proj_t *projection_init(int argc, char **argv, FILE *in);

/* Dumps the projection information to the specified file */
int projection_dump(FILE *out, proj_t *proj);

#endif
