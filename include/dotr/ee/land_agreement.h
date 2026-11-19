#ifndef DOTR_EE_LAND_AGREEMENT_H
#define DOTR_EE_LAND_AGREEMENT_H

/* Classifies a monster unit's kind against a field's land type into one of three agreement results (1, 2
 * or 0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* struct jp_64b6d (the unit's variable record, member names from the Japanese build's debug
 * information) as perm_efc.h lays it out; this group reads bVarKind (+62) and wCardNo (+60).
 * ASSUMED: struct jp_65648 is 8 bytes as declared there. */
#include "dotr/ee/perm_efc.h"

/* Ta_GetToonAgreement (0x2492f0, small_misc): non-zero when the card agrees with the toon land
 * (assumed from this use). */
int Ta_GetToonAgreement(int cardNo);

/* 0x24df80 Do_CheckLandAgreement (original name); its caller CheckLandAgreement2 (0x24df00,
 * state_queries) passes SzDuel_GetUnit's record. Returns 0 when the top three bits of
 * bVarKind are not zero (assumed: not a monster); otherwise, with kind = bVarKind & 0x1f:
 * land 0: 1 for kinds 4, 5, 9, 17, 19, 2 for kind 7; land 1: 1 for 2, 10, 14, 18, 2 for 12, 13,
 * 16, 19; land 2: 1 for 0, 6, 8, 15, 2 for 2; land 3: 1 for 3, 4, 2 for 1; land 4: 1 for 12,
 * 13, 15, 16, 2 for 14, 17; land 5: 1 for 1, 2, 7, 2 for 8; land 6 (toon, assumed): 1 when
 * Ta_GetToonAgreement(wCardNo) is non-zero, else 2; lands 7 and 8: 0; land 9: 1 for kind 20;
 * every other kind and land (10 and above, negative): 0. The meaning of 1 and 2 is not
 * established here. */
int Do_CheckLandAgreement(int land, struct jp_64b6d *vInfo);

#endif
