#include "dotr/ee/small_misc.h"

/* Draws the parts of one face entry and steps its animated parts. Three random numbers are
 * drawn first, also for a null entry: two six-bit values summed (the extra wait of a kind 0
 * part that has returned to frame 0) and one three-bit value (the extra wait of a kind 1 part).
 * A fixed part is drawn from its own rectangle row and position. An animated part (flag bit 7)
 * counts its timer down while its kind's bit is set in the entry mask, advances its frame
 * when the timer is 0 and wraps at the part's frame count; with the bit clear the frame is
 * reset. A part with frames is drawn from ShAnime_GetAnmDat and, when its timer is 0, the
 * timer is reloaded from that frame's delay. Entry bytes, indexed from +4: [0] set, [1] row,
 * [2] mask, [7 + n] timer and [23 + n] frame of the n-th animated part. */
void ShFace_DrawOne(struct DotrMiscEntry52 *entry)
{
    struct DotrMiscByteRow info;
    struct DotrMiscFaceFix fix;
    struct DotrMiscFaceAnm anm;
    struct DotrMiscHalfRow rect;
    struct DotrMiscFaceFixAnm *anim;
    int n;
    int i;
    int wait0;
    int wait1;

    wait0 = ((func_0019a5c8() >> 4) & 0x3f) + ((func_0019a5c8() >> 4) & 0x3f);
    wait1 = (func_0019a5c8() >> 4) & 7;
    if (entry != 0) {
        ShAnime_GetPatInfo(entry->unknown_04[0], entry->unknown_04[1], &info);
        n = 0;
        for (i = 0; i < (unsigned char)info.unknown_values[0]; i++) {
            ShAnime_GetFixInfo(entry->unknown_04[0], entry->unknown_04[1], i, &fix);
            if (fix.flags & 0x80) {
                anim = (struct DotrMiscFaceFixAnm *)&fix;
                if (entry->unknown_04[2] & (1 << anim->kind)) {
                    if (entry->unknown_04[7 + n] != 0) {
                        entry->unknown_04[7 + n]--;
                    }
                    if (entry->unknown_04[7 + n] == 0) {
                        entry->unknown_04[23 + n]++;
                    }
                    if (entry->unknown_04[23 + n] >= anim->count) {
                        entry->unknown_04[23 + n] = 0;
                    }
                } else {
                    entry->unknown_04[23 + n] = 0;
                }
                if (anim->count != 0) {
                    ShAnime_GetAnmDat(entry->unknown_04[0], entry->unknown_04[1], i, entry->unknown_04[23 + n], &anm);
                    ShAnime_GetPartsRect(entry->unknown_04[0], anm.row, &rect);
                    ShFace_Prim(entry, &rect, anm.x, anm.y);
                    if (entry->unknown_04[7 + n] == 0) {
                        switch (anim->kind) {
                        case 0:
                            if (entry->unknown_04[23 + n] == 0) {
                                entry->unknown_04[7 + n] = anm.delay + wait0;
                            } else {
                                entry->unknown_04[7 + n] = anm.delay;
                            }
                            break;
                        case 1:
                            entry->unknown_04[7 + n] = anm.delay + wait1;
                            break;
                        default:
                            entry->unknown_04[7 + n] = anm.delay;
                            break;
                        }
                    }
                }
                n++;
            } else {
                ShAnime_GetPartsRect(entry->unknown_04[0], fix.row, &rect);
                ShFace_Prim(entry, &rect, fix.x, fix.y);
            }
        }
    }
}
