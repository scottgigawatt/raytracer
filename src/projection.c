/*
 * projection.c: This file contains the implementation details for initializing
 *               the projection information.
 *
 * Author:       Scott Gigawatt
 *
 * Version:      22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "projection.h"
#include "mem.h"

/* 
 * projection_init: Initializes the world dimensions and viewpoint.
 *
 * Parameters:      in    - The file from which the information will be read.
 *                  model - The model to initialize.
 *
 * Return:          A pointer to the projection information.
 */
proj_t *projection_init(int argc, char **argv, FILE *in) {
    proj_t *proj = (proj_t *)Malloc(sizeof(proj_t)); /* The new projection */
    int    rc    = 0;                                /* The read count     */
    int    i;                                        /* Counter            */

    /* Check for valid arguments */
    if (argc != VEC_SIZE) {
        msg_exit(stderr, "projection_init: error: invalid argument count");
    }

    /* Get the screen size in pixels (x, y) */
    for (i = 0; i < VEC_SIZE - 1; ++i) {
        proj->win_size_pixel[i] = atoi( *(argv + i + 1) );
    }

    /* Get the world dimensions (x, y) and check for errors */
    if (( rc = vec_get2(in, proj->win_size_world) ) != VEC_SIZE - 1) {
        fprintf(stderr, "world rc: %d\n", rc);
        msg_exit(stderr, "projection_init: error: invalid read count");
    }

    consume_line(in);

    /* Get the viewpoint (x, y, z) and check for errors */
    if (( rc = vec_get3(in, proj->view_point) ) != VEC_SIZE) {
        fprintf(stderr, "point rc: %d\n", rc);
        msg_exit(stderr, "projection_init: error: invalid read count");
    }

    consume_line(in);

    return proj;
}

/* 
 * projection_dump: Dumps the contents of the projection structure to the 
 *                  specified output file.
 *
 * Parameters:      out  - The file to which the projection info will be dumped.
 *                  proj - The projection structure to dump.
 *
 * Return:          EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int projection_dump(FILE *out, proj_t *proj) {
    fprintf(out, "Projection data - \n");

    /* Print out the screen size */
    ivec_prn2(out, "screen size - ", proj->win_size_pixel);

    /* Print out the world size */
    vec_prn2(out, "world size - ",   proj->win_size_world);

    /* Print out the view point */
    vec_prn3(out, "view point - ",   proj->view_point);

    return EXIT_SUCCESS;
}
