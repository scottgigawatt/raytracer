/*
 * light.c: This file contains the function implementations for different 
 *          light sources.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#include <math.h>
#include "light.h"
#include "mem.h"
#include "raytrace.h"

/*
 * light_init: Initializes and returns a new diffuse light source object.
 *
 * Parameters: in      - The file containing the lighting data.
 *             objtype - The object type identifier.
 *
 * Return:     The newly created light object.
 */
obj_t *light_init(FILE *in, int objtype) {
    obj_t   *obj = object_init(in, objtype);           /* The light object */
    light_t *new = (light_t *)Malloc(sizeof(light_t)); /* The new light    */
    int     rc   = 0;                                  /* The read count   */

    /* Link the light to the object structure */
    obj->priv    = new;
    obj->destroy = light_destroy;
    obj->dump    = light_dump;

    /* Get the light emissivity information and check for errors */
    if (( rc = vec_get3(in, obj->emissivity) ) != VEC_SIZE) {
        msg_exit(stderr, "light_init: error: invalid center read count");
    }

    consume_line(in);

    /* Get the center of the light and check for errors */
    if (( rc = vec_get3(in, new->center) ) != VEC_SIZE) {
        msg_exit(stderr, "light_init: error: invalid center read count");
    }

    consume_line(in);

    return obj;
}

/* 
 * light_dump: Dumps the contents of the light object to the specified 
 *             output file.
 *
 * Parameters: out - The file to which the plane object will be dumped.
 *             obj - The plane object to dump.
 *
 * Return:     EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int light_dump(FILE *out, obj_t *obj) {
    light_t *light = (light_t *)obj->priv; /* The light object to dump */

    /* Print out the light information */
    fprintf(out,  "Dumping object of type Light\n\nLight data\n");
    vec_prn3(out, "emissivity - ", obj->emissivity);
    vec_prn3(out, "center     - ", light->center);

    return EXIT_SUCCESS;
}

/* 
 * default_getamb: Gets the ambient light information from the specified 
 *                 object.
 *
 * Parameters:     obj     - The object containing ambient light information.
 *                 ambient - Storage for the ambient light information.
 */
void default_getamb(obj_t *obj, double *ambient) {
    /* Copy the ambient light information from the object */
    vec_scale3(1.0, obj->material.ambient, ambient);
}

/* 
 * default_getdiff: Gets the diffuse light information from the specified object.
 *
 * Parameters:      obj     - The object containing diffuse light information.
 *                  diffuse - Storage for the diffuse light information.
 */
void default_getdiff(obj_t *obj, double *diffuse) {
    /* Copy the diffuse light information from the object */
    vec_scale3(1.0, obj->material.diffuse, diffuse);
}

/* 
 * default_getspec: Gets the specular light information from the specified 
 *                  object.
 *
 * Parameters:      obj      - The object containing specular light information.
 *                  specular - Storage for the specular light information.
 */
void default_getspec(obj_t *obj, double *specular) {
    /* Copy the specular light information from the object */
    vec_scale3(1.0, obj->material.specular, specular);
}

/* 
 * default_getemiss: Gets the light emissivity information from the specified 
 *                   object.
 *
 * Parameters:       obj      - The object containing emissivity information.
 *                   specular - Storage for the light emissivity information.
 */
void default_getemiss(obj_t *obj, double *emissivity) {
    /* Copy the light emissivity information from the object */
    vec_scale3(1.0, obj->emissivity, emissivity);
}

/* 
 * diffuse_illumination: Gets the diffuse light information from the specified 
 *                       object.
 *
 * Parameters: model  - A pointer to the world model containing the lights.
 *             hitobj - The object that was hit by the ray.
 *             ivec   - The (r, g, b) intensity vector.
 */
void diffuse_illumination(model_t *model, obj_t *hitobj, double *ivec) {
    link_t *cursor = NULL; /* Cursor into the list of lights */

    /* Iterate over the scene objects */
    for (cursor = model->lights->head; cursor; cursor = cursor->next) {
        /* Process the light object */
        process_light(model->scene, hitobj, (obj_t *)cursor->item, ivec);
    }
}


/* 
 * process_light: Processes the diffuse lighting information for the specified
 *                object.
 *
 * Parameters:    lst      - The objects in the world scene.
 *                hitobj   - The object that was hit by the ray.
 *                lightobj - The current light source.
 *                ivec     - The (r, g, b) intensity vector.
 *
 * Return:        EXIT_SUCCESS if no errors were encountered, failure otherwise.
 */
int process_light(list_t *lst, obj_t *hitobj, obj_t *lightobj, double *ivec) {
    light_t *light = (light_t *)lightobj->priv; /* The light to process      */
    obj_t   *obj   = NULL;                      /* The occluding object      */
    double  dir[VEC_SIZE];                      /* Unit vector direction     */
    double  diffuse[VEC_SIZE];                  /* Diffuse light information */
    double  light_dist;                         /* Distance to the light     */
    double  obj_dist;                           /* Distance to the object    */
    double  cos;                                /* Cosine of light angle     */
    int     i;                                  /* Counter                   */
    
    /* Compute direction from the hit point to the light source */
    vec_diff3(hitobj->hitloc, light->center, dir);

    /* Find the distance to the light source */
    light_dist = vec_length3(dir);

    /* Compute direction unit vector */
    vec_unit3(dir, dir);

    /* Check to see if the light is self-occluded */
    if ( (cos = vec_dot3(hitobj->normal, dir)) < 0 ) {
        return MISS;
    }

    /* See if there is an object in front of the light source */
    obj = find_closest_obj(lst, hitobj->hitloc, dir, hitobj, &obj_dist);

    /* Check to see if the object is occluding the light source */
    if ( (obj != NULL) && (obj_dist < light_dist) ) {
        /* Debugging information */
        #ifdef DBG_DIFFUSE
            ivec_prn1(stderr, "hit object occluded by   ", &obj->objid);
            vec_prn1(stderr,  "distance was             ", &obj_dist);
        #endif

        return MISS;
    }

    /* Get diffuse lighting information from the object */
    hitobj->getdiff(hitobj, diffuse);
    
    /* Compute the illumination information */
    for (i = 0; i < VEC_SIZE; ++i) {
        *(ivec + i) += diffuse[i] * lightobj->emissivity[i] * cos / light_dist;
    }

    /* Debugging information */
    #ifdef DBG_DIFFUSE
        ivec_prn1(stderr, "hit object id was        ", &hitobj->objid);
        vec_prn3(stderr,  "hit point was            ", hitobj->hitloc);
        vec_prn3(stderr,  "normal at hitpoint       ", hitobj->normal);
        ivec_prn1(stderr, "light object id was      ", &lightobj->objid);
        vec_prn3(stderr,  "light center was         ", light->center);
        vec_prn3(stderr,  "unit vector to light is  ", dir);
        vec_prn1(stderr,  "distance to light is     ", &light_dist);
        vec_prn1(stderr,  "cos is                   ", &cos);
        vec_prn3(stderr,  "emissivity of the light  ", lightobj->emissivity);
        vec_prn3(stderr,  "diffuse reflectivity     ", diffuse);
        vec_prn3(stderr,  "current ivec             ", ivec);
    #endif

    return EXIT_SUCCESS;
}

/* 
 * light_destroy: Destroys the specified light object.
 *
 * Parameters:    obj - The light object to destroy.
 */
void light_destroy(obj_t *obj) {
    /* Release memory associated with the light object */
    Free(obj->priv);
}
