/*
 * tplane.c: This file contains the implementation details for a tiled plane
 *           object.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "tplane.h"
#include "mem.h"
#include "veclib3d.h"

/* 
 * tplane_init: Allocates memory for, initializes, and returns a new tiled 
 *              plane.
 *
 * Parameters:  in      - The file containing the tiled plane specifications.
 *              objtype - Represents the type of object to initialize.
 *
 * Return:      A pointer to the newly initialized tiled plane.
 */
obj_t *tplane_init(FILE *in, int objtype) {
    obj_t    *obj   = plane_init(in, objtype);              /* The object   */
    plane_t  *plane = (plane_t *)obj->priv;                 /* The plane    */
    tplane_t *new   = (tplane_t *)Malloc(sizeof(tplane_t)); /* Tiled plane  */
    int      rc     = 0;                                    /* Read count   */

    /* Link tiled plane structure and override the reflectivity functions */
    plane->priv  = new;
    obj->getamb  = tp_amb;
    obj->getdiff = tp_diff;
    obj->getspec = tp_spec; 
    obj->hits    = hits_plane;
    obj->destroy = plane_destroy;
    obj->dump    = plane_dump;

    /* Get the x direction vector and check for errors */
    if (( rc = vec_get3(in, new->xdir) ) != VEC_SIZE) {
        msg_exit(stderr,"tplane_init: error: invalid read count");
    }

    consume_line(in);

    /* Get the plane width and height and check for errors */
    if (( rc = vec_get2(in, new->size) ) != VEC_SIZE - 1) {
        msg_exit(stderr,"tplane_init: error: invalid read count");
    }

    consume_line(in);

    /* Get the background material information */
    material_init(in, &new->background);

    /* Compute the rotation matrix (used for hits function) */
    vec_unit3(new->xdir, new->rotmat[0]);
    vec_unit3(plane->normal, new->rotmat[2]);
    vec_cross3(new->rotmat[2], new->rotmat[0], new->rotmat[1]);

    return obj;
}

/* 
 * tplane_dump: Dumps the contents of the plane object to the specified 
 *              output file.
 *
 * Parameters:  out - The file to which the plane object will be dumped.
 *              obj - The plane object to dump.
 *
 * Return:      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int tplane_dump(FILE *out, obj_t *obj) {
    plane_t  *plane  = (plane_t *)obj->priv;    /* The plane object to dump  */
    tplane_t *tplane = (tplane_t *)plane->priv; /* The tplane object to dump */

    fprintf(out, "Dumping object of type TPlane\n");

    /* Dump standard plane data */
    plane_dump(stderr, obj);

    /* Print out the tplane information */
    fprintf(out, "\nTPlane data\n");

    /* Print out the x direction vector information */
    vec_prn3(out, "xdir - ", tplane->xdir);

    /* Print out the plane size information */
    vec_prn2(out, "size - ", tplane->size);

    /* Print out the plane rotation matrix information */
    mat_prn3(out, "rotation matrix - ", tplane->rotmat);

    /* Print out the plane background material information */
    material_dump(out, &tplane->background);

    return EXIT_SUCCESS;
}

/*
 * tp_amb:     Retrieves the ambient lighting information for the specified
 *             tiled plane object.
 *
 * Parameters: obj     - The object containing the tiled plane data.
 *             ambient - Storage for the ambient lighting information.
 */
void tp_amb(obj_t *obj, double *ambient) {
   plane_t  *plane  = (plane_t *)obj->priv;    /* The plane object */
   tplane_t *tplane = (tplane_t *)plane->priv; /* The tiled plane  */

   /* Retreive appropriate material information */
   if (tp_select(obj)) {
       vec_scale3(1.0, obj->material.ambient, ambient);
   } else {
       vec_scale3(1.0, tplane->background.ambient, ambient);
   }
}

/*
 * tp_diff:    Retrieves the diffuse lighting information for the specified
 *             tiled plane object.
 *
 * Parameters: obj     - The object containing the tiled plane data.
 *             diffuse - Storage for the diffuse lighting information.
 */
void tp_diff(obj_t *obj, double *diffuse) {
   plane_t  *plane  = (plane_t *)obj->priv;    /* The plane object */
   tplane_t *tplane = (tplane_t *)plane->priv; /* The tiled plane  */

   /* Retreive appropriate material information */
   if (tp_select(obj)) {
       vec_scale3(1.0, obj->material.diffuse, diffuse);
   } else {
       vec_scale3(1.0, tplane->background.diffuse, diffuse);
   }
}

/*
 * tp_spec:    Retrieves the specular lighting information for the specified
 *             tiled plane object.
 *
 * Parameters: obj      - The object containing the tiled plane data.
 *             specular - Storage for the specular lighting information.
 */
void tp_spec(obj_t *obj, double *specular) {
   plane_t  *plane  = (plane_t *)obj->priv;    /* The plane object */
   tplane_t *tplane = (tplane_t *)plane->priv; /* The tiled plane  */

   /* Retreive appropriate material information */
   if (tp_select(obj)) {
       vec_scale3(1.0, obj->material.specular, specular);
   } else {
       vec_scale3(1.0, tplane->background.specular, specular);
   }
}

/*
 * tp_select:  Determines whether the object hit location lies within the
 *             background or foreground of the tiled plane.
 *
 * Parameters: obj - The object containing the tiled plane data.
 *
 * Return:     Whether the hit location lies in the background or foreground.
 */
int tp_select(obj_t *obj) {
   plane_t  *plane  = (plane_t *)obj->priv;    /* The plane object        */
   tplane_t *tplane = (tplane_t *)plane->priv; /* The tiled plane         */
   double   hitloc[VEC_SIZE];                  /* The new hit point       */
   int relx;                                   /* The relative x position */
   int rely;                                   /* The relative y position */

   /* Transform the finite plane coordinates */
   vec_diff3(plane->point, obj->hitloc, hitloc);
   mat_xform3(tplane->rotmat, hitloc, hitloc);

   /* Compute the relative x an y positions */
   relx = (int)(10000 + hitloc[0] / tplane->size[0]);
   rely = (int)(10000 + hitloc[1] / tplane->size[1]);

   return (relx + rely) % 2;
}
