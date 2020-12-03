/*
 * plane.c: This file contains the implementation details for a plane object.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "plane.h"
#include "mem.h"
#include "veclib3d.h"

/* 
 * plane_init: Allocates memory for, initializes, and returns a new plane.
 *
 * Parameters: in      - The file containing the plane specifications.
 *             objtype - Represents the type of object to initialize.
 *
 * Return:     A pointer to the newly initialized plane.
 */
obj_t *plane_init(FILE *in, int objtype) {
    obj_t   *obj = object_init(in, objtype);           /* The plane object */
    plane_t *new = (plane_t *)Malloc(sizeof(plane_t)); /* The new plane    */
    int     rc   = 0;                                  /* The read count   */

    /* Link the plane to the object structure */
    obj->priv    = new;
    obj->hits    = hits_plane;
    obj->destroy = plane_destroy;
    obj->dump    = plane_dump;
    new->priv    = NULL;
     
    /* Get the normal vector information (x, y, z) and check for errors */
    if (( rc = vec_get3(in, new->normal) ) != VEC_SIZE) {
        msg_exit(stderr,"plane_init: error: invalid read count");
    }

    consume_line(in);

    /* Read in the point vector information (x, y, z) and check for errors */
    if (( rc = vec_get3(in, new->point) ) != VEC_SIZE) {
        msg_exit(stderr,"plane_init: error: invalid read count");
    }

    consume_line(in);

    return obj;
}

/* 
 * plane_dump: Dumps the contents of the plane object to the specified 
 *             output file.
 *
 * Parameters: out - The file to which the plane object will be dumped.
 *             obj - The plane object to dump.
 *
 * Return:     EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int plane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv; /* The plane object to dump */

    fprintf(out, "Dumping object of type Plane\n");

    /* Print out the object reflectivity information */
    material_dump(out, &obj->material);

    /* Print out the plane information */
    fprintf(out, "\nPlane data\n");

    /* Print out the normal vector information */
    vec_prn3(out, "normal - ", plane->normal);

    /* Print out the point vector information */
    vec_prn3(out, "point  - ", plane->point);

    return EXIT_SUCCESS;
}

/* 
 * hits_plane: Determines if a ray hits a plane object, returning the 
 *             distance to the point on the plane.
 *
 * Parameters: base - The origins of the ray (x, y, z).
 *             dir  - The direction of the ray (x, y, z).
 *             obj  - The plane object we want to hit.
 *
 * Return:     The distance to the hit location.
 */
double hits_plane(double *base, double *dir, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv; /* The plane to test for a hit   */
    double  hit_loc[VEC_SIZE];             /* The hit location of the ray   */
    double  distance;                      /* The distance to the hit point */
    double  a;                             /* Value a for distance formula  */
    double  b;                             /* Value b for distance formula  */
    double  c;                             /* Value c for distance formula  */

    /* Compute formula values for calculating distance */
    a = vec_dot3(plane->normal, plane->point);
    b = vec_dot3(plane->normal, base);
    c = vec_dot3(plane->normal, dir);

    /* Find the distance to the plane */
    distance = (a - b) / c;

    /* Find the hit location */
    vec_scale3(distance, dir, hit_loc);
    vec_sum3(base, hit_loc, hit_loc);

    if (distance < 0 || hit_loc[2] > 0.01 || c == 0) {
        /* Ray missed the plane*/
        distance = MISS;
    } else {
        /* Ray hit the sphere, save hit location and normal */
        vec_scale3(1.0, hit_loc, obj->hitloc);
        vec_scale3(1.0, plane->normal, obj->normal);
    }

    return distance;
}

/* 
 * plane_destroy: Destroys the specified plane object.
 *
 * Parameters:    obj - The plane object to destroy.
 */
void plane_destroy(obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv; /* The plane to destroy */

    /* Release memory associated with the plane private data */
    Free(plane->priv);
    /* Release memory associated with the plane object */
    Free(obj->priv);
}
