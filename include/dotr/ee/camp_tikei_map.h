#ifndef DOTR_EE_CAMP_TIKEI_MAP_H
#define DOTR_EE_CAMP_TIKEI_MAP_H

/* The terrain map panel of the campaign terrain window (0x236f90), called from
 * Taku_Camp_TIKEI_WINDOW; shares the camp_tikei item and window views.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */
#include "dotr/ee/camp_tikei.h"

/* The views, storage and callee signatures of 0x236f90 are declared in camp_tikei.h
 * (DotrTikeiCounts, the 7 x 7 maps at 0x2cee5c, the quad rows at 0x2986b0), except these,
 * which are declared inside the function (they link by address):
 * - 0x218ab0 Taku_G4_Set2 with halfword width and height (the float width is converted by
 *   the call itself; quad_offsets.h declares it with shorts too);
 * - the four halfwords of the fourth window record of camp_window_Map_data (0x297bae x,
 *   0x297bb0 y, 0x297bb2 w, 0x297bb4 h) each read by its own address.
 * MATCHING DEVICE: the four halfwords are separate symbols on the target; on the host they
 * name the fields of camp_window_Map_data[3].
 * Assumed: these types. */
extern DotrTikeiWord func_00218ab0_tikeimap(DotrTikeiWord cursor, void *quad, short dx, short dy, short width,
                                            short height);
#ifdef __MWERKS__
extern short data_00297bae_tikeimap;
extern short data_00297bb0_tikeimap;
extern short data_00297bb2_tikeimap;
extern short data_00297bb4_tikeimap;
#else
#define data_00297bae_tikeimap (camp_window_Map_data[3].x)
#define data_00297bb0_tikeimap (camp_window_Map_data[3].y)
#define data_00297bb2_tikeimap (camp_window_Map_data[3].w)
#define data_00297bb4_tikeimap (camp_window_Map_data[3].h)
#endif

#endif
