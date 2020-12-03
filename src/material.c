/*
 * material.c: This file contains the implementation details for reading in
 *             and dumping out reflectivity material information.
 *
 * Author:     Scott Gigawatt
 *
 * Version:    22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "material.h"

/* 
 * material_init: Reads in the ambient, diffuse, and specular reflectivity 
 *                information from the specified file.
 *
 * Parameters:    in  - The file containing the material information.
 *                mat - A pointer to the structure to store the material 
 *                      reflectivity information.
 *
 * Return:        EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int material_init(FILE *in, material_t *mat) {
    int  rc = 0;        /* The read count */

    /* Read in the ambient light information and check for errors */
    if (( rc = vec_get3(in, mat->ambient) ) != VEC_SIZE) {
        msg_exit(stderr, "material_init: error: invalid read count");
    }

    consume_line(in);

    /* Read in the diffuse light information and check for errors */
    if (( rc = vec_get3(in, mat->diffuse) ) != VEC_SIZE) {
        msg_exit(stderr, "material_init: error: invalid read count");
    }

    consume_line(in);

    /* Read in the specular light information and check for errors */
    if (( rc = vec_get3(in, mat->specular) ) != VEC_SIZE) {
        msg_exit(stderr, "material_init: error: invalid read count");
    }

    consume_line(in);

    return EXIT_SUCCESS;
}

/* 
 * material_dump: Dumps the contents of the sphere object reflectivity material
 *                to the specified output file.
 *
 * Parameters:    out - The file to which the material info will be dumped.
 *                mat - The material structure to dump.
 *
 * Return:        EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int material_dump(FILE *out, material_t *mat) {
    fprintf(out, "Material data - \n");

    /* Print out ambient reflectivity information */
    vec_prn3(out, "ambient  - ", mat->ambient);

    /* Print out diffuse reflectivity information */
    vec_prn3(out, "diffuse  - ", mat->diffuse);

    /* Print out specular reflectivity information */
    vec_prn3(out, "specular - ", mat->specular);

    return EXIT_SUCCESS;
}
