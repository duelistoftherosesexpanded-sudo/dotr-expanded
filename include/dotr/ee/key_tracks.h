#ifndef DOTR_EE_KEY_TRACKS_H
#define DOTR_EE_KEY_TRACKS_H

/* Adjacent leaf functions at 0x1fbe50 and 0x1fc050 that bisect an array of time-stamped
 * float records and blend the two bracketing records.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/camera.md. */

/* 16-byte record: an unsigned time word (converted as unsigned) and three floats. */
struct DotrKeyTriple {
    unsigned int t;
    float x, y, z;
};

/* 8-byte record: an unsigned time word and one float. */
struct DotrKeySingle {
    unsigned int t;
    float v;
};

/* Only three floats are stored through the first argument. */
struct DotrKeyOut {
    float x, y, z;
};

/* Argument types assumed from the loads, stores and $f12. */
void lSzView_CalcInterpCameraMotion(struct DotrKeyOut *outputPosition, struct DotrKeyTriple *keys, int keyframeCount,
                                    float currentTime);
float lSzView_CalcInterpCameraMotion1(struct DotrKeySingle *keys, int n, float t);

#endif
