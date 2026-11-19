#ifndef DOTR_EE_RH_INVERT_MATRIX_H
#define DOTR_EE_RH_INVERT_MATRIX_H

/* Matrix inversion by cofactors: the 4x4 inverse written only when the determinant is
 * non-zero.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* A 4x4 float matrix, row by row. */
struct DotrRhInvMatrix {
    float m[4][4];
};

/* 0x20bde0 RhGeom_InvertMatrix (name from the Japanese build's symbols): computes the
 * determinant of src by cofactor expansion along the first row; when it is not 0.0 writes
 * the inverse (adjugate / determinant, each cofactor multiplied by 1.0 or -1.0 in double
 * precision and converted back to float) to dst, else leaves dst unchanged. The double
 * arithmetic is done by the compiler's runtime routines fptodp (0x1a2370), dpmul (0x1a2918)
 * and dptofp (0x1a3038) in the library block. */
void RhGeom_InvertMatrix(void *dst, void *src);

#endif
