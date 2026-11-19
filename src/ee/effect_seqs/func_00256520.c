#include "dotr/ee/effect_seqs.h"

#define E002_MIN(a, b) ((a) < (b) ? (a) : (b))
#define E002_MAX(a, b) ((b) < (a) ? (a) : (b))
/* City-block distance of cell (x, y) from the cell packed in dwFrom, against the reach in dwTo. */
#define E002_NEAR(x, y) \
    ((E002_MAX(y, (eInfo->dwFrom & 0xFF)) - E002_MIN(y, (eInfo->dwFrom & 0xFF))) \
     + (E002_MAX(x, (eInfo->dwFrom >> 8)) - E002_MIN(x, (eInfo->dwFrom >> 8))) <= eInfo->dwTo)

int Ta_Eff_002(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    int Buf[49];
    int tCount;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int j;
    int i;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    for (j = 0; j < 49; j++) {
        Buf[j] = -1;
    }
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Ta_AllUnitFade2(eInfo, Temp, ID, -1, 1, 3);
        for (j = 0; j < 7; j++) {
            for (i = 0; i < 7; i++) {
                if (E002_NEAR((unsigned)i, (unsigned)j)) {
                    Buf[tCount] = (i << 8) | j;
                    tCount++;
                }
            }
        }
        if (tCount != 1) {
            TaView_ReqStdCompleteView(30);
        } else {
            TaView_ReqFocusCamera2(0, Buf[0], 180);
        }
        if (eInfo->dwPar == 8) {
            TaEffect_LoadModule(Temp, 503);
        } else {
            TaEffect_LoadModule(Temp, 502);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Ta_IsFinishFade() == 0 || TaEffect_IsLoadModule(Temp) != 1) {
            return 1;
        }
        for (i = 0; i < 49; i++) {
            Buf[i] = -1;
        }
        switch (eInfo->dwTo) {
        case 16:
            for (i = 0; i < 7; i++) {
                Buf[i] = ((eInfo->dwFrom >> 8) << 8) | i;
                tCount++;
            }
            break;
        default:
            for (j = 0; j < 7; j++) {
                for (i = 0; i < 7; i++) {
                    if (E002_NEAR((unsigned)i, (unsigned)j)) {
                        Buf[tCount] = (i << 8) | j;
                        tCount++;
                    }
                }
            }
            break;
        }
        for (i = 0; i < tCount; i++) {
            switch (eInfo->dwPar) {
            case 8:
                if (i == 0) {
                    pCall->eNO = 503;
                    pCall->dwFrom = 1;
                    pCall->dwTo = Buf[i];
                    pCall->dwPar = eInfo->dwPar;
                } else {
                    pCall->eNO = 503;
                    pCall->dwFrom = 0;
                    pCall->dwTo = Buf[i];
                    pCall->dwPar = eInfo->dwPar;
                }
                break;
            default:
                if (i == 0) {
                    if (SzDuel_GetField(Buf[i] & 0xFF, Buf[i] >> 8) != 8) {
                        pCall->eNO = 502;
                        pCall->dwFrom = 1;
                        pCall->dwTo = Buf[i];
                        pCall->dwPar = eInfo->dwPar;
                    }
                } else {
                    if (SzDuel_GetField(Buf[i] & 0xFF, Buf[i] >> 8) != 8) {
                        pCall->eNO = 502;
                        pCall->dwFrom = 0;
                        pCall->dwTo = Buf[i];
                        pCall->dwPar = eInfo->dwPar;
                    }
                }
                break;
            }
            /* The original tests the stored byte, zero-extended, for >= 0
             * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
            if ((stored = (Temp->eID[i] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                Temp->ePos[Temp->oNum] = ((Buf[i] >> 8) << 8) | (Buf[i] & 0xFF);
                Temp->oNum++;
            }
        }
        Temp->wLvl++;
        return 1;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) != 0) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if ((Temp->ePos[i] & 0xFFFF) != 0xFFFF) {
                        Temp->ePos[i] = 0xFFFF;
                    }
                }
            } else {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            Temp->wLvl++;
            TaView_RestoreOriginalView(Temp, 30);
            Ta_AllUnitFade2(eInfo, Temp, ID, 1, 1, 3);
        }
        return 1;
    default:
        if (Temp->wCamTime++ > 30) {
            if (Ta_IsFinishFade() == 1) {
                SzDuel_CloseMsgPopup();
                return 0;
            }
        }
        break;
    }
    return 1;
}
