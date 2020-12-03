/*
 * model.c: This file contains the implementation specifications for
 *          a world model.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "light.h"
#include "sphere.h"
#include "psphere.h"
#include "plane.h"
#include "pplane.h"
#include "tplane.h"
#include "fplane.h"

/* Dummy initialization function for unimplemented objects */
obj_t *dummy_init(FILE *in, int objtype){ return NULL; }

/* Table of function pointers for object initialization */
static obj_t *(*obj_loaders[])(FILE *in, int objtype) = {
    light_init,   /* Placeholder for a light object             (type 10) */
    dummy_init,   /* Placeholder for a spotlight object         (type 11) */
    dummy_init,   /* Placeholder for a projector object         (type 12) */
    sphere_init,  /* Placeholder for a sphere object            (type 13) */
    plane_init,   /* Placeholder for a plane object             (type 14) */
    fplane_init,  /* Placeholder for a finite plane object      (type 15) */
    tplane_init,  /* Placeholder for a tiled plane object       (type 16) */
    dummy_init,   /* Placeholder for a textured plane object    (type 17) */
    dummy_init,   /* Placeholder for a ref sphere object        (type 18) */
    psphere_init, /* Placeholder for a procedural sphere object (type 19) */
    pplane_init,  /* Placeholder for a procedural plane object  (type 20) */
    dummy_init,   /* Placeholder for a paraboloid object        (type 21) */
    dummy_init,   /* Placeholder for a cylinder object          (type 22) */
    dummy_init,   /* Placeholder for a cone object              (type 23) */
    dummy_init    /* Placeholder for a hyperboloid object       (type 24) */
};

/* 
 * model_init: Initializes the world model structure.
 *
 * Parameters: in    - The file from which the model information will be read.
 *             model - The model to initialize.
 *
 * Return:     EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int model_init(FILE *in, model_t *model) {
    obj_t *new = NULL; /* A Pointer to the newly created object */
    int   obj_type;    /* The object type read from the model   */

    /* Read in the world scene information */
    while ( ivec_get1(in, &obj_type) == 1 ) {
        consume_line(in);

        /* Initialize the appropriate object type */
        if ( (obj_type >= FIRST_TYPE) && (obj_type <= LAST_TYPE) ) {
            /* Initialize object from object loader function table */
            new = (*obj_loaders[obj_type - FIRST_TYPE])(in, obj_type);

            /* Check for successful object initialization */
            if (new == NULL) {
                msg_exit(stderr, "model_init: error: initialization failed");
            }

            /* Add new object to appropriate scene list */
            if (obj_type == LIGHT) {
                list_add(model->lights, new);
            } else {
                list_add(model->scene, new);
            }
        }
    }

    return EXIT_SUCCESS;
}

/* 
 * model_dump: Dumps the model information to the specified file.
 *
 * Parameters: out   - The file to which the projection info will be dumped.
 *             model - The model structure to dump.
 *
 * Return:     EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int model_dump(FILE* out, model_t *model) {
    /* Dump scene object data to the specified file */
    objlist_dump(out, model->scene);

    /* Dump scene light data to the specified file  */
    objlist_dump(out, model->lights);

    return EXIT_SUCCESS;
}

/* 
 * objlist_dump: Dumps the list of objects to the specified file.
 *
 * Parameters:   out   - The file to which the object info will be dumped.
 *               scene - The list of objects in the world scene.
 */
void objlist_dump(FILE* out, list_t *scene) {
    obj_t  *obj    = NULL; /* The current object in the scene */
    link_t *cursor = NULL; /* Cursor into the scene list      */

    /* Check for an empty scene */
    if (scene) {
        /* Iterate over the scene list */
        for (cursor = scene->head; cursor; cursor = cursor->next) {
            /* Get the current object */
            obj = (obj_t *)cursor->item;
            obj->dump(stderr, obj);
        }
    }
}

/* 
 * consume_line: Consumes a line from the specified file.
 *
 * Parameters:   in - The file from which the line is read.
 */
void consume_line(FILE *in) {
    char junk[BUF_SIZE]; /* Input buffer */

    /* Consume one line from the file */
    fgets(junk, BUF_SIZE, in);
}
