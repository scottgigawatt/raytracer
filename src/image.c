/*
 * image.c: This file contains function implementations for creating a ray
 *          traced image.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#include "image.h"
#include "mem.h"
#include "object.h"
#include "veclib3d.h"

/* 
 * make_image: Creates a new image based on the specified model.
 *
 * Parameters: model - The model on which the image will be based.
 */
void make_image(model_t *model) {
    unsigned char *pixmap = NULL; /* The image data                       */
    unsigned char *pixloc = NULL; /* The location of the current pixel    */
    int           vals[VEC_SIZE]; /* The width, height, and max color val */
    int           i;              /* Counter variable                     */
    int           j;              /* Counter variable                     */

    /* Initialize PPM header values (x, y) and maximum color value */
    vals[0] = model->proj->win_size_pixel[0];
    vals[1] = model->proj->win_size_pixel[1];
    vals[2] = MAX_COLOR;

    /* Allocate space for the image data */
    pixmap = (unsigned char *)Malloc(vals[0] * vals[1] * PIXEL_SIZE);

    /* Loop through the pixmap and create the image pixels */
    for (i = 0; i < vals[1]; ++i) {
        for (j = 0; j < vals[0]; ++j) {
            /* Get the location of the next pixel */
            pixloc = pixmap + (vals[0] * i * PIXEL_SIZE) + (j * PIXEL_SIZE);

            /* Create the next pixel in the image */
            make_pixel(model, j, vals[1] - i, pixloc);

            /* Debugging information */
            #ifdef DBG_PIX
                fprintf(stderr, "\nPIX %4d %4d - ", vals[0], vals[1]);
            #endif
        }
    }
    
    /* Write the PPM image data to standard out */
    write_ppm(pixmap, ID_COLOR, vals, stdout);

    /* Free any memory associated with the ray tracer */
    dalloc(model, pixmap);
}

/* 
 * make_pixel: Creates a new pixel based on the specified model.
 *
 * Parameters: model  - The model on which the pixel color will be based.
 *             x      - The x pixel coordinate.
 *             y      - The y pixel coordinate.
 *             pixval - The newly computed pixel value (r, g, b).
 */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval) {
    double *world = alloca(VEC_SIZE * sizeof(double)); /* World coordinates */
    double *ivec  = alloca(VEC_SIZE * sizeof(double)); /* Intensity values  */
    double *total = alloca(VEC_SIZE * sizeof(double)); /* Total intensity   */
    double dir[VEC_SIZE];                              /* Direction vector  */
    int    i;                                          /* Counter           */

    /* Initialize total intensity vector to zero */
    *total = *(total + 1) = *(total  + 2) = 0.0;

    /* Check to see if anti-aliasing is turned off */
    #ifndef AA_SAMPLES
        #define AA_SAMPLES 1
    #endif

    /* Take multiple samples for anti-aliasing */
    for (i = 0; i < AA_SAMPLES; ++i) {
        /* Initialize the intensity to zero */
        *ivec = *(ivec + 1) = *(ivec  + 2) = 0.0;

        /* Get the world coordinates */
        map_pix_to_world(model->proj, x, y, world);

        /* Debugging information */
        #ifdef DBG_PIX
            fprintf(stderr, "\nWRL (%5.1lf, %5.1lf) - ", *(world), 
                                                         *(world + 1));
        #endif

        /* Compute direction vector from view point to the world */
        vec_diff3(model->proj->view_point, world, dir);
        vec_unit3(dir, dir);

        /* Trace a ray of light to the world scene */
        ray_trace(model, model->proj->view_point, dir, ivec, 0.0, NULL);
        vec_sum3(ivec, total, total);
    }

    vec_scale3((1.0 / AA_SAMPLES), total, ivec);

    /* Clamp each element of intensity to the range [0.0, 1.0] */
    for (i = 0; i < VEC_SIZE; ++i) {
        if (*(ivec + i) < 0.0) {
            *(ivec + i) = 0.0;
        } else if (*(ivec + i) > 1.0) {
            *(ivec + i) = 1.0;
        }
    }

    /* Set (r, g, b) components of pixel */
    for (i = 0; i < VEC_SIZE; ++i) {
        *(pixval + i) = *(ivec + i) * MAX_COLOR;
    }
}

/* 
 * map_pix_to_world: Maps the 2D screen coordinates to 3D world coordinates.
 *
 * Parameters:       proj  - A pointer to a projection definition.
 *                   x     - The x pixel coordinate.
 *                   y     - The y pixel coordinate.
 *                   world - A pointer to the world coordinates (x, y, z).
 */
void map_pix_to_world(proj_t *proj, int x, int y, double *world) {
    double rx; /* Random x value */
    double ry; /* Random y value */

    /* Compute the random pixel for anti-aliasing */
    rx = randpix(x); 
    ry = randpix(y);

    /* Map the x value */
    *(world + 0)  = rx / (proj->win_size_pixel[0] - 1)
                              *  proj->win_size_world[0];
    *(world + 0) -= proj->win_size_world[0] / 2.0;

    /* Map the y value */
    *(world + 1)  = ry / (proj->win_size_pixel[1] - 1)
                              *  proj->win_size_world[1];
    *(world + 1) -= proj->win_size_world[1] / 2.0;

    /* Map the z value */
    *(world + 2)  = 0.0;
}

/*
 * randpix:    Calculates a random pixel value.
 *
 * Parameters: x - The coordinate on which to base the random pixel value.
 *
 * Return:     A random pixel value.
 */
double randpix(int x) {
   return x + ( ((double)rand() / RAND_MAX) - .5 );
}

/*
 * write_ppm:  Writes the PPM image data pointed to by buf to the specified
 *             file stream.
 *
 * Parameters: buf     - Pointer to buffer containing PPM image data to write.
 *             id      - The PPM header id.
 *             vals[0] - The image width.
 *             vals[1] - The image height.
 *             vals[2] - The maximum color value.
 *             stream  - The output file stream.
 */
void write_ppm(unsigned char *buf, char *id, int *vals, FILE *stream) {
    int num_pix = *(vals) * *(vals + 1); /* The number of pixels */
    int wc      = 0;                     /* The write count      */

    /* Print PPM header information */
    fprintf(stream, "%s %d %d %d\n", id, *(vals), *(vals + 1), *(vals + 2));
    
    /* Write color PPM data */
    if (!strncmp(id, ID_COLOR, sizeof(ID_COLOR))) {
        wc = fwrite(buf, PIXEL_SIZE, num_pix, stream);
    /* Write grayscale PPM data */
    } else if (!strncmp(id, ID_GRAY, sizeof(ID_GRAY))) {
        wc = fwrite(buf, CHAR_SIZE, num_pix, stream);
    /* Header id was invalid */
    } else {
        msg_exit(stderr, "write_ppm: error: invalid header id");
    }

    /* Check for write errors */
    if (wc != num_pix) {
        msg_exit(stderr, "write_ppm: error: invalid pixel count");
    }
}

/*
 * dalloc:     Releases any memory associated with the ray tracer.
 *
 * Parameters: model  - The model to to free.
 *             pixmap - The image data to free.
 */
void dalloc(model_t *model, unsigned char *pixmap) {
    link_t *cursor = NULL; /* Cursor into the scene list      */
    obj_t  *obj    = NULL; /* The current object in the scene */

    /* Check for an empty scene */
    if (model->scene) {
        /* Iterate over the scene object list */
        for (cursor = model->scene->head; cursor; cursor = cursor->next) {
            /* Get the current object */
            obj = (obj_t *)cursor->item;

            /* Free memory associated with the object */
            obj->destroy(obj);
        }
    }

    /* Check for an empty list */
    if (model->lights) {
        /* Iterate over the light list */
        for (cursor = model->lights->head; cursor; cursor = cursor->next) {
            /* Get the current object */
            obj = (obj_t *)cursor->item;

            /* Free memory associated with the object */
            obj->destroy(obj);
        }
    }
    
    /* Delete the list of scene and light objects */
    list_del(model->scene);
    list_del(model->lights);

    /* Free memory associated with the projection, model, and image */
    Free(model->proj); 
    Free(model); 
    Free(pixmap);
}
