/*
 * model.h: This header file contains the implementation specifications for
 *          a world model.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#ifndef MODEL_H
#define MODEL_H

/* The maximum size for a line of input */
#ifndef BUF_SIZE 
    #define BUF_SIZE 256
#endif 

/* Object types */
#define FIRST_TYPE   10
#define LIGHT        10
#define SPOTLIGHT    11
#define PROJECTOR    12
#define SPHERE       13
#define PLANE        14
#define FINITE_PLANE 15
#define TILED_PLANE  16
#define TEX_PLANE    17
#define REF_SPHERE   18
#define P_SPHERE     19
#define P_PLANE      20
#define PARABOLOID   21
#define CYLINDER     22
#define CONE         23
#define HYPERBOLOID  24
#define LAST_TYPE    25
#define LAST_LIGHT   12
#define TEX_FIT      1
#define TEX_TILE     2
#define MISS        -1 
#define MAX_DIST     20

#include <stdlib.h>
#include "list.h"
#include "object.h"
#include "projection.h"

/* A structure to contain the model information */
typedef struct model_type {
    proj_t *proj;   /* The projection information */
    list_t *lights; /* The lights in the scene    */
    list_t *scene;  /* The scene information      */
} model_t;

/* Read the model information from the specified file */
int model_init(FILE *in, model_t *model);

/* Dumps the model information to the specified file */
int model_dump(FILE* out, model_t *model);

/* Dumps the list of objects to the specified file */
void objlist_dump(FILE* out, list_t *scene);

/* Consumes a line from the specified file */
void consume_line(FILE *in);

#endif
