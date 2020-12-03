/*
 * raytrace.c: This file contains function implementations for tracing a ray
 *             along a path.
 *
 * Author:     Scott Gigawatt
 *
 * Version:    22 March 2011
 */

#include <limits.h>
#include "raytrace.h"
#include "veclib3d.h"

/*
 * ray_trace:  This function traces a single ray and returns the composite 
 *             intensity of the light it encounters. 
 *
 * Parameters: model      - A pointer to the model container.
 *             base       - The viewer location (x, y, z), or previous hit.
 *             dir        - Unit vector (x, y, z) direction to the object.
 *             ivec       - The intensity (r, g, b) return location.
 *             total_dist - The distance ray has traveled so far.
 *             last_hit   - The object that reflected this ray or NULL.
 */
void ray_trace(model_t *model, double *base, double *dir, double *ivec, 
                                       double total_dist, obj_t *last_hit) {
    obj_t  *closest          = NULL;              /* The closest object    */
    double specref[VEC_SIZE] = { 0.0, 0.0, 0.0 }; /* Specular reflectivity */
    double mindist;                               /* The minimum distance  */

    /* Termination condition for specular reflectivity */
    if (total_dist > MAX_DIST) {
        return;
    }
    
    /* Find the closest object */
    closest = find_closest_obj(model->scene, base, dir, NULL, &mindist);

    if (closest == NULL) {
        return;
    }
    
    /* Find the new total distance */
    total_dist += mindist;

    /* Get the ambient lighting information from the object */
    closest->getamb(closest, ivec);
    closest->getspec(closest, specref);
    
    /* Compute the diffuse lighting information for the object */
    diffuse_illumination(model, closest, ivec);

    /* Divide the intensity by the total distance */
    vec_scale3(1.0 / total_dist, ivec, ivec);

    /* Check to see if object has specular reflectivity */
    if ( vec_dot3(specref, specref) > 0 ) {
        double specint[VEC_SIZE] = { 0.0, 0.0, 0.0 }; /* Specular intensity */
        double refdir[VEC_SIZE]  = { 0.0, 0.0, 0.0 }; /* Reflection dir     */
        double norm[VEC_SIZE]    = { 0.0, 0.0, 0.0 }; /* Unit vector normal */

        /* Compute direction of reflection */
        vec_unit3(closest->normal, norm);
        vec_reflect3(dir, norm, refdir);

        /* Recursively call raytrace to get specular reflectivity */
        ray_trace(model, closest->hitloc, refdir, specint, total_dist, closest);

        /* Multiply specint by specref and store in specref */
        vec_mul3(specint, specref, specref);
    }

    /* Add specular reflectivity to the intensity vector */
    vec_sum3(ivec, specref, ivec);

    /* Hit function debugging information */
    #ifdef DBG_HIT
        fprintf(stderr, "HIT %4d: %5.1lf (%5.1lf, %5.1lf, %5.1lf) - ",
                                    closest->objid, mindist,
                                    closest->hitloc[0], 
                                    closest->hitloc[1],
                                    closest->hitloc[2]);
    #endif

    /* Ambient light debugging information */
    #ifdef DBG_AMB
        fprintf(stderr, "AMB (%5.1lf, %5.1lf, %5.1lf) - ", *(ivec),
                                                           *(ivec + 1),
                                                           *(ivec + 2));
    #endif

    /* Diffuse light debugging information */
    #ifdef DBG_DIFFUSE
        fprintf(stderr, "DIFFUSE (%5.1lf, %5.1lf, %5.1lf) - ", *(ivec),
                                                               *(ivec + 1),
                                                               *(ivec + 2));
    #endif
}

/*
 * find_closest_obj: This function determines the nearest object that is hit
 *                   by the ray.  If none of the objects in the scene is hit,
 *                   NULL is returned.
 *
 * Parameters:       scene    - A pointer to the scene.
 *                   base     - The viewer location (x, y, z), or previous hit.
 *                   dir      - Unit vector (x, y, z) direction to the object.
 *                   last_hit - The object that reflected this ray or NULL.
 *                   min_dist - Storage for the distance ray has traveled.
 *
 * Return:           The closest object in the scene.
 */
obj_t *find_closest_obj(list_t *scene, double *base, double *dir, 
                                       obj_t *last_hit, double *min_dist) {
    obj_t  *obj     = NULL;    /* The current object in the scene */
    obj_t  *closest = NULL;    /* The closest object              */
    link_t *cursor  = NULL;    /* Cursor into the scene list      */
    double min      = INT_MAX; /* The minimum distance            */
    double dist;               /* The current object's distance   */

    /* Iterate over the scene objects */
    for (cursor = scene->head; cursor; cursor = cursor->next) {
        /* Get the current object */
        obj = (obj_t *)cursor->item;

        /* Do not check the same object twice */
        if (obj != last_hit) {
            /* Find distance to object */
            dist = obj->hits(base, dir, obj);

            /* Check to see if the object is the closest */
            if (dist < min && dist > 0.0) {
                min     = dist;
                closest = obj;
            }

            /* Debugging information */
            #ifdef DBG_FIND
                fprintf(stderr, "FND %4d: %5.1lf - ", obj->objid, dist);
            #endif
        }
    }

    /* Set the new minimum distance */
    *min_dist = min;

    return closest;
}
