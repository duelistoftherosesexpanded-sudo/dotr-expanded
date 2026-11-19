#include "dotr/ee/battle_draws.h"

/* lSzBattle_DrawNormal_Pipe: draws model arg1 once (default rotation), or 4 / 2 / 12 / 28 times
 * for kind arg2 1 / 3 / 5 / 6 with stepped rotations or positions offset from the corners at
 * 275 / 425; kind 0 also draws shadow 7 and one shadow per unit named by the two words at
 * 0x31ab88. The vectors are 128-bit unions copied whole (one lq/sq pair each). */
int lSzBattle_DrawNormal_Pipe(int arg0, int arg1, int arg2) {
    int i;
    int j;
    int *entry = gSzBattle_bl;
    union DotrBattleDrawQuad scale;

    scale = data_002922b0;
    switch (arg2) {
    case 1: {
        union DotrBattleDrawQuad rotation;
        rotation = data_002922c0;
        i = 0;
        do {
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
            rotation.i[1] += 0x4000;
            i++;
        } while (i < 4);
        break;
    }
    case 3: {
        union DotrBattleDrawQuad rotation;
        rotation = data_002922d0;
        i = 0;
        do {
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
            rotation.i[1] += 0x8000;
            i++;
        } while (i < 2);
        break;
    }
    case 5: {
        union DotrBattleDrawQuad rotation;
        union DotrBattleDrawQuad position;
        float signZ;
        float baseX;
        float baseZ;
        float signX;

        rotation = data_002922e0;
        SzModel_GetPos(arg1, &position);
        i = 0;
        do {
            if (i % 2 == 0) {
                signX = -1.0f;
            } else {
                signX = 1.0f;
            }
            if (i >> 1 == 0) {
                signZ = -1.0f;
            } else {
                signZ = 1.0f;
            }
            baseX = 275.0f * signX;
            position.f[0] = baseX * scale.f[0];
            baseZ = 275.0f * signZ;
            position.f[2] = baseZ * scale.f[2];
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrBattleDrawWord)&position, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
            position.f[0] = scale.f[0] * (baseX - 100.0f * signX);
            position.f[2] = baseZ * scale.f[2];
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrBattleDrawWord)&position, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
            position.f[0] = baseX * scale.f[0];
            position.f[2] = scale.f[2] * (baseZ - 100.0f * signZ);
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrBattleDrawWord)&position, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
            i++;
        } while (i < 4);
        break;
    }
    case 6: {
        union DotrBattleDrawQuad rotation;
        union DotrBattleDrawQuad position;
        float signX;
        float signZ;
        float baseX;
        float baseZ;
        float distance;

        rotation = data_002922f0;
        SzModel_GetPos(arg1, &position);
        i = 0;
        do {
            if (i % 2 == 0) {
                signX = -1.0f;
            } else {
                signX = 1.0f;
            }
            if (i >> 1 == 0) {
                signZ = -1.0f;
            } else {
                signZ = 1.0f;
            }
            baseX = 425.0f * signX;
            position.f[0] = baseX * scale.f[0];
            baseZ = 425.0f * signZ;
            position.f[2] = baseZ * scale.f[2];
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrBattleDrawWord)&position, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
            j = 0;
            do {
                distance = 100.0f * (float)(j + 1);
                position.f[0] = scale.f[0] * (baseX - distance * signX);
                position.f[2] = baseZ * scale.f[2];
                arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrBattleDrawWord)&position, 0, (DotrBattleDrawWord)&scale, 0, 0);
                position.f[0] = baseX * scale.f[0];
                position.f[2] = scale.f[2] * (baseZ - distance * signZ);
                arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrBattleDrawWord)&position, 0, (DotrBattleDrawWord)&scale, 0, 0);
                j++;
            } while (j < 3);
            i++;
        } while (i < 4);
        break;
    }
    default: {
        union DotrBattleDrawQuad rotation;
        rotation = data_00292300;
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (DotrBattleDrawWord)&rotation, (DotrBattleDrawWord)&scale, 0, 0);
        break;
    }
    }
    if (arg2 == 0) {
        arg0 = lSzBattle_DrawFakeShadow(arg0, 7, -1, -13.5f, 400.0f);
        i = 0;
        do {
            int value = *entry;
            if (value >= 0) {
                unsigned char *card = (unsigned char *)SzDuel_GetUnit(value >> 8, value & 0xFF);
                if (card != 0) {
                    arg0 = lSzBattle_DrawFakeShadow(arg0, card[0x46], -1, -13.5f, 400.0f);
                }
            }
            i++;
            entry++;
        } while (i < 2);
    }
    return arg0;
}
