#include "dotr/ee/unit_request.h"

void lSzDuel_DoUnitRequest(struct DotrStateCell *cell, DotrStateWord unknown)
{
    struct DotrUnitRequestObject *o;
    struct DotrStateVector v;
    int mode;
    struct DotrStateItem *u = (struct DotrStateItem *)unknown;
    int n, flag;
    signed char d;
    unsigned char level;
    float f;
    mode = SzGame_GetMode(1);
    if (cell == 0) {
        return;
    }
    if (u == 0) {
        return;
    }
    if (!u->status.unknown_bit_7) {
        if (!u->modelStatusFlags.unknown_bit_1) {
            if (u->status.unknown_bit_0) {
                n = u->unknown_4c[1] - 4;
                n = n > 64 ? n : 64;
                u->unknown_4c[1] = n;
            } else {
                n = u->unknown_4c[1] + 4;
                n = n < 128 ? n : 128;
                u->unknown_4c[1] = n;
            }
        }
        if (u->unknown_56.unknown_bits_4_5) {
            d = u->unknown_56.unknown_bits_4_5;
            if ((d > 0 && u->openRate >= 255) || (d < 0 && u->openRate <= 0)) {
                u->unknown_56.unknown_bits_4_5 = 0;
            } else if (d > 0) {
                n = u->openRate + 32;
                n = n < 255 ? n : 255;
                u->openRate = n;
            } else {
                n = u->openRate - 32;
                n = n > 0 ? n : 0;
                u->openRate = n;
            }
        }
        if (u->unknown_56.unknown_bits_6_7) {
            d = u->unknown_56.unknown_bits_6_7;
            if ((d > 0 && u->unknown_55 >= 255) || (d < 0 && u->unknown_55 <= 0)) {
                u->unknown_56.unknown_bits_6_7 = 0;
            } else if (d > 0) {
                n = u->unknown_55 + 32;
                n = n < 255 ? n : 255;
                u->unknown_55 = n;
            } else {
                n = u->unknown_55 - 32;
                n = n > 0 ? n : 0;
                u->unknown_55 = n;
            }
        }
        n = u->unknown_56.unknown_bits_0_1;
        if ((n > 0 && u->ctlRateC >= u->ctlGoalC) || (n < 0 && u->ctlRateC <= 0)) {
            if (mode == 1) {
                u->ctlModeC = 1;
            } else {
                u->ctlModeC = 0;
            }
            u->unknown_56.unknown_bits_0_1 = 0;
        } else if (n > 0) {
            int t = u->ctlRateC + 8;
            t = t < u->ctlGoalC ? t : u->ctlGoalC;
            u->ctlRateC = t;
        } else if (n < 0) {
            int t = u->ctlRateC - 8;
            t = t > 0 ? t : 0;
            u->ctlRateC = t;
        }
        o = (struct DotrUnitRequestObject *)u->cardModel;
        if (o != 0) {
            if (u->modelStatusFlags.unknown_bit_1) {
                level = 128;
            } else {
                level = cell->mapAlpha;
            }
            o->unknown_14 = -((u->unknown_55 << 14) / 255);
            if (u->team == 0) {
                o->unknown_14 = o->unknown_14 + 0x8000;
            }
            o->unknown_18 = (u->openRate << 15) / 255;
            n = (u->unknown_4c[1] * level) >> 7;
            o->unknown_40.unknown_0 = n;
            o->unknown_40.unknown_1 = n;
            o->unknown_40.unknown_2 = n;
            if (u->ctlModeC & 1) {
                o->unknown_40.unknown_3 = u->ctlRateC;
            }
            if (u->ctlModeC & 2) {
                f = (float)(unsigned)u->ctlRateC / 1280.0f;
                o->unknown_30.z = f;
                o->unknown_30.y = f;
                o->unknown_30.x = f;
            }
        }
    }
    if (u->slot >= 6) {
        return;
    }
    if (u->modelStatusFlags.unknown_bit_0) {
        return;
    }
    flag = 0;
    if (SzModel_IsModelStandby(u->modelId)) {
        if (u->slot == 0) {
            flag = 1;
        } else if (u->status.unknown_bit_4) {
            flag = 1;
        }
    }
    if (flag) {
        n = u->unknown_56.unknown_bits_2_3;
        if ((n > 0 && u->ctlRateM >= u->ctlGoalM) || (n < 0 && u->ctlRateM <= 0)) {
            if (mode == 1) {
                u->unknown_4f = 1;
            } else {
                u->unknown_4f = 0;
            }
            u->unknown_56.unknown_bits_2_3 = 0;
        } else if (n > 0) {
            int t = u->ctlRateM + 8;
            t = t < u->ctlGoalM ? t : u->ctlGoalM;
            u->ctlRateM = t;
        } else if (n < 0) {
            int t = u->ctlRateM - 8;
            t = t > 0 ? t : 0;
            u->ctlRateM = t;
        }
    }
    if (u->unknown_4f & 1) {
        SzModel_SetAlpha(u->modelId, u->ctlRateM);
    }
    if (u->unknown_4f & 2) {
        f = (float)(unsigned)u->ctlRateM / 128.0f;
        f = f < 1.0f ? f : 1.0f;
        v.z = f;
        v.y = f;
        v.x = f;
        SzModel_SetScale(u->modelId, &v);
    }
}
