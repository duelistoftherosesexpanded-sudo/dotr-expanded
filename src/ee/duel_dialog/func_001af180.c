#include "dotr/ee/duel_dialog.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* SzDuel_DrawHandCard_Pipe (0x1af180): in mode 1 of SzGame_GetMode, when flag bit 2 of the
 * control byte +0xd67 is set or one of the bytes +0xd45, +0xd47, +0xd48 is non-zero, walks the
 * six card slots +0xd2c..+0xd31 from the last: for each slot whose card control entry has a
 * node without flag bit 6 of +0x83 it sets the highlight colour (row +0xd3c of
 * gSzDuel_aaicolRgba with alpha from 0x318118 for the selected card +0xd4f, else alpha 0),
 * the card depth (interpolated from Ex3d_GetSampleZ towards 0xfffffa while flag bit 6 of +0x9f
 * is set, else 0xfffffa), positions it on screen or in the world, and draws it between two
 * Z-buffer mode changes. Returns the advanced cursor.
 * The node pointer passed to SzDuel_DrawCard_Pipe is cast to the cursor word type, NULL is written
 * 0, and the stores of 1 into two signed one-bit fields are written -1 (the same bit, without a
 * host overflow warning). */
int SzDuel_DrawHandCard_Pipe(int arg0, DotrDialogHandCardNode *object)
{
    extern DotrDialogHandPalette gSzDuel_aaicolRgba_dialog[];
    unsigned char *state = data_00317490_dialog.b;
    int i;
    int id;
    DotrDialogHandActor *actor;
    int resource;

    if (SzGame_GetMode(1) != 1) {
        return arg0;
    }
    if (!((DotrDialogHandFlags *)(state + 0xD67))->bit2) {
        if (state[0xD45] == 0 && state[0xD47] == 0 && state[0xD48] == 0) {
            return arg0;
        }
    }
    i = 5;
    do {
        id = state[0xD2C + i];
        actor = SzDuel_GetCardControl(id);
        if (actor != 0) {
            object = actor->object;
            if (object != 0) {
                resource = object->resource;
                if (!object->flags83.bit6) {
                    if (id != 0) {
                        if (state[0xD47] == 0 && state[0xD4F] == id - 1) {
                            object->color = gSzDuel_aaicolRgba_dialog[state[0xD3C]].color;
                            object->color.a = data_00318118_dialog >> 8;
                        } else {
                            object->color.a = 0;
                        }
                    }
                    if (actor->flags9F.bit6) {
                        int phase;
                        float difference = actor->scale - 0.1f;
                        unsigned int color;
                        float progress;
                        extern unsigned int Ex3d_GetSampleZ(float, void *);
                        difference = difference >= 0.0f ? difference : -difference;
                        phase = difference <= 0.000001f ? 0 : 1;
                        color = Ex3d_GetSampleZ(30.0f, actor->tracks[phase]);
                        progress = (float)(unsigned int)actor->timer / (float)(unsigned int)actor->duration;
                        if (phase != 0) {
                            progress = 1.0f - progress;
                        }
                        SzDuel_SetCardDefPrioZ((unsigned int)((float)color * (1.0f - progress) + 16777210.0f * progress));
                    } else {
                        SzDuel_SetCardDefPrioZ(0xFFFFFA);
                    }
                    if (actor->flags9F.bit2) {
                        SzDuel_GetCardNodeOnScreen(object, 1, object->x, object->y);
                        arg0 = SzDuel_SetupCard2_Pipe(arg0, resource, object);
                    } else {
                        int result = SzDuel_GetCardNode(object, 1);
                        resource = result ? resource : 0;
                        arg0 = SzDuel_SetupCard2_Pipe(arg0, resource, object);
                        if (0 != id) {
                            if (state[0xD47] != 0) {
                                if (((DotrDialogHandFlags *)(state + 0xD67))->bit4) {
                                    if (object->flags81.bit4 && result == 0) {
                                        object->flags81.bit4 = 0;
                                        object->flags81.bit6 = 0;
                                    }
                                } else {
                                    if (!object->flags81.bit4 && result == 0) {
                                        object->flags81.bit4 = -1;
                                        object->flags81.bit6 = -1;
                                    }
                                }
                            }
                        }
                    }
                    arg0 = RhBase_SetZBufferMode(SzDuel_DrawCard_Pipe(RhBase_SetZBufferMode(arg0, 1), (DotrDialogWord)object), 0);
                }
            }
        }
        i--;
    } while (i >= 0);
    return arg0;
}
