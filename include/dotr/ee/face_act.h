#ifndef DOTR_EE_FACE_ACT_H
#define DOTR_EE_FACE_ACT_H

/* The face animation step ShFace_Act, which reads the face mode word and returns without any
 * effect in this build.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* gSh_FaceMode (0x351170, name from the Japanese build's data symbols), as small_misc.h
 * declares it; only the first word is read here. */
extern int gSh_FaceMode[];

/* 0x2464c0 ShFace_Act: reads the face mode and returns; it has no effect in this build.
 * MATCHING DEVICE: the body is a switch whose one case, 0, returns (`if (mode == 0) return;`
 * compiles the same; a switch whose case only breaks is removed entirely). */
void ShFace_Act(void);

#endif
