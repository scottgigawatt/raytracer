/*
 * raytrace.h: This file contains function specifications for tracing a ray
 *             along a path.
 *
 * Author:     Scott Gigawatt
 *
 * Version:    22 March 2011
 */

#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "object.h"
//#include "model.h"

/* Processes the diffuse light information for the world objects */
void diffuse_illumination(model_t *model, obj_t *hitobj, double *ivec);

/* Traces a single ray and returns the intensity of the light encountered */
void ray_trace(model_t *model, double *base, double *dir, double *ivec,
                               double total_dist, obj_t *last_hit);

/* Determines the nearest object that is hit by the ray */
obj_t *find_closest_obj(list_t *scene, double *base, double *dir,
                        obj_t *last_hit, double *min_dist);

#endif
