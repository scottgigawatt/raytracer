/*
 * sphere.c: This file contains the implementation details for a sphere object.
 *
 * Author:   Scott Gigawatt
 *
 * Version:  22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sphere.h"
#include "mem.h"
#include "veclib3d.h"

/* 
 * sphere_init: Allocates memory for, initializes, and returns a new sphere.
 *
 * Parameters:  in      - The file containing the sphere specifications.
 *              objtype - Represents the type of object to initialize.
 *
 * Return:      A pointer to the newly initialized sphere.
 */
obj_t *sphere_init(FILE *in, int objtype) {
    obj_t    *obj = object_init(in, objtype);             /* The sphere obj */
    sphere_t *new = (sphere_t *)Malloc(sizeof(sphere_t)); /* The new sphere */
    int      rc   = 0;                                    /* The read count */
    
    /* Link the plane to the object structure */
    obj->priv    = new;
    obj->hits    = hits_sphere;
    obj->destroy = sphere_destroy;
    obj->dump    = sphere_dump;
    
    /* Read in the center vector information (x, y, z) and check for errors */
    if (( rc = vec_get3(in, new->center) ) != VEC_SIZE) {
        msg_exit(stderr, "sphere_init: error: invalid read count");
    }

    consume_line(in);

    /* Read in the radius information and check for errors */
    if (( rc = vec_get1(in, &new->radius) ) != 1) {
        msg_exit(stderr, "sphere_init: error: invalid read count");
    }

    consume_line(in);

    return obj;
}

/* 
 * sphere_dump: Dumps the contents of the sphere object to the specified 
 *              output file.
 *
 * Parameters:  out - The file to which the sphere object will be dumped.
 *              obj - The sphere object to dump.
 *
 * Return:      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int sphere_dump(FILE *out, obj_t *obj) {
    sphere_t *sphere = (sphere_t *)obj->priv; /* The sphere object to dump */

    fprintf(out, "Dumping object of type Sphere\n");

    /* Print out the object reflectivity information */
    material_dump(out, &obj->material);

    /* Print out the plane information */
    fprintf(out, "\nSphere data\n");

    /* Print out the center vector information */
    vec_prn3(out, "center - ", sphere->center);

    /* Print out the radius information */
    vec_prn1(out, "radius - ", &sphere->radius);

    return EXIT_SUCCESS;
}

/* 
 * hits_sphere: Determines if a ray hits a sphere object, returning the 
 *              distance to the point on the sphere.
 *
 * Parameters:  base - The base location of the ray.
 *              dir  - The unit vector direction of the ray.
 *              obj  - The sphere object to test.
 *
 * Return:      The distance to the hit location.
 */
double hits_sphere(double *base, double *dir, obj_t *obj) {
    sphere_t *sphere = (sphere_t *)obj->priv; /* The sphere object         */
    double view[VEC_SIZE];                    /* The new view point        */
    double hit_loc[VEC_SIZE];                 /* Ray hit location          */
    double normal[VEC_SIZE];                  /* The normal to the sphere  */
    double distance;                          /* Hit point distance        */
    double quad;                              /* The discriminate          */
    double a;                                 /* Quadratic formula value   */
    double b;                                 /* Quadratic formula value   */
    double c;                                 /* Quadratic formula value   */

    /* Compute the new view point */
    vec_diff3(sphere->center, base, view);

    /* Compute quadratic formula values for distance formula */
    a    = vec_dot3(dir, dir);
    b    = 2 * vec_dot3(view, dir);
    c    = vec_dot3(view, view) - (sphere->radius * sphere->radius);
    quad = (b * b) - (4 * a * c);

    /* Find the distance to the sphere */
    distance = ( (-1 * b) - sqrt(quad) ) / (2 * a);

    /* Find the hit location */
    vec_scale3(distance, dir, hit_loc);
    vec_sum3(base, hit_loc, hit_loc);

    if (quad <= 0) {
        /* Ray missed the sphere */
        distance = MISS;
    } else {
        /* Ray hit the sphere, save hit location and normal */
        vec_scale3(1.0, hit_loc, obj->hitloc);
        vec_diff3(sphere->center, hit_loc, normal);
        vec_unit3(normal, obj->normal);
    }

    return distance;
}

/* 
 * sphere_destroy: Destroys the specified sphere object.
 *
 * Parameters:     obj - The sphere object to destroy.
 */
void sphere_destroy(obj_t *obj) {
    /* Release memory associated with the sphere object */
    Free(obj->priv);
}
