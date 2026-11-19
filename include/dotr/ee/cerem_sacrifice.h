#ifndef DOTR_EE_CEREM_SACRIFICE_H
#define DOTR_EE_CEREM_SACRIFICE_H

/* Magic.c (Japanese build debug information) table function giving the three sacrifice codes of a
 * ceremony card; no callees and no globals.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Ta_GetCeremSacrifice (0x254c90, Magic.c): prototype from the Japanese build's debug
 * information; Buf is assumed to be three ints (only Buf[0..2] are written). */
void Ta_GetCeremSacrifice(int *Buf, int Obj);

#endif
