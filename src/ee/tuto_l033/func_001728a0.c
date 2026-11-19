#include "dotr/ee/tuto_l033.h"

/* L032 (0x1728a0): one tutorial script, the sibling of L033 (same group). Each call mirrors the
 * message window's display flag into its draw flag and sets the analogue info of port 0 axes
 * 0..3 to 0x80; pad button 0x800 returns -2. Then the step halfword at +0x1a2 runs one step
 * (switch through a jump table): fades, tutorial messages 0x9dc..0xa47, unit and life-point
 * set-up, waits on the counters at +0x1a4 and +0x162, the pointing finger, and the button the
 * player is to press next (returned as a mask). Past the last step it returns -1.
 * The record is read through raw byte offsets: it is a byte pointer (func_0027fc20_l032 /
 * func_0027f630_l032 take it so). Compiled with #pragma global_optimizer off. */
#ifdef __MWERKS__
#pragma global_optimizer off
#endif

int L032(int arg0, unsigned char *arg1)
{
    int temp_a0;
    unsigned short temp_a0_2;
    unsigned short temp_a0_3;
    unsigned short temp_a0_4;
    unsigned short temp_a0_5;
    unsigned short temp_a0_6;
    unsigned short temp_a0_7;
    unsigned short temp_a0_8;
    unsigned short temp_v1_17;
    unsigned short temp_v1_30;

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
    temp_a0 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
    switch (temp_a0) {
    case 0x0:
        (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = 2U;
        goto block_296;
    case 0x1:
        if (SzEffect_IsModelStandby2(0) != 0) {
            if (SzEffect_IsModelStandby2(0x100) != 0) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x2:
        if (func_0027fc20_l032(0x9DC, 0x9DF, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x3:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            TaTuto_FocusUnit__00172510(0x3C);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
        }
        goto block_296;
    case 0x4:
        if (TaTuto_ControlFade(0x10, 1, 0) == 1) {
            SzDuel_OpenMsgPopup(0, -0x78, 0x50, SzDuel_GetMsgName(0x3C));
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x3c) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x5:
        func_0027fc20_l032(0x9E0, 0x9E1, arg1);
        goto block_296;
    case 0x6:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            TaTuto_RestoreOriginalView(arg1 + 0x1B0, 0);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
        }
        goto block_296;
    case 0x7:
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(0x10, 1, 0) == 1) {
            func_0027fc20_l032(0x9E2, 0x9E2, arg1);
        }
        goto block_296;
    case 0x8:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x80;
        }
        goto block_296;
    case 0x9:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0xA:
        func_0027fc20_l032(0x9E3, 0x9E5, arg1);
        goto block_296;
    case 0xB:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x1000;
        }
        goto block_296;
    case 0xC:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x40;
        }
        goto block_296;
    case 0xD:
        func_0027fc20_l032(0x9E6, 0x9E8, arg1);
        (*(unsigned short *)((unsigned char *)(arg1) + 0x162)) = 0U;
        goto block_296;
    case 0xE:
        func_0027f630_l032(arg1, 0xA, 0x800, -0xE8, -0x8C);
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x162)))++ > 0x3c) {
            SzDuel_CloseHandCursor();
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            (*(unsigned short *)((unsigned char *)(arg1) + 0x162)) = 0U;
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
        }
        goto block_296;
    case 0xF:
        func_0027fc20_l032(0x9E9, 0x9EC, arg1);
        goto block_296;
    case 0x10:
        func_0027f630_l032(arg1, 0xA, 0x800, 0x3C, 0x4B);
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x162)))++ > 0x3c) {
            SzDuel_CloseHandCursor();
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            (*(unsigned short *)((unsigned char *)(arg1) + 0x162)) = 0U;
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
        }
        goto block_296;
    case 0x11:
        func_0027fc20_l032(0x9ED, 0x9F0, arg1);
        goto block_296;
    case 0x12:
    case 0x13:
    case 0x14:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x2000;
        }
        goto block_296;
    case 0x15:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x8000;
        }
        goto block_296;
    case 0x16:
        func_0027fc20_l032(0x9F1, 0x9F1, arg1);
        goto block_296;
    case 0x17:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x40;
        }
        goto block_296;
    case 0x18:
        func_0027fc20_l032(0x9F2, 0x9F3, arg1);
        goto block_296;
    case 0x19:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x40;
        }
        goto block_296;
    case 0x1A:
        if (func_0027fc20_l032(0x9F4, 0x9F6, arg1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            TaView_ReqStdCompleteView(0x1E);
        }
        goto block_296;
    case 0x1B:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x3c) {
            SzDuel_SetWindowDisp(0);
            if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
                TaTuto_FocusUnit__00172510(0);
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x1C:
        if (TaTuto_ControlFade(0x10, 1, 0) == 1) {
            SzDuel_OpenMsgPopup(0, -0x78, 0x50, SzDuel_GetMsgName(0));
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x3c) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x1D:
        func_0027fc20_l032(0x9F7, 0x9F7, arg1);
        goto block_296;
    case 0x1E:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            TaTuto_RestoreOriginalView(arg1 + 0x1B0, 0);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x1F:
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(0x10, 1, 0) == 1) {
            func_0027fc20_l032(0x9F8, 0x9F8, arg1);
        }
        goto block_296;
    case 0x20:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x40;
        }
        goto block_296;
    case 0x21:
        func_0027fc20_l032(0x9F9, 0x9FB, arg1);
        goto block_296;
    case 0x22:
    case 0x23:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_2 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_2 + 1);
            switch (temp_a0_2) {
            case 34:
                return 0x1000;
            case 35:
                /* fallthrough */
            default:
                return 0x40;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x24:
        if (func_0027fc20_l032(0x9FC, 0xA00, arg1) == 1) {
            return 0x800;
        }
        goto block_296;
    case 0x25:
        func_0027fc20_l032(0xA01, 0xA01, arg1);
        goto block_296;
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x30:
    case 0x31:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 6) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_v1_17 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_v1_17 + 1);
            switch (temp_v1_17) {
            case 38:
            case 40:
            case 43:
            case 44:
            case 45:
            case 46:
            case 48:
                return 0x40;
            case 39:
            case 42:
            case 47:
                return 0x1000;
            case 41:
                return 0x80;
            case 49:
            default:
                return 0x800;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x32:
        func_0027fc20_l032(0xA02, 0xA05, arg1);
        goto block_296;
    case 0x33:
    case 0x34:
    case 0x36:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_3 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_3 + 1);
            switch (temp_a0_3) {
            case 0x33:
                /* fallthrough */
            case 0x36:
                return 0x40;
            case 0x34:
                return 0x1000;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x35:
        func_0027fc20_l032(0xA06, 0xA06, arg1);
        goto block_296;
    case 0x37:
        if (func_0027fc20_l032(0xA07, 0xA09, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x38:
        if (TaTuto_ControlFade(8, -1, 0) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xFA0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 0x15, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x39:
        if ((SzEffect_IsModelStandby2(1) != 0) && (SzEffect_IsModelStandby2(0x101) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 4);
            }
        }
        goto block_296;
    case 0x3A:
        TaTuto_FocusUnit__00172510(0x9C);
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            SzDuel_SetWindowDisp(1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x3B:
        func_0027fc20_l032(0xA0A, 0xA0E, arg1);
        goto block_296;
    case 0x3C:
    case 0x3D:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_4 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_4 + 1);
            switch (temp_a0_4) {
            case 0x3C:
                return 0x1000;
            case 0x3D:
                return 0x4000;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x3E:
        if (func_0027fc20_l032(0xA0F, 0xA0F, arg1) == 1) {
            return 0x40;
        }
        goto block_296;
    case 0x3F:
    case 0x40:
    case 0x46:
    case 0x47:
    case 0x4D:
    case 0x4E:
    case 0x54:
    case 0x55:
    case 0x5B:
    case 0x5C:
    case 0x62:
    case 0x63:
    case 0x69:
    case 0x6A:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_5 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_5 + 1);
            switch (temp_a0_5) {
            case 0x3F:
            case 0x46:
            case 0x4D:
            case 0x54:
            case 0x5B:
            case 0x62:
            case 0x69:
                return 0x1000;
            case 0x40:
            case 0x47:
            case 0x4E:
            case 0x55:
            case 0x5C:
            case 0x63:
            case 0x6A:
                return 0x40;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x41:
        if (func_0027fc20_l032(0xA10, 0xA11, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x42:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 5, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x43:
        if ((SzEffect_IsModelStandby2(1) != 0) && (SzEffect_IsModelStandby2(0x101) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 4);
            }
        }
        goto block_296;
    case 0x44:
        TaTuto_FocusUnit__00172510(0x9C);
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x45:
        if (func_0027fc20_l032(0xA12, 0xA13, arg1) != 0) {
            return 0x40;
        }
        goto block_296;
    case 0x48:
        if (func_0027fc20_l032(0xA14, 0xA14, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x49:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xFA0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 0x16, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x4A:
        if ((SzEffect_IsModelStandby2(1) != 0) && (SzEffect_IsModelStandby2(0x101) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 4);
            }
        }
        goto block_296;
    case 0x4B:
        SzDuel_SetWindowDisp(1);
        TaTuto_FocusUnit__00172510(0x9C);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x4C:
        if (func_0027fc20_l032(0xA15, 0xA16, arg1) != 0) {
            return 0x40;
        }
        goto block_296;
    case 0x4F:
        if (func_0027fc20_l032(0xA17, 0xA18, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x50:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xFA0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 0xA9, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 3, 1);
            SzDuel_SetUnitStatus(1, 1, 5, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x51:
        if ((SzEffect_IsModelStandby2(1) != 0) && (SzEffect_IsModelStandby2(0x101) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 4);
            }
        }
        goto block_296;
    case 0x52:
        SzDuel_SetWindowDisp(1);
        TaTuto_FocusUnit__00172510(0x9C);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x53:
        if (func_0027fc20_l032(0xA19, 0xA1C, arg1) != 0) {
            return 0x40;
        }
        goto block_296;
    case 0x56:
        if (func_0027fc20_l032(0xA1D, 0xA1D, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x57:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xFA0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 0x64, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 3, 1);
            SzDuel_SetUnitStatus(1, 1, 5, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x58:
        if ((SzEffect_IsModelStandby2(1) != 0) && (SzEffect_IsModelStandby2(0x101) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 4);
            }
        }
        goto block_296;
    case 0x59:
        SzDuel_SetWindowDisp(1);
        TaTuto_FocusUnit__00172510(0x9C);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x5A:
        if (func_0027fc20_l032(0xA1E, 0xA1F, arg1) != 0) {
            return 0x40;
        }
        goto block_296;
    case 0x5D:
        if (func_0027fc20_l032(0xA20, 0xA21, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x5E:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xFA0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 4, 0x9C, 0);
            SzDuel_AddUnit(1, 3, 3, 0x277, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 3, 1);
            SzDuel_SetUnitStatus(1, 1, 5, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x5F:
        if ((SzEffect_IsModelStandby2(1) != 0) && (SzEffect_IsModelStandby2(0x101) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 4);
            }
        }
        goto block_296;
    case 0x60:
        SzDuel_SetWindowDisp(1);
        TaTuto_FocusUnit__00172510(0x9C);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x61:
        if (func_0027fc20_l032(0xA22, 0xA22, arg1) != 0) {
            return 0x40;
        }
        goto block_296;
    case 0x64:
        if (func_0027fc20_l032(0xA23, 0xA25, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x65:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_CalcLP(0, 4, 0xFA0);
            SzDuel_CalcLP(1, 4, 0xED8);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(1, 1);
            SzDuel_AddUnit(0, 3, 2, 0x9C, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x66:
        if (SzEffect_IsModelStandby2(1) != 0) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
                SzDuel_SetPlayerCur(0, 3, 2);
            }
        }
        goto block_296;
    case 0x67:
        SzDuel_SetWindowDisp(1);
        TaTuto_FocusUnit__00172510(0x9C);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x68:
        if (func_0027fc20_l032(0xA26, 0xA2A, arg1) != 0) {
            return 0x40;
        }
        goto block_296;
    case 0x6B:
        if (func_0027fc20_l032(0xA2B, 0xA2C, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x6C:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            SzDuel_AddUnit(0, 3, 5, 0x272, 0);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x6D:
        SzDuel_SetPlayerCur(0, 3, 5);
        if (SzEffect_IsModelStandby2(2) != 0) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x6E:
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x6F:
        if (func_0027fc20_l032(0xA2D, 0xA2F, arg1) == 1) {
            return 0x40;
        }
        goto block_296;
    case 0x70:
        func_0027fc20_l032(0xA30, 0xA30, arg1);
        goto block_296;
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7D:
    case 0x7E:
    case 0x7F:
    case 0x80:
    case 0x82:
    case 0x83:
    case 0x84:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_v1_30 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_v1_30 + 1);
            switch (temp_v1_30) {
            case 0x84:
                return 0x40;
            case 0x72:
            case 0x74:
                return 4;
            case 0x71:
            case 0x73:
            case 0x76:
            case 0x79:
            case 0x82:
                return 8;
            case 0x77:
                return 0x1000;
            case 0x78:
                return 0x4000;
            case 0x7A:
                return 0x2000;
            case 0x7B:
                return 0x8000;
            case 0x7D:
            case 0x7F:
            case 0x83:
                return 2;
            case 0x7E:
            case 0x80:
                return 1;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x75:
        func_0027fc20_l032(0xA31, 0xA31, arg1);
        goto block_296;
    case 0x7C:
        func_0027fc20_l032(0xA32, 0xA35, arg1);
        goto block_296;
    case 0x81:
        func_0027fc20_l032(0xA36, 0xA36, arg1);
        goto block_296;
    case 0x85:
        if (func_0027fc20_l032(0xA37, 0xA38, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x86:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_AddUnit(1, 2, 2, 3, 0);
            SzDuel_AddUnit(0, 2, 6, 0x2AB, 0);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x87:
        SzDuel_SetPlayerCur(0, 2, 2);
        if (SzEffect_IsModelStandby2(0x101) != 0) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x88:
        SzDuel_SetWindowDisp(1);
        TaTuto_FocusUnit__00172510(3);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x89:
        func_0027fc20_l032(0xA39, 0xA3B, arg1);
        goto block_296;
    case 0x8A:
        if (((int) (arg0 & 0xFFFF) >> 8) == 6) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0 + 1);
            return 0x10;
        }
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 3) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x4000;
        }
        goto block_296;
    case 0x8B:
    case 0x8D:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x3c) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_6 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_6 + 1);
            switch (temp_a0_6) {
            case 0x8B:
                return 0x4000;
            case 0x8D:
                return 0x20;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x8C:
        func_0027fc20_l032(0xA3C, 0xA40, arg1);
        goto block_296;
    case 0x8E:
    case 0x8F:
    case 0x90:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_7 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_7 + 1);
            switch (temp_a0_7) {
            case 0x8E:
                /* fallthrough */
            case 0x90:
                return 0x40;
            case 0x8F:
                return 2;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x91:
        if (((int) (arg0 & 0xFFFF) >> 8) == 2) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0 + 1);
            goto block_296;
        }
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 3) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            return 0x1000;
        }
        goto block_296;
    case 0x92:
        if (func_0027fc20_l032(0xA41, 0xA41, arg1) == 1) {
            SzDuel_SetWindowDisp(0);
        }
        goto block_296;
    case 0x93:
        if (TaTuto_ControlFade(8, -1, 1) == 1) {
            TaTuto_BkupOriginalView(arg1 + 0x1B0);
            SzDuel_DelUnit(0, 1);
            SzDuel_DelUnit(0, 2);
            SzDuel_DelUnit(1, 1);
            SzDuel_CalcLP(0, 4, 0x320);
            SzDuel_AddUnit(1, 2, 6, 0, 0);
            SzDuel_AddUnit(1, 3, 5, 0xA0, 0);
            SzDuel_AddUnit(1, 4, 5, 0x82, 0);
            SzDuel_AddUnit(0, 4, 6, 0x9D, 0);
            SzDuel_SetUnitStatus(0, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 1, 4, 1);
            SzDuel_SetUnitStatus(1, 2, 4, 1);
            SzDuel_SetUnitStatus(1, 3, 4, 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x94:
        SzDuel_SetPlayerCur(0, 3, 6);
        (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
        goto block_296;
    case 0x95:
        if ((SzEffect_IsModelStandby2(0x101) != 0) && (SzEffect_IsModelStandby2(0x102) != 0) && (SzEffect_IsModelStandby2(0x103) != 0) && (SzEffect_IsModelStandby2(1) != 0)) {
            if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0x1e) {
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
                (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            }
        }
        goto block_296;
    case 0x96:
        SzDuel_SetWindowDisp(1);
        if (TaTuto_ControlFade(8, 1, 1) == 1) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) ((*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) + 1);
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
        }
        goto block_296;
    case 0x97:
        func_0027fc20_l032(0xA42, 0xA44, arg1);
        goto block_296;
    case 0x98:
    case 0x99:
    case 0x9A:
        if (((*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)))++ > 0xc) {
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A4)) = 0U;
            temp_a0_8 = (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2));
            (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0_8 + 1);
            switch (temp_a0_8) {
            case 0x98:
                return 0x100;
            case 0x99:
                return 0x8000;
            case 0x9A:
                return 0x20;
            }
        } else {
            goto block_296;
        }
        break;
    case 0x9B:
        (*(unsigned short *)((unsigned char *)(arg1) + 0x1A2)) = (unsigned short) (temp_a0 + 1);
        goto block_296;
    case 0x9C:
        func_0027fc20_l032(0xA45, 0xA46, arg1);
        SzDuel_SetWindowDisp(0);
        goto block_296;
    case 0x9D:
        if ((TaTuto_ControlFade(8, -1, 0) == 1) && (func_0027fc20_l032(0xA47, 0xA47, arg1) == 1)) {
            return -1;
        }
        goto block_296;
    default:
        return -1;
    }
block_296:
    return 0;
}

#ifdef __MWERKS__
#pragma global_optimizer reset
#endif
