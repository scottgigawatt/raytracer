/*
 * image.h: This header file contains function prototypes for creating a
 *          raytraced image.
 *
 * Author:  Scott Gigawatt
 *
 * Version: 22 March 2011
 */

#ifndef IMAGE_H
#define IMAGE_H

#define MAX_COLOR  255
#define ID_COLOR   "P6"
#define ID_GRAY    "P5"
#define PIXEL_SIZE 3 * sizeof(unsigned char)
#define CHAR_SIZE  sizeof(unsigned char)

#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "projection.h"
#include "raytrace.h"

/* Creates a new image based on the specified model */
void make_image(model_t *model);

/* Creates a new pixel based on the specified model */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval);

/* Maps the 2D screen coordinates to 3D world coordinates */
void map_pix_to_world(proj_t *proj, int x, int y, double *world);

/* Calculates a random pixel value */
double randpix(int x);

/* Writes the PPM image data pointed to by *buf to the specified file */
void write_ppm(unsigned char *buf, char *id, int *vals, FILE *stream);

/* Releases any memory associated with the ray tracer */
void dalloc(model_t *model, unsigned char *pixmap);

#endif
