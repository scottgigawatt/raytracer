/*
 * main.c:  This file contains the main function for the ray tracer program.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "raytrace.h"
#include "mem.h"
#include "image.h"

/*
 * main:       This function provides an entry point into the ray tracer
 *             program.
 *
 * Parameters: argc    - The number of command line arguments.
 *             argv[1] - The window width in pixels (x).
 *             argv[2] - The window height in pixels (y).
 *
 * Return:     EXIT_SUCCESS if no errors were encountered, failure otherwise.
 */
int main(int argc, char **argv) {
    model_t *model = (model_t *)Malloc(sizeof(model_t)); /* The world model */
    int     rc     = 0;                                  /* The read count  */

    /* Initialize the projection information */
    model->proj = projection_init(argc, argv, stdin);
    projection_dump(stderr, model->proj);

    /* Initialize the light and scene object list */
    model->lights = list_init();
    model->scene  = list_init();

    /* Initialize the model */
    rc = model_init(stdin, model);
    model_dump(stderr, model);

    /* Create the image */
    if (rc == 0) {
        make_image(model);
    }

    return EXIT_SUCCESS;
}
