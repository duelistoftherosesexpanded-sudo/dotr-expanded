#ifndef DOTR_EE_HALFWORD_LISTS_H
#define DOTR_EE_HALFWORD_LISTS_H

/* 49-entry halfword lists whose empty entries hold 0xFFFF (fill, then count the entries in use).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* TaLogic018_GetZoneSide (low_leaves), called by 0x15a120; three arguments forwarded, result unused. */
extern void TaLogic018_GetZoneSide(int a, int b, unsigned short *out);

/* Fills out[0..48] with 0xFFFF, calls TaLogic018_GetZoneSide(a, b, out) and then TaLogic018_GetZoneSize(out),
 * whose result is discarded. int parameters assumed. */
void TaLogic018_GetOutVirusZone(int a, int b, unsigned short *out);
/* Number of entries in p[0..48] that are not 0xFFFF, as a byte. */
int TaLogic018_GetZoneSize(unsigned short *p);

#endif
