#include "dotr/ee/eff_109.h"

/* Ta_Eff_109 (0x26c170): effect script 109. */
int Ta_Eff_109(unsigned int *src, void *arg1, int arg2) {
    int busy;
    struct DotrEff109Request data;
    unsigned char done = 0;
    struct DotrEff109Request *req = &data;
    struct DotrEff109State *state = arg1;
    struct DotrEff109Card *card;
    struct DotrEff109Vec4 color;
    struct DotrEff109Vec4 pos;
    struct DotrEff109Rot rot;
    int i;
    unsigned int *list;
    unsigned char *handle;

    data.command = 999;
    data.value = 0;
    data.cell = 0;
    data.flag = 0;
    SzDuel_SetWindowDraw(0);
    color.w = 0.0f;
    color.z = 0.0f;
    color.y = 0.0f;
    color.x = 0.0f;
    busy = 0;
    card = (struct DotrEff109Card *)SzDuel_GetUnit(src[1], 0);
    if (arg2 >= 0) {
        InitEffectTemp(state);
        SzSound_PlayBGM(0x2B, 0);
        TaEffect_LoadModule(state, 0x3D);
        TaEffect_LoadModule(state, 0x3E);
        TaEffect_LoadModule(state, 0x3F);
        TaEffect_LoadModule(state, 0x26C);
        if (TaTuto_IsTuto(SzDuel_GetTurnOwner()) == 0) {
            SzModel_Load(0x20, -1, 0x2E1, 1);
            SzModel_SetScale(0x20, &color);
        }
        TaUtil_DoOpen(src[1], src[2] & 7, 1);
        TaUtil_DoOpen(src[1], (src[2] >> 3) & 7, 1);
        TaUtil_DoOpen(src[1], (src[2] >> 6) & 7, 1);
        TaUtil_DoOpen(src[1], (src[2] >> 9) & 7, 1);
        state->fade = 0x80;
        for (i = 1; i < 11; i++) {
            if (SzDuel_GetUnit((src[1] == 0) ? 1 : 0, i)) {
                TaUtil_DoOpen((src[1] == 0) ? 1 : 0, i, 1);
            }
        }
        return 1;
    }
    switch (state->stage) {
    case 0:
        if (SzSound_IsLoadingBGM() == 0) {
            TaView_ReqFocusCamera(0, (src[1] << 8) | (src[2] & 7), 0x32, 0xB4);
            SzSound_PlayBGM(0x2B, 1);
            if (card->model == 0) {
                card->model = SzDuel_GetCardRequest();
            }
            if (card->model != 0) {
                *(struct DotrEff109Card **)(card->model + 0x90) = card;
                SzDuel_CalcUnitPos(card->x, card->y, 0, card->model);
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z = (src[1] == 0) ? 350.0f : -350.0f;
                rot.x = 0;
                rot.y = (src[1] == 0) ? 0x8000 : 0;
                rot.z = 0;
                SzModel_SetPos(0x20, &pos);
                SzModel_SetRot(0x20, &rot);
            }
            TaUtil_DoOpen(src[1], 0, 1);
            SzDuel_SetUnitCtlGoalC(src[1], 0, 0);
            SzDuel_SetUnitCtlRateC(src[1], 0, -1, 0);
            SzDuel_SetUnitOpenRate(src[1], 0, 1, 0x80);
            state->stage++;
        }
        break;
    case 1:
        if (state->timer++ > 24) {
            if (SzEffect_IsModelStandby2((src[1] << 8) | ((src[2] >> 3) & 7))) {
                TaView_ReqFocusCamera(0, (src[1] << 8) | ((src[2] >> 3) & 7), 0x32, 0xB4);
                state->timer = 0;
                state->stage++;
            }
        }
        break;
    case 2:
        if (state->timer++ > 24) {
            if (SzEffect_IsModelStandby2((src[1] << 8) | ((src[2] >> 6) & 7))) {
                TaView_ReqFocusCamera(0, (src[1] << 8) | ((src[2] >> 6) & 7), 0x32, 0xB4);
                state->timer = 0;
                state->stage++;
            }
        }
        break;
    case 3:
        if (state->timer++ > 24) {
            if (SzEffect_IsModelStandby2((src[1] << 8) | ((src[2] >> 9) & 7))) {
                TaView_ReqFocusCamera(0, (src[1] << 8) | ((src[2] >> 9) & 7), 0x32, 0xB4);
                state->timer = 0;
                state->stage++;
            }
        }
        break;
    case 4:
        if (state->timer++ > 24) {
            if (SzEffect_IsModelStandby2(src[1] << 8)) {
                TaView_ReqFocusCamera(0, src[1] << 8, 0xC8, 0xB4);
                SzGame_SetAutoBlur(6);
                state->timer = 0;
                state->stage++;
            }
        }
        break;
    case 5:
        if (state->timer++ == 0) {
            SzSound_PlaySE(0, 0x27);
            TaView_ReqFocusCamera(0xC, src[1] << 8, 0x32, 0xB4);
            return 1;
        }
        if (state->timer >= 12) {
            SzGame_SetAutoBlur(4);
        }
        if (state->timer >= 24 && TaEffect_IsLoadModule(state)) {
            SzGame_SetAutoBlur(0);
            TaView_ReqExzFocusCamera(0, src[1] << 8, 0xB4, 0xB4);
            state->phase++;
            state->timer = 0;
            state->stage++;
        }
        break;
    }
    switch (state->phase) {
    case 0:
        break;
    case 1:
        req->command = 0x26C;
        req->value = 0;
        req->cell = SzDuel_GetUnitLoc(src[1], (src[2] >> 9) & 7);
        req->flag = 0;
        state->handles[state->count] = Ta_Do_Effect(req, state);
        state->count++;
        state->timer = 0;
        state->phase++;
        break;
    case 2:
        if (state->timer++ > 24) {
            req->command = 0x26C;
            req->value = 0;
            req->cell = SzDuel_GetUnitLoc(src[1], (src[2] >> 6) & 7);
            req->flag = 0;
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->count++;
            state->timer = 0;
            state->phase++;
        }
        break;
    case 3:
        if (state->timer++ > 24) {
            req->command = 0x26C;
            req->value = 0;
            req->cell = SzDuel_GetUnitLoc(src[1], (src[2] >> 3) & 7);
            req->flag = 0;
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->count++;
            state->timer = 0;
            state->phase++;
        }
        break;
    case 4:
        if (state->timer++ > 24) {
            req->command = 0x26C;
            req->value = 0;
            req->cell = SzDuel_GetUnitLoc(src[1], src[2] & 7);
            req->flag = 0;
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->count++;
            state->timer = 0;
            state->phase++;
        }
        break;
    case 5:
        if (state->timer++ > 24) {
            Ta_UnitFade(src[1], 0, 1, 1, 2);
            req->command = 0x26C;
            req->value = 0;
            req->cell = SzDuel_GetUnitLoc(src[1], 0);
            req->flag = 0;
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->count++;
            state->timer = 0;
            state->phase++;
        }
        break;
    case 6:
        for (i = 0; i < state->count; i++) {
            if (SzEffect_IsUse(state->handles[i])) {
                return 1;
            }
        }
        if (TaTuto_IsTuto(SzDuel_GetTurnOwner()) == 1) {
            if (SzEffect_GetBlackOutForDuel() > 0) {
                SzEffect_SetBlackOutForDuel((unsigned char)(SzEffect_GetBlackOutForDuel() - 4));
            } else {
                done++;
            }
            if (SzEffect_GetBlackOutForMonster() > 0) {
                SzEffect_SetBlackOutForMonster((unsigned char)(SzEffect_GetBlackOutForMonster() - 4));
            } else {
                done++;
            }
            if (done == 2) {
                SzDuel_SetWindowDisp(0);
                SzDuel_CloseMsgPopup();
                return 0;
            }
            return 1;
        }
        state->count = 0;
        if (SzModel_IsModelStandby(0x20)) {
            list = (unsigned int *)SzModel_GetEffTextureB(0x20);
            SzText_FreeRequest(5, -1);
            for (i = 0; i < 8; i++) {
                SzText_AddFile(5, i, 0, *list);
                list++;
            }
            SzView_FreeRequest(0, 0);
            SzView_ReqCamera(0, 0, 8, 0, 0x20, (void *)SzModel_GetCamera(0x20, 0x13));
            req->command = 0x3D;
            req->value = src[1];
            req->cell = src[2];
            req->flag = src[3];
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->counters[state->count] = 0x3D;
            state->count++;
            req->command = 0x3E;
            req->value = src[1];
            req->cell = src[2];
            req->flag = src[3];
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->counters[state->count] = 0x3E;
            state->count++;
            req->command = 0x3F;
            req->value = src[1];
            req->cell = src[2];
            req->flag = src[3];
            state->handles[state->count] = Ta_Do_Effect(req, state);
            state->counters[state->count] = 0x3F;
            state->count++;
            state->phase++;
        }
        break;
    case 7:
        Ta109_BlackOutForExz(-1);
        if (state->timer++ > 100) {
            color.x = 1.0f;
            color.y = 1.0f;
            color.z = 1.0f;
            color.w = 1.0f;
            SzModel_SetScale(0x20, &color);
            state->phase++;
            state->timer = 0;
        }
        break;
    case 8:
        for (i = 0; i < state->count; i++) {
            handle = &state->handles[i];
            if (SzEffect_IsUse(*handle)) {
                if (SzEffect_GetStatus(*handle) == 2) {
                    Ta109_BlackOutForExz(1);
                    SzDuel_DelUnit(src[1], src[2] & 7);
                    SzDuel_DelUnit(src[1], (src[2] >> 3) & 7);
                    SzDuel_DelUnit(src[1], (src[2] >> 6) & 7);
                    SzDuel_DelUnit(src[1], (src[2] >> 9) & 7);
                    SzModel_Preset(0x20, -1, 999);
                    if (card->model) {
                        SzDuel_FreeCardRequest(card->model);
                    }
                    card->model = 0;
                }
                busy++;
            }
        }
        if (busy == 0) {
            SzText_FreeRequest(5, -1);
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    }
    return 1;
}
