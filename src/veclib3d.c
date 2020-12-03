/*
 * veclib3d.c: This program contains a library of commonly used vector
 *             functions.
 *
 * Author:     Scott Gigawatt
 *
 * Version:    15 February 2011
 */

#include <stdlib.h>
#include "veclib3d.h"

/*
 * vec_dot3:   Returns the inner product of two input vectors.
 *
 * Parameters: v1 - The first input vector.
 *             v2 - The second input vector.
 *
 * Return:     The inner product of the two input vectors.
 */
double vec_dot3(double *v1, double *v2) {
    return *(v1) * *(v2) + *(v1 + 1) * *(v2 + 1) + *(v1 + 2) * *(v2 + 2);
}

/*
 * vec_dist3:  Returns the distance between two input vectors.
 *
 * Parameters: v1 - The first input vector.
 *             v2 - The second input vector.
 *
 * Return:     The distance between the two input vectors.
 */
double vec_dist3(double *v1, double *v2) {
    double v3[VEC_SIZE]; /* The direction vector */

    /* Compute the vector from v1 to v2 */
    vec_diff3(v1, v2, v3);

    return vec_length3(v3);
}

/*
 * vec_scale3: Scales a 3D vector by a factor.
 *
 * Parameters: fact - The scaling factor.
 *             v1   - The input vector.
 *             v2   - The output vector.
 */
void vec_scale3(double fact, double *v1, double *v2) {
    int i; /* Counter */

    /* Compute the scaled vector */
    for (i = 0; i < VEC_SIZE; ++i) {
        *(v2 + i) = *(v1 + i) * fact;
    }
}

/*
 * vec_length3: Returns the length of a 3D vector.
 *
 * Parameters:  v1 - The vector whose length is desired.
 *
 * Return:      The length of the 3D vector.
 */
double vec_length3(double *v1) {
    return sqrt(*(v1) * *(v1) + *(v1 + 1) * *(v1 + 1) + *(v1 + 2) * *(v1 + 2));
}

/*
 * vec_diff3:  Computes the difference of two vectors as follows:
 *             
 *             v3 = v2 - v1
 *
 * Parameters: v1 - The first input vector.
 *             v2 - The second input vector.
 *             v3 - The output vector.
 */
void vec_diff3(double *v1, double *v2, double *v3) {
    int i; /* Counter */

    /* Compute the vector difference */
    for (i = 0; i < VEC_SIZE; ++i) {
        *(v3 + i) = *(v2 + i) - *(v1 + i);
    }
}

/*
 * vec_sum3:  Computes the sum of two vectors as follows:
 *             
 *             v3 = v1 + v2
 *
 * Parameters: v1 - The first input vector.
 *             v2 - The second input vector.
 *             v3 - The output vector.
 */
void vec_sum3(double *v1, double *v2, double *v3) {
    int i; /* Counter */

    /* Compute the vector sum */
    for (i = 0; i < VEC_SIZE; ++i) {
        *(v3 + i) = *(v1 + i) + *(v2 + i);
    }
}

/*
 * vec_mul3:  Computes the product of two vectors as follows:
 *             
 *             v3_x = v1_x * v2_x
 *             v3_y = v1_y * v2_y
 *             v3_z = v1_z * v2_z
 *
 * Parameters: v1 - The first input vector.
 *             v2 - The second input vector.
 *             v3 - The output vector.
 */
void vec_mul3(double *v1, double *v2, double *v3) {
    int i; /* Counter */

    /* Compute the vector sum */
    for (i = 0; i < VEC_SIZE; ++i) {
        *(v3 + i) = *(v1 + i) * *(v2 + i);
    }
}

/*
 * vec_unit3:  Constructs a unit vector in the direction of an input vector.
 *             
 * Parameters: v1 - The first input vector.
 *             v2 - The output unit vector.
 */
void vec_unit3(double *v1, double *v2) {
    if ( vec_length3(v1) == 0 ) {
        /* Warn vector length was 0 */
        fprintf(stderr, "error: vector length was 0");
    } else {
        /* Compute unit vector */
        vec_scale3(1 / vec_length3(v1), v1, v2);
    }
}

/*
 * vec_get3:   Gets a 3D vector from the specified file.
 *
 * Parameters: in - The file containing the vector data.
 *             v1 - Storage for the vector data.
 *
 * Return:     The number of successfully read vector values.
 */
int vec_get3(FILE *in, double *v1) {
    int  rc = 0; /* The read counter */
    int  i;      /* Counter          */

    /* Read in the vector information (x, y, z) */
    for (i = 0; i < VEC_SIZE; ++i) {
        rc += fscanf(in, "%lf", v1 + i);
    }

    return rc;
}

/*
 * vec_prn3:   Prints a label and the contents of a 3D vector to the specified
 *             output file.
 *             
 * Parameters: out   - The output file.
 *             label - The label to print.
 *             v1    - The vector to print.
 */
void vec_prn3(FILE *out, char *label, double *v1) {
    fprintf(out, "%s\n%8.3f %8.3f %8.3f\n", label, *(v1), *(v1 + 1), *(v1 + 2));
}

/*
 * vec_get2:   Gets a 2D vector from the specified file.
 *
 * Parameters: in - The file containing the vector data.
 *             v1 - Storage for the vector data.
 *
 * Return:     The number of successfully read vector values.
 */
int vec_get2(FILE *in, double *v1) {
    int  rc = 0; /* The read counter */
    int  i;      /* Counter          */

    /* Read in the vector information (x, y) */
    for (i = 0; i < VEC_SIZE - 1; ++i) {
        rc += fscanf(in, "%lf", v1 + i);
    }

    return rc;
}

/*
 * ivec_get2:  Gets a 2D integer vector from the specified file.
 *
 * Parameters: in - The file containing the vector data.
 *             v1 - Storage for the vector data.
 *
 * Return:     The number of successfully read vector values.
 */
int ivec_get2(FILE *in, int *v1) {
    int  rc = 0; /* The read counter */
    int  i;      /* Counter          */

    /* Read in the vector information (x, y) */
    for (i = 0; i < VEC_SIZE - 1; ++i) {
        rc += fscanf(in, "%d", v1 + i);
    }

    return rc;
}

/*
 * vec_prn2:   Prints a label and the contents of a 2D vector to the specified
 *             output file.
 *             
 * Parameters: out   - The output file.
 *             label - The label to print.
 *             v1    - The vector to print.
 */
void vec_prn2(FILE *out, char *label, double *v1) {
    fprintf(out, "%s\n%8.3f x %8.3f\n", label, *(v1), *(v1 + 1));
}

/*
 * ivec_prn2:  Prints a label and the contents of a 2D integer vector to the 
 *             specified output file.
 *             
 * Parameters: out   - The output file.
 *             label - The label to print.
 *             v1    - The integer vector to print.
 */
void ivec_prn2(FILE *out, char *label, int *v1) {
    fprintf(out, "%s\n%6d x %6d\n", label, *(v1), *(v1 + 1));
}

/*
 * vec_get1:   Gets a 1D vector from the specified file.
 *
 * Parameters: in - The file containing the vector data.
 *             v1 - Storage for the vector data.
 *
 * Return:     The number of successfully read vector values.
 */
int vec_get1(FILE *in, double *v1) {
    /* Attempt to read in a double value */
    return fscanf(in, "%lf", v1);
}

/*
 * ivec_get1:  Gets a 1D integer vector from the specified file.
 *
 * Parameters: in - The file containing the vector data.
 *             v1 - Storage for the vector data.
 *
 * Return:     The number of successfully read vector values.
 */
int ivec_get1(FILE *in, int *v1) {
    /* Attempt to read in a integer value */
    return fscanf(in, "%d", v1);
}

/*
 * vec_prn1:   Prints a label and the contents of a 1D vector to the specified
 *             output file.
 *             
 * Parameters: out   - The output file.
 *             label - The label to print.
 *             v1    - The vector to print.
 */
void vec_prn1(FILE *out, char *label, double *v1) {
    fprintf(out, "%s\n%8.3f\n", label, *v1);
}

/*
 * ivec_prn1:  Prints a label and the contents of a 1D integer vector to the 
 *             specified output file.
 *             
 * Parameters: out   - The output file.
 *             label - The label to print.
 *             v1    - The vector to print.
 */
void ivec_prn1(FILE *out, char *label, int *v1) {
    fprintf(out, "%s\n%6d\n", label, *v1);
}

/* 
 * vec_cross3: Computes the outer product of two input vectors.
 *
 * Parameters: v1 - The left input vector.
 *             v2 - The right input vector.
 *             v3 - The output vector.
 */
void vec_cross3(double *v1, double *v2, double *v3) {
    double vec[VEC_SIZE]; /* Cross product vector */

    /* Compute the cross product */
    vec[0] = ( *(v1 + 1) * *(v2 + 2) ) - ( *(v1 + 2) * *(v2 + 1) );
    vec[1] = ( *(v1 + 2) * *(v2)     ) - ( *(v1)     * *(v2 + 2) );
    vec[2] = ( *(v1)     * *(v2 + 1) ) - ( *(v1 + 1) * *(v2)     );

    /* Copy to output vector */
    vec_scale3(1.0, vec, v3);
}

/* 
 * vec_project3: Projects a vector onto a plane with the specified normal.
 *
 * Parameters:   n - The plane normal.
 *               v - The input vector.
 *               w - The projected vector.
 */
void vec_project3(double *n, double *v, double *w) {
    double proj[VEC_SIZE]; /* The projection vector */
    double dot = vec_dot3(n, v);

    /* Compute the projection of v onto the plane with normal n */
    vec_scale3(dot, n, proj);
    vec_diff3(proj, v, proj);
    vec_scale3(1.0, proj, w);
}

/*
 * vec_reflect3: Computes the direction of a reflected ray of light.
 *
 * Parameters:   unitin   - Unit vector in incoming direction.
 *               unitnorm - Outward surface normal.
 *               unitout  - Unit vector in direction of reflection.
 *
 * Return:       The distance the ray has traveled.
 */
void vec_reflect3(double *unitin, double *unitnorm, double *unitout) {
    double u[VEC_SIZE]; /* Opposite direction vector */
    double n[VEC_SIZE]; /* Normal vector             */
    double v[VEC_SIZE]; /* Reflection vector         */

    /* Compute the reflection vector */
    vec_scale3(-1.0, unitin, u);
    vec_scale3(2 * vec_dot3(u, unitnorm), unitnorm, n);
    vec_diff3(u, n, v);

    /* Copy to output vector */
    vec_unit3(v, unitout);
}

/* 
 * mat_id3:    Constructs a 3 x 3 identity matrix.
 *
 * Parameters: mtx - The output identity matrix.
 */
void mat_id3(double mtx[][VEC_SIZE]) {
    int  i; /* Row index    */
    int  j; /* Column index */

    /* Create the identity matrix */
    for (i = 0; i < VEC_SIZE; ++i) {
        for (j = 0; j < VEC_SIZE; ++j) {
            if (i != j) {
                mtx[i][j] = 0;
            } else {
                mtx[i][j] = 1;
            }
        }
    }
}

/* 
 * mat_mul3:   Multiplies two 3 x 3 input matrices.
 *
 * Parameters: x - The left input matrix.
 *             y - The right input matrix.
 *             z - The output matrix.
 */
void mat_mul3(double x[][VEC_SIZE], double y[][VEC_SIZE], double z[][VEC_SIZE]) {
    double mult[VEC_SIZE][VEC_SIZE]; /* Local copy to support aliasing */
    int    i;                        /* Row index                      */
    int    j;                        /* Column index                   */
    int    k;                        /* Counter                        */

    /* Perform multiplication */
    for (i = 0; i < VEC_SIZE; ++i) {
        for (j = 0; j < VEC_SIZE; ++j) {
            mult[i][j] = 0;

            for (k = 0; k < VEC_SIZE; ++k) {
                mult[i][j] += x[i][k] * y[k][j];
            }

        }
    }

    /* Save to output matrix */
    mat_copy3(mult, z);
}

/* 
 * mat_xpose3: Transposes a 3 x 3 input matrix.
 *
 * Parameters: x - The original matrix.
 *             z - The transposed matrix.
 */
void mat_xpose3(double x[][VEC_SIZE], double z[][VEC_SIZE]) {
    double xpose[VEC_SIZE][VEC_SIZE]; /* Local copy to support aliasing */
    int    i;                         /* Row index                      */
    int    j;                         /* Column index                   */

    /* Transpose the matrix */
    for (i = 0; i < VEC_SIZE; ++i) {
        for (j = 0; j < VEC_SIZE; ++j) {
            xpose[i][j] = x[j][i];
        }
    }

    /* Save to output matrix */
    mat_copy3(xpose, z);
}

/* 
 * mat_xform3: Linearly transposes a 3 x 3 input matrix by a 3 x 1 column 
 *             vector.
 *
 * Parameters: y - The transform matrix.
 *             x - The input column vector.
 *             z - The output vector.
 */
void mat_xform3(double y[][VEC_SIZE], double *x,double *z) {
    double xform[VEC_SIZE]; /* The transform vector */
    int    i;               /* Row index            */
    int    j;               /* Column index         */

    /* Compute the matrix transform */
    for (i = 0; i < VEC_SIZE; ++i) {
        xform[i] = 0;

        for (j = 0; j < VEC_SIZE; ++j) {
            xform[i] += y[i][j] * *(x + j);
        }
    }

    /* Save to output vector */
    vec_scale3(1.0, xform, z);
}

/* 
 * mat_copy3:  Copies the specified 3 x 3 matrix.
 *
 * Parameters: x - The matrix to copy.
 *             y - The output matrix.
 */
void mat_copy3(double x[][VEC_SIZE], double y[][VEC_SIZE]) {
    int  i; /* Row index */

    /* Copy the matrix information */
    for (i = 0; i < VEC_SIZE; ++i) {
        vec_scale3(1.0, &x[i][0], &y[i][0]);
    }
}

/* 
 * mat_prn3:   Prints the specified label and 3 x 3 matrix to the specified 
 *             file.
 *
 * Parameters: out   - The output file.
 *             label - The label to print.
 *             x     - The matrix to print.
 */
void mat_prn3(FILE *out, char *label, double x[][VEC_SIZE]) {
    int  i; /* Row index    */
    int  j; /* Column index */

    fprintf(out, "%s\n", label);

    /* Print the matrix information */
    for (i = 0; i < VEC_SIZE; ++i) {
        for (j = 0; j < VEC_SIZE; ++j) {
            fprintf(out, "%13.3f", x[i][j]);
        }
        fprintf(out, "\n");
    }
}

/*
 * msg_exit:   Prints an error message to the specified file and exits.
 *             
 * Parameters: out - The output file.
 *             msg - The error message.
 */
void msg_exit(FILE *out, char *msg) {
    fprintf(out, "%s\n", msg);
    exit(EXIT_FAILURE);
}
