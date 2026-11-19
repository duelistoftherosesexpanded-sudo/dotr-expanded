#include "dotr/ee/tuto_l033.h"

/* L033 (0x1747d0): one tutorial script. Each call first mirrors the message window's display
 * flag into its draw flag and sets the analogue info of port 0 axes 0..3 to 0x80; pad button
 * 0x800 returns -2. Then the step word (+0x1a2, 0..0x3f through a jump table) runs one step:
 * fades (TaTuto_ControlFade), messages 0xa48..0xa6f (TaTuto_OpenTutoMsg), unit and field
 * set-up, waits of 0x1e / 0x3c frames, and the button the player is to press next.
 * Two of its callees are declared in the header with int-parameter prototypes. */
int L033(int arg0, void *tutorial)
{
    struct DotrTutoL033Record *arg1 = tutorial;
    (void)arg0;
    if (SzDuel_GetWindowDisp() != 0) {
        SzDuel_SetWindowDraw(1);
    } else {
        SzDuel_SetWindowDraw(0);
    }
    SzPad_SetAnalogInfo(0, 0, 0x80);
    SzPad_SetAnalogInfo(0, 1, 0x80);
    SzPad_SetAnalogInfo(0, 2, 0x80);
    SzPad_SetAnalogInfo(0, 3, 0x80);
    if (SzPad_Check(0, 0, 0x800) != 0) {
        return -2;
    }
    switch (arg1->step) {
    case 0x0:
        SzDuel_SetIgnoreJudge(1);
        SzDuel_SetWindowDisp(1);
        arg1->step = 2;
        break;
    case 0x1:
        if (SzEffect_IsModelStandby2(0) != 0) {
            if (SzEffect_IsModelStandby2(0x100) != 0) {
                arg1->step++;
            }
        }
        break;
    case 0x2:
        if (TaTuto_OpenTutoMsg(0xA48, 0xA4A, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0x3:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1->text);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0x3E8);
            SzDuel_AddUnit(0, 3, 1, 0x9C, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x4:
        SzDuel_SetPlayerCur(0, 3, 1);
        if (SzEffect_IsModelStandby2(1) != 0) {
            if (arg1->wait++ > 0x1E) {
                arg1->wait = 0;
                arg1->step++;
            }
        }
        break;
    case 0x5:
        TaTuto_FocusUnit__001745f0(0x9C);
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            SzDuel_SetWindowDisp(1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x6:
        if (TaTuto_OpenTutoMsg(0xA4B, 0xA4B, arg1) == 1) {
            arg1->wait = 0;
            arg1->timer = 0;
        }
        break;
    case 0x7:
        TaTuto_DrawFinger(arg1, 0xA, 0x800, 0x12C, -0x9C);
        if (arg1->timer++ > 0x3C) {
            SzDuel_CloseHandCursor();
            arg1->wait = 0;
            arg1->timer = 0;
            arg1->step++;
        }
        break;
    case 0x8:
        TaTuto_OpenTutoMsg(0xA4C, 0xA4C, arg1);
        break;
    case 0x9:
    case 0xA:
    case 0xB:
    case 0x11:
    case 0x12:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1F:
        if (arg1->wait++ > 0xC) {
            arg1->wait = 0;
            switch (arg1->step++) {
            case 9:
            case 11:
            case 23:
            case 25:
                return 0x40;
            case 10:
            case 17:
            case 24:
                return 0x1000;
            case 18:
                return 0x10;
            case 22:
                return 0x4000;
            case 31:
                return 0x80;
            }
        }
        break;
    case 0xC:
        if (TaTuto_OpenTutoMsg(0xA4D, 0xA4E, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0xD:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1->text);
            SzDuel_DelUnit(0, 1);
            SzDuel_CalcLP(0, 4, 0x1F4);
            SzDuel_CalcLP(1, 4, 0x1F4);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 0x15A, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0xE:
        SzDuel_SetPlayerCur(0, 3, 4);
        if (SzEffect_IsModelStandby2(1) != 0 && SzEffect_IsModelStandby2(0x101) != 0) {
            if (arg1->wait++ > 0x1E) {
                arg1->wait = 0;
                arg1->step++;
            }
        }
        break;
    case 0xF:
        TaTuto_FocusUnit__001745f0(0x9C);
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            SzDuel_SetWindowDisp(1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x10:
        TaTuto_OpenTutoMsg(0xA4F, 0xA51, arg1);
        break;
    case 0x14:
        TaTuto_OpenTutoMsg(0xA52, 0xA54, arg1);
        break;
    case 0x13:
    case 0x15:
        if (arg1->wait++ > 0xC) {
            arg1->wait = 0;
            switch (arg1->step++) {
            case 19:
                return 0x4000;
            case 21:
                return 0x20;
            }
        }
        break;
    case 0x1A:
        if (TaTuto_OpenTutoMsg(0xA55, 0xA57, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0x1B:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1->text);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_MoveUnit(0, 0, 3, 4);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xFA0);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x1C:
        SzDuel_SetPlayerCur(0, 3, 4);
        if (arg1->wait++ > 0x1E) {
            arg1->wait = 0;
            arg1->step++;
        }
        break;
    case 0x1D:
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            SzDuel_SetWindowDisp(1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x1E:
        TaTuto_OpenTutoMsg(0xA58, 0xA58, arg1);
        break;
    case 0x20:
        TaTuto_OpenTutoMsg(0xA59, 0xA59, arg1);
        break;
    case 0x21:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1->text);
            SzDuel_AddUnit(1, 3, 3, 3, 0);
            SzDuel_AddUnit(1, 2, 3, 0x11D, 0);
            SzDuel_AddUnit(1, 4, 3, 0x20A, 0);
            SzDuel_AddUnit(1, 2, 4, 0x277, 0);
            SzDuel_AddUnit(1, 4, 4, 0x15, 0);
            SzDuel_AddUnit(1, 2, 5, 0x2AB, 0);
            SzDuel_AddUnit(1, 3, 5, 0x2AB, 0);
            SzDuel_AddUnit(1, 4, 5, 0x2AB, 0);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 2, 4, 1);
            SzDuel_SetUnitStatus(1, 3, 4, 1);
            SzDuel_SetUnitStatus(1, 4, 4, 1);
            SzDuel_SetUnitStatus(1, 5, 4, 1);
            arg1->step++;
            arg1->wait = 0;
            return 0x20;
        }
        break;
    case 0x22:
        if (SzEffect_IsModelStandby2(0x101) != 0 && SzEffect_IsModelStandby2(0x102) != 0 && SzEffect_IsModelStandby2(0x103) != 0 && SzEffect_IsModelStandby2(0x104) != 0 && SzEffect_IsModelStandby2(0x105) != 0) {
            if (arg1->wait++ > 0x1E) {
                arg1->wait = 0;
                arg1->step++;
                TaView_ReqStdCompleteView(0);
            }
        }
        break;
    case 0x23:
        if (TaTuto_ControlFade(0x10, 1, 1) == 1) {
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x24:
        if (TaTuto_OpenTutoMsg(0xA5A, 0xA5C, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0x25:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            SzDuel_DelUnit(1, 6);
            SzDuel_DelUnit(1, 7);
            SzDuel_DelUnit(1, 8);
            SzDuel_SetUnitCtlRate(1, 1, -1, 0);
            SzDuel_SetUnitCtlGoal(1, 1, 0);
            SzDuel_SetUnitCtlRate(1, 2, -1, 0);
            SzDuel_SetUnitCtlGoal(1, 2, 0);
            SzDuel_SetUnitCtlRate(1, 3, -1, 0);
            SzDuel_SetUnitCtlGoal(1, 3, 0);
            SzDuel_SetUnitCtlRate(1, 4, -1, 0);
            SzDuel_SetUnitCtlGoal(1, 4, 0);
            SzDuel_SetUnitCtlRate(1, 5, -1, 0);
            SzDuel_SetUnitCtlGoal(1, 5, 0);
            SzDuel_MoveUnit(1, 1, 6, 0);
            SzDuel_MoveUnit(1, 2, 6, 1);
            SzDuel_MoveUnit(1, 3, 6, 2);
            SzDuel_MoveUnit(1, 4, 6, 3);
            SzDuel_MoveUnit(1, 5, 6, 4);
            SzDuel_MoveUnit(0, 0, 0, 6);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x26:
        SzDuel_SetPlayerCur(0, 0, 6);
        if (arg1->wait++ > 0x1E) {
            arg1->wait = 0;
            arg1->step++;
            TaView_ReqHalfFarView(0);
            return 0x80;
        }
        break;
    case 0x27:
        if (TaTuto_ControlFade(8, 1, 0) == 1) {
            SzDuel_SetWindowDisp(1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x28:
        TaTuto_OpenTutoMsg(0xA5D, 0xA5D, arg1);
        break;
    case 0x29:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            SzDuel_SetUnitCtlGoal(1, 1, 0x80);
            SzDuel_SetUnitCtlRate(1, 1, 1, 0x80);
            SzDuel_SetUnitCtlGoal(1, 2, 0x80);
            SzDuel_SetUnitCtlRate(1, 2, 1, 0x80);
            SzDuel_SetUnitCtlGoal(1, 3, 0x80);
            SzDuel_SetUnitCtlRate(1, 3, 1, 0x80);
            SzDuel_MoveUnit(1, 1, 0, 5);
            SzDuel_MoveUnit(1, 2, 1, 5);
            SzDuel_MoveUnit(1, 3, 1, 6);
            arg1->step++;
        }
        break;
    case 0x2A:
        if (TaTuto_ControlFade(0x10, 1, 0) == 1 && TaTuto_OpenTutoMsg(0xA5E, 0xA5E, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0x2B:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            SzDuel_MoveUnit(1, 1, 6, 0);
            SzDuel_MoveUnit(1, 2, 6, 1);
            SzDuel_MoveUnit(1, 3, 6, 2);
            SzDuel_MoveUnit(0, 0, 3, 5);
            SzDuel_SetField(3, 6, 8);
            SzDuel_SetField(2, 6, 8);
            SzDuel_SetField(4, 6, 8);
            SzDuel_SetField(2, 5, 8);
            SzDuel_SetField(4, 5, 8);
            SzDuel_SetField(2, 4, 8);
            SzDuel_SetField(4, 4, 8);
            arg1->step++;
            return 0x20;
        }
        break;
    case 0x2C:
        SzDuel_SetPlayerCur(0, 3, 5);
        if (arg1->wait++ > 0x1E) {
            arg1->wait = 0;
            arg1->step++;
            return 0x80;
        }
        break;
    case 0x2D:
        if (TaTuto_ControlFade(8, 1, 0) == 1) {
            SzDuel_SetWindowDisp(1);
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x2E:
        TaTuto_OpenTutoMsg(0xA5F, 0xA5F, arg1);
        break;
    case 0x2F:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            SzDuel_MoveUnit(1, 1, 3, 4);
            arg1->step++;
        }
        break;
    case 0x30:
        if (TaTuto_ControlFade(0x10, 1, 0) == 1 && TaTuto_OpenTutoMsg(0xA60, 0xA60, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0x31:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            SzDuel_DelUnit(1, 1);
            SzDuel_DelUnit(1, 2);
            SzDuel_DelUnit(1, 3);
            SzDuel_DelUnit(1, 4);
            SzDuel_DelUnit(1, 5);
            SzDuel_MoveUnit(0, 0, 3, 6);
            SzDuel_SetField(3, 6, 7);
            SzDuel_SetField(2, 6, 7);
            SzDuel_SetField(4, 6, 7);
            SzDuel_SetField(2, 5, 7);
            SzDuel_SetField(4, 5, 7);
            SzDuel_SetField(2, 4, 7);
            SzDuel_SetField(4, 4, 7);
            SzDuel_SetTurnCount(0x63);
            arg1->step++;
            return 0x20;
        }
        break;
    case 0x32:
        if (arg1->wait++ > 0x1E) {
            arg1->wait = 0;
            arg1->step++;
            SzDuel_SetPlayerCur(0, 3, 6);
        }
        break;
    case 0x33:
        if (TaTuto_ControlFade(8, 1, 0) == 1) {
            SzDuel_SetWindowDisp(1);
            TaTuto_OpenTutoMsg(0xA61, 0xA61, arg1);
        }
        break;
    case 0x34:
        TaTuto_DrawFinger(arg1, 0xA, 0x800, 0x1E, -0xB4);
        if (arg1->timer++ > 0x3C) {
            SzDuel_CloseHandCursor();
            arg1->wait = 0;
            arg1->timer = 0;
            arg1->step++;
        }
        break;
    case 0x35:
        if (TaTuto_OpenTutoMsg(0xA62, 0xA65, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        break;
    case 0x36:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            SzDuel_AddUnit(0, 6, 0, 0x39, 0);
            SzDuel_AddUnit(0, 5, 0, 0x38, 0);
            SzDuel_AddUnit(0, 0, 0, 0x37, 0);
            SzDuel_AddUnit(0, 1, 0, 0x36, 0);
            SzDuel_SetTurnCount(0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(0, 2, 4, 1);
            SzDuel_SetUnitStatus(0, 3, 4, 1);
            SzDuel_SetUnitStatus(0, 4, 4, 1);
            arg1->step++;
        }
        break;
    case 0x37:
        if (SzEffect_IsModelStandby2(1) != 0 && SzEffect_IsModelStandby2(2) != 0 && SzEffect_IsModelStandby2(3) != 0 && SzEffect_IsModelStandby2(4) != 0) {
            if (arg1->wait++ > 0x1E) {
                arg1->wait = 0;
                arg1->step++;
            }
        }
        break;
    case 0x38:
        if (TaTuto_ControlFade(8, 1, 0) == 1) {
            SzDuel_SetWindowDisp(1);
            if (TaTuto_OpenTutoMsg(0xA66, 0xA6A, arg1) == 1) {
                SzDuel_SetWindowDisp(0);
            }
        }
        break;
    case 0x39:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1->text);
            TaTuto_FocusUnit__001745f0(0x3A);
            arg1->step++;
        }
        break;
    case 0x3A:
        if (TaTuto_ControlFade(0x10, 1, 0) == 1) {
            SzDuel_OpenMsgPopup(0, -0x78, 0x50, SzDuel_GetMsgName(0x3A));
            if (arg1->wait++ > 0x3C) {
                arg1->wait = 0;
                arg1->step++;
            }
        }
        break;
    case 0x3B:
        TaTuto_OpenTutoMsg(0xA6B, 0xA6B, arg1);
        break;
    case 0x3C:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            SzDuel_MoveUnit(0, 1, 2, 6);
            SzDuel_MoveUnit(0, 2, 4, 6);
            SzDuel_MoveUnit(0, 3, 2, 5);
            SzDuel_MoveUnit(0, 4, 4, 5);
            TaTuto_RestoreOriginalView(arg1->text, 0);
            arg1->step++;
        }
        break;
    case 0x3D:
        if (TaTuto_ControlFade(0x10, 1, 0) == 1 && TaTuto_OpenTutoMsg(0xA6C, 0xA6E, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
            return 0x800;
        }
        break;
    case 0x3E:
        arg1->step++;
        return 0x800;
    case 0x3F:
        if (TaTuto_OpenTutoMsg(0xA6F, 0xA6F, arg1) == 1) {
            return -1;
        }
        break;
    default:
        return -1;
    }
    return 0;
}
