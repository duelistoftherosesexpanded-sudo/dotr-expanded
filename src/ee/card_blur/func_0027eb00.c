#include "dotr/ee/card_blur.h"

/* One step of the card-transit effect that owns the work block pobj. Answers 8 when neither
 * the source (eInfo->dwFrom) nor the target (eInfo->dwTo) has a card request. With nID >= 0
 * it first sets the block up: clears it, copies the card requests, marks the units busy,
 * stores nID as the kind and prepares the screen points, look vectors and the four blur
 * coordinates from the target card. Every call then draws the cards, for kind 1 moves the
 * target card towards the source while the counter runs down (27 again at 0, setting flag
 * bit 31 when Ta_LkCheckFlag reports it clear), shifts the blur coordinates by one and,
 * while Ta_LkCheckFlag reports bit 31, counts calls: 2 on the fourth, 999 stored after it.
 * Name, prototype, parameter and local names and the structure layouts are from the Japanese
 * build's debug information (Tasaki_Eff.c); the locals are declared in the reverse of that
 * order. The one-bit flags are signed bit-fields, so a set flag is stored as -1 here (the
 * same bytes as storing 1 with the pinned compiler, and defined on the host). */
int TaLib_c615(struct jp_aa4a2 *pobj, struct jp_ab71b *eInfo, int nID)
{
    int nFrom;
    int nTarget;
    struct jp_aa2f3 *pCardReqF;
    struct jp_aa2f3 *pCardReqT;
    int i;
    int ret;

    ExGetMyOT();
    nFrom = eInfo->dwFrom;
    nTarget = eInfo->dwTo;
    pCardReqF = SzEffect_GetCardRequest2(nFrom);
    pCardReqT = SzEffect_GetCardRequest2(nTarget);
    if (pCardReqF == 0 && pCardReqT == 0) {
        return 8;
    }
    if (nID >= 0) {
        func_00195108(pobj, 0x430);
        Ta_LkInitCoordinate(pobj);
        if (pCardReqF != 0) {
            pobj->CardReqF = *pCardReqF;
        }
        if (pCardReqT != 0) {
            pobj->CardReqT = *pCardReqT;
        }
        if (pCardReqF != 0) {
            SzEffect_SetBusyFlag2(nFrom, 1);
            pobj->CardReqF.Draw.fScale = -1;
            pobj->CardReqF.Draw.fLook = -1;
        }
        if (pCardReqT != 0) {
            SzEffect_SetBusyFlag2(nTarget, 1);
            pobj->CardReqT.Draw.fScale = -1;
            pobj->CardReqT.Draw.fLook = -1;
        }
        pobj->effflags = 0;
        pobj->kind = nID;
        pobj->cardmove = 14;
        pobj->count = 0;
        if (pCardReqF != 0) {
            pCardReqF->Draw.icolRgba.a = 0;
        }
        if (pCardReqT != 0) {
            pCardReqT->Draw.icolRgba.a = 0;
        }
        pobj->fadeT = 128;
        pobj->fadeF = 128;
        pobj->corF.pntScr.x = -125.0f;
        pobj->corF.pntScr.y = -8.0f;
        pobj->corT.pntScr.x = 125.0f;
        pobj->corT.pntScr.y = -8.0f;
        if (pCardReqF != 0) {
            pobj->effflags |= 1;
            pobj->corF.nCount = 1;
            Ex3d_GetPointOnScreen(&pobj->CardReqF.Draw.pntPos, pobj->corF.pntScr.x, pobj->corF.pntScr.y);
            pobj->corF.pntPos = pCardReqF->Draw.pntPos;
            Ex3d_GetLookVector(&pobj->CardReqF.Draw.ivecLook);
            pobj->corF.ivecLook.z = 0;
            pobj->corF.ivecLook.y = 0;
            pobj->corF.ivecLook.x = 0;
            pobj->CardReqF.Draw.ivecRot.x = 0;
            pobj->CardReqF.Draw.ivecRot.y = 0x10000;
            pobj->CardReqF.Draw.ivecRot.z = 0x10000;
            pobj->corF.ivecRot = pCardReqF->Draw.ivecRot;
            pobj->corF.vecScale.y = 0.1f;
            pobj->corF.vecScale.x = 0.1f;
            pobj->CardReqF.Draw.vecScale.x = 0.46875f;
            pobj->CardReqF.Draw.vecScale.y = 0.48913f;
        }
        if (pCardReqT != 0) {
            pobj->effflags |= 2;
            pobj->corT.nCount = 1;
            Ex3d_GetPointOnScreen(&pobj->CardReqT.Draw.pntPos, pobj->corT.pntScr.x, pobj->corT.pntScr.y);
            pobj->corT.pntPos = pCardReqT->Draw.pntPos;
            Ex3d_GetLookVector(&pobj->CardReqT.Draw.ivecLook);
            pobj->corT.ivecLook.z = 0;
            pobj->corT.ivecLook.y = 0;
            pobj->corT.ivecLook.x = 0;
            pobj->CardReqT.Draw.ivecRot.x = 0;
            pobj->CardReqT.Draw.ivecRot.y = 0x10000;
            pobj->CardReqT.Draw.ivecRot.z = 0x10000;
            pobj->corT.ivecRot = pCardReqT->Draw.ivecRot;
            pobj->corT.vecScale.y = 0.1f;
            pobj->corT.vecScale.x = 0.1f;
            pobj->CardReqT.Draw.vecScale.x = 0.46875f;
            pobj->CardReqT.Draw.vecScale.y = 0.48913f;
            for (i = 0; i < 4; i++) {
                pobj->blur[i].trans = pobj->CardReqT.Draw.pntPos;
                pobj->blur[i].rot = pobj->CardReqT.Draw.ivecRot;
                pobj->blur[i].scale = pobj->CardReqT.Draw.vecScale;
            }
        }
    }
    ret = 0;
    Ta_LkSetCoordinate(pobj);
    SzDuel_StartCardSession();
    if (pobj->kind == 0 && pCardReqF != 0) {
        Ta_LkPrim_DrawCard(&pobj->CardReqF, pobj->fadeF, 0);
    }
    if (pCardReqT != 0) {
        Ta_LkPrim_DrawCard(&pobj->CardReqT, pobj->fadeT, 0);
    }
    if (pobj->kind == 1) {
        Ex3d_GetPointOnScreen(&pobj->CardReqF.Draw.pntPos, pobj->corF.pntScr.x, pobj->corF.pntScr.y);
        if (pobj->cardmove < 7) {
            Ta_LkOBJ_TransitVector(&pobj->CardReqT, &pobj->CardReqF, pobj->cardmove);
        }
        if (pobj->cardmove != 0) {
            pobj->cardmove--;
        } else {
            if (Ta_LkCheckFlag(pobj->effflags, 31) == 0) {
                pobj->effflags |= 0x80000000;
            }
            pobj->cardmove = 27;
        }
    }
    SzDuel_FinishCardSession();
    for (i = 3; i >= 0; i--) {
        if (i != 0) {
            pobj->blur[i].trans = pobj->blur[i - 1].trans;
            pobj->blur[i].rot = pobj->blur[i - 1].rot;
            pobj->blur[i].scale = pobj->blur[i - 1].scale;
        } else {
            pobj->blur[i].trans = pobj->CardReqT.Draw.pntPos;
            pobj->blur[i].rot = pobj->CardReqT.Draw.ivecRot;
            pobj->blur[i].scale = pobj->CardReqT.Draw.vecScale;
        }
    }
    if (Ta_LkCheckFlag(pobj->effflags, 31) != 0) {
        pobj->count++;
        if (pobj->count == 4) {
            ret = 2;
        } else if (pobj->count > 4) {
            pobj->count = 999;
        }
    }
    return ret;
}
