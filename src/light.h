/*
 * light.h: This header file contains the implementation specifications for
 *          different light sources.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "list.h"
#include "model.h"
#include "object.h"
#include "veclib3d.h"

/* Represents a source of light */
typedef struct light_type {
    double center[VEC_SIZE];  /* The center location of the light source */
} light_t;

/* Allocates, initializes and returns a new diffuse light source object */
obj_t *light_init(FILE *in, int objtype);

/* Dumps the contents of the light object to the specified file */
int light_dump(FILE *out, obj_t *obj);

/* Gets the ambient light information from the specified object */
void default_getamb(obj_t *obj, double *ambient);

/* Gets the diffuse light information from the specified object */
void default_getdiff(obj_t *obj, double *diffuse);

/* Gets the specular light information from the specified object */
void default_getspec(obj_t *obj, double *specular);

/* Gets the emissivity light information from the specified object */
void default_getemiss(obj_t *obj, double *emissivity);

/* Processes the diffuse light information for the world objects */
void diffuse_illumination(model_t *model, obj_t *hitobj, double *ivec);

/* Processes the diffuse lighting information for the specified object */
int process_light(list_t *lst, obj_t *hitobj, obj_t *lightobj, double *ivec);

/* Destroys the specified light object */
void light_destroy(obj_t *obj);

#endif
