/*
 * fplane.c: This file contains the implementation details for a finite plane
 *           object.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "fplane.h"
#include "mem.h"
#include "veclib3d.h"

/*
 * fplane_init: Allocates memory for, initializes, and returns a new finite
 *              plane.
 *
 * Parameters:  in      - The file containing the finite plane specifications.
 *              objtype - Represents the type of object to initialize.
 *
 * Return:      A pointer to the newly initialized finite plane.
 */
obj_t *fplane_init(FILE *in, int objtype) {
    obj_t    *obj   = plane_init(in, objtype);              /* The object   */
    plane_t  *plane = (plane_t *)obj->priv;                 /* The plane    */
    fplane_t *new   = (fplane_t *)Malloc(sizeof(fplane_t)); /* Finite plane */
    int      rc     = 0;                                    /* Read count   */
    double   proj[VEC_SIZE];                                /* Projection   */

    /* Link the finite plane structure to the plane */
    plane->priv  = new;
    obj->hits    = hits_fplane;
    obj->destroy = plane_destroy;
    obj->dump    = fplane_dump;

    /* Get the x direction vector and check for errors */
    if (( rc = vec_get3(in, new->xdir) ) != VEC_SIZE) {
        msg_exit(stderr,"fplane_init: error: invalid read count");
    }

    consume_line(in);

    /* Get the plane width and height and check for errors */
    if (( rc = vec_get2(in, new->size) ) != VEC_SIZE - 1) {
        msg_exit(stderr,"fplane_init: error: invalid read count");
    }

    consume_line(in);

    /* Project onto plane surface */
    vec_unit3(new->xdir, new->xdir);
    vec_unit3(plane->normal, plane->normal);
    vec_project3(plane->normal, new->xdir, new->xdir);

    /* Compute the rotation matrix (used for hits function) */
    vec_unit3(new->xdir, new->rotmat[0]);
    vec_unit3(plane->normal, new->rotmat[2]);
    vec_cross3(new->rotmat[2], new->rotmat[0], new->rotmat[1]);

    return obj;
}

/*
 * fplane_dump: Dumps the contents of the plane object to the specified
 *              output file.
 *
 * Parameters:  out - The file to which the plane object will be dumped.
 *              obj - The plane object to dump.
 *
 * Return:      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int fplane_dump(FILE *out, obj_t *obj) {
    plane_t  *plane  = (plane_t *)obj->priv;    /* The plane object to dump  */
    fplane_t *fplane = (fplane_t *)plane->priv; /* The fplane object to dump */

    fprintf(out, "Dumping object of type FPlane\n");

    /* Dump standard plane data */
    plane_dump(stderr, obj);

    /* Print out the fplane information */
    fprintf(out, "\nFPlane data\n");

    /* Print out the x direction vector information */
    vec_prn3(out, "xdir - ", fplane->xdir);

    /* Print out the plane size information */
    vec_prn2(out, "size - ", fplane->size);

    /* Print out the plane rotation matrix information */
    mat_prn3(out, "rotation matrix - ", fplane->rotmat);

    return EXIT_SUCCESS;
}

/*
 * hits_fplane: Determines if a ray hits a plane object, returning the
 *              distance to the point on the plane.
 *
 * Parameters:  base - The origins of the ray (x, y, z).
 *              dir  - The direction of the ray (x, y, z).
 *              obj  - The finite plane object we want to hit.
 *
 * Return:      The distance to the hit location.
 */
double hits_fplane(double *base, double *dir, obj_t *obj) {
    plane_t  *plane  = (plane_t *)obj->priv;    /* The infinite plane */
    fplane_t *fplane = (fplane_t *)plane->priv; /* The finite plane   */
    double   hitloc[VEC_SIZE];                  /* The new hit point  */
    double   distance;                          /* Hit point distance */
    int      i;                                 /* Counter            */

    /* Check to see if ray hit the infinite plane */
    if ( (distance = hits_plane(base, dir, obj)) < 0 ) {
        return distance;
    }

    /* Transform the finite plane coordinates */
    vec_diff3(plane->point, obj->hitloc, hitloc);
    mat_xform3(fplane->rotmat, hitloc, hitloc);

    for (i = 0; i < VEC_SIZE - 1; ++i) {
        /* Check to see if the hit was within the finite plane bounds */
        if ( (hitloc[i] > fplane->size[i]) || (hitloc[i] < 0.0) ) {
            return MISS;
        }
    }

    return distance;
}
