/*
 * veclib3d.h: This program contains function prototypes and constants for
 *             commonly used vector functions.
 *
 * Author:     Scott Gigawatt
 *
 * Version:    15 February 2011
 */

#ifndef VECLIB3D_H
#define VECLIB3D_H

#include <math.h>
#include <stdio.h>

/* The size of a 3D vector */
#define VEC_SIZE 3

/* Computes the dot product of two vectors */
double vec_dot3(double *v1, double *v2);

/* Computes the distance between two vectors */
double vec_dist3(double *v1, double *v2);

/* Scales the specified vector by a factor */
void vec_scale3(double fact, double *v1, double *v2);

/* Computes the length of the specified vector */
double vec_length3(double *v1);

/* Computes the difference between two vectors*/
void vec_diff3(double *v1, double *v2, double *v3);

/* Computes the sum of two vectors*/
void vec_sum3(double *v1, double *v2, double *v3);

/* Computes the product of two vectors*/
void vec_mul3(double *v1, double *v2, double *v3);

/* Computes the unit vector for the specified vector */
void vec_unit3(double *v1, double *v2);

/* Gets a 3D vector from the specified file. */
int vec_get3(FILE *in, double *v1);

/* Prints the specified label and 3D vector to the specified file */
void vec_prn3(FILE *out, char *label, double *v1);

/* Gets a 2D vector from the specified file. */
int vec_get2(FILE *in, double *v1);

/* Gets a 2D integer vector from the specified file. */
int ivec_get2(FILE *in, int *v1);

/* Prints the specified label and 2D vector to the specified file */
void vec_prn2(FILE *out, char *label, double *v1);

/* Prints the specified label and 2D integer vector to the specified file */
void ivec_prn2(FILE *out, char *label, int *v1);

/* Gets a 1D vector from the specified file. */
int vec_get1(FILE *in, double *v1);

/* Gets a 1D integer vector from the specified file */
int ivec_get1(FILE *in, int *v1);

/* Prints the specified label and 1D vector to the specified file */
void vec_prn1(FILE *out, char *label, double *v1);

/* Prints the specified label and 1D integer vector to the specified file */
void ivec_prn1(FILE *out, char *label, int *v1);

/* Computes the outer product of two input vectors */
void vec_cross3(double *v1, double *v2, double *v3);

/* Projects a vector onto a plane */
void vec_project3(double *n, double *v, double *w);

/* Computes the direction of a reflected ray of light */
void vec_reflect3(double *unitin, double *unitnorm, double *unitout);

/* Constructs an identity matrix */
void mat_id3(double mtx[][VEC_SIZE]);

/* Multiplies two 3 x 3 input matrices */
void mat_mul3(double x[][VEC_SIZE], double y[][VEC_SIZE], double z[][VEC_SIZE]);

/* Transposes a 3 x 3 input matrix */
void mat_xpose3(double x[][VEC_SIZE], double z[][VEC_SIZE]);

/* Linearly transposes a 3 x 3 input matrix by a 3 x 1 column vector */
void mat_xform3(double y[][VEC_SIZE], double *x,double *z);

/* Copies the specified 3 x 3 matrix. */
void mat_copy3(double x[][VEC_SIZE], double y[][VEC_SIZE]);

/* Prints the specified label and 3 x 3 matrix to the specified file */
void mat_prn3(FILE *out, char *label, double x[][VEC_SIZE]);

/* Prints an error message to the specified file and exits */
void msg_exit(FILE *out, char *msg);

#endif
