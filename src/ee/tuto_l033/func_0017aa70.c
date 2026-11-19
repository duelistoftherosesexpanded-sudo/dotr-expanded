#include "dotr/ee/tuto_l033.h"

/* L036 (0x17aa70): one tutorial script, a sibling of L033 (same record). Each call mirrors the
 * message window's display flag into its draw flag and sets the analogue info of port 0 axes
 * 0..3 to 0x80; pad button 0x800 returns -2. Then the step word (+0x1a2, through jump tables)
 * runs one step: messages 0xbc1..0xbe8 (TaTuto_OpenTutoMsg), fades, waits on the counters at
 * +0x1a4 and +0x162, the pointing finger, analogue-axis flashes, and the button the player is
 * to press next (returned as a mask). Past the last step it returns -1.
 * Its callees without prototypes are declared in the header with int-parameter prototypes. */
int L036(int arg0, void *tutorial)
{
    struct DotrTutoL033Record *arg1 = tutorial;
    int x;
    int y;
    unsigned short flag = 0;
    unsigned short first;
    unsigned short last;
    unsigned short kind;

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
        arg1->step = 2;
        break;
    case 0x1:
        if (SzEffect_IsModelStandby2(0) != 0) {
            if (SzEffect_IsModelStandby2(0x100) != 0) {
                arg1->step = 0x35;
            }
        }
        break;
    case 0x2:
    case 0xA:
    case 0xD:
    case 0x11:
    case 0x13:
    case 0x19:
    case 0x1C:
    case 0x20:
    case 0x22:
    case 0x24:
    case 0x27:
    case 0x2A:
    case 0x2E:
    case 0x30:
    case 0x32:
    case 0x36:
    case 0x39:
    case 0x3C:
    case 0x3E:
    case 0x40:
    case 0x42:
    case 0x44:
        switch (arg1->step) {
        case 0x44:
            first = 0x20C;
            last = 0x20C;
            kind = 0;
            break;
        case 0x42:
            first = 0x20A;
            last = 0x20B;
            kind = 1;
            break;
        case 0x40:
            first = 0x209;
            last = 0x209;
            kind = 0;
            break;
        case 0x3E:
            first = 0x208;
            last = 0x208;
            kind = 0;
            break;
        case 0x3C:
            first = 0x206;
            last = 0x207;
            kind = 0;
            break;
        case 0x39:
            first = 0x205;
            last = 0x205;
            kind = 0;
            break;
        case 0x36:
            first = 0x202;
            last = 0x204;
            kind = 0;
            break;
        case 0x32:
            first = 0x201;
            last = 0x201;
            kind = 1;
            break;
        case 0x30:
            first = 0x200;
            last = 0x200;
            kind = 0;
            break;
        case 0x2E:
            first = 0x1FD;
            last = 0x1FF;
            kind = 0;
            break;
        case 0x2A:
            first = 0x1FA;
            last = 0x1FC;
            kind = 2;
            break;
        case 0x27:
            first = 0x1F8;
            last = 0x1F9;
            kind = 0;
            break;
        case 0x24:
            first = 0x1F4;
            last = 0x1F7;
            kind = 0;
            break;
        case 0x22:
            first = 0x1F3;
            last = 0x1F3;
            kind = 0;
            break;
        case 0x20:
            first = 0x1F1;
            last = 0x1F2;
            kind = 0;
            break;
        case 0x1C:
            first = 0x1EF;
            last = 0x1F0;
            kind = 2;
            break;
        case 0x19:
            first = 0x1EE;
            last = 0x1EE;
            kind = 0;
            break;
        case 0x13:
            first = 0x1EC;
            last = 0x1ED;
            kind = 0;
            break;
        case 0x11:
            first = 0x1EA;
            last = 0x1EB;
            kind = 0;
            break;
        case 0xD:
            first = 0x1E9;
            last = 0x1E9;
            kind = 2;
            break;
        case 0xA:
            first = 0x1E7;
            last = 0x1E8;
            kind = 0;
            break;
        case 0x2:
            first = 0x1E5;
            last = 0x1E6;
            kind = 0;
            break;
        default:
            first = 0x1E5;
            last = 0x1E5;
            kind = 0;
            break;
        }
        if (TaTuto_OpenTutoMsg(first + 0x9DC, last + 0x9DC, arg1) == 1) {
            switch (kind) {
            case 2:
                SzDuel_SetWindowDisp(0);
                return 0x20;
            case 1:
                SzDuel_SetWindowDisp(0);
                break;
            }
        }
        break;
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x21:
        if (arg1->wait++ > 0xC) {
            arg1->wait = 0;
            switch (arg1->step++) {
            case 0x21:
                return 4;
            case 0x14:
            case 0x16:
                return 0x2000;
            case 0x15:
            case 0x17:
                return 0x8000;
            case 0x18:
                break;
            }
        }
        break;
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xB:
    case 0xC:
    case 0x12:
    case 0x1A:
    case 0x25:
    case 0x28:
    case 0x2F:
    case 0x31:
    case 0x41:
        if (arg1->wait++ > 0x6) {
            arg1->wait = 0;
            switch (arg1->step++) {
            case 0x2F:
            case 0x31:
                return 1;
            case 0x1A:
                return 0x10;
            case 0x12:
                return 8;
            case 0x3:
            case 0xC:
                return 0x80;
            case 0x4:
                return 0x1000;
            case 0x5:
            case 0x8:
            case 0x9:
                return 0x40;
            case 0x6:
            case 0x7:
                return 0x2000;
            case 0xB:
            case 0x25:
            case 0x28:
                return 0x4000;
            case 0x41:
                return 0x600;
            }
        }
        break;
    case 0x1B:
        if (arg1->wait++ > 0x1E) {
            arg1->wait = 0;
            switch (arg1->step++) {
            case 0x1B:
                return 0x20;
            }
        }
        break;
    case 0xE:
    case 0x1D:
    case 0x2B:
    case 0x33:
    case 0x43:
        if (TaTuto_ControlFade(0x10, -1, 0) == 1) {
            arg1->wait = 0;
            switch (arg1->step++) {
            case 0xE:
                SzDuel_DelUnit(0, 1);
                break;
            case 0x1D:
            case 0x2B:
            case 0x33:
            case 0x43:
                break;
            }
        }
        break;
    case 0xF:
    case 0x1E:
    case 0x2C:
    case 0x34:
        if (arg1->wait++ > 0x1E) {
            arg1->wait = 0;
            arg1->step++;
        }
        break;
    case 0x10:
    case 0x1F:
    case 0x2D:
    case 0x35:
        switch (arg1->step) {
        case 0x10:
        case 0x1F:
        case 0x2D:
        case 0x35:
            flag = 1;
            break;
        }
        if (TaTuto_ControlFade(0x10, 1, 1) == 1) {
            if (flag) {
                SzDuel_SetWindowDisp(1);
            }
            arg1->step++;
            arg1->wait = 0;
        }
        break;
    case 0x23:
    case 0x26:
    case 0x29:
        switch (arg1->step) {
        case 0x23:
            arg1->step = 0x26;
            return 0;
        case 0x26:
            x = -0x48;
            y = 0x14;
            break;
        case 0x29:
            x = -0xA8;
            y = 0x3C;
            break;
        }
        TaTuto_DrawFinger(arg1, 0xA, 0x800, x, y);
        if (arg1->timer++ > 0x3C) {
            SzDuel_CloseHandCursor();
            arg1->wait = 0;
            arg1->timer = 0;
            arg1->step++;
        }
        break;
    case 0x37:
        SzPad_SetAnalogInfo(0, 2, 0);
        if (arg1->timer++ > 0x6) {
            arg1->wait = 0;
            arg1->timer = 0;
            arg1->step++;
        }
        break;
    case 0x38:
        if (arg1->timer++ >= 0xC) {
            SzPad_SetAnalogInfo(0, 2, 0xFF);
            if (arg1->timer > 0x12) {
                arg1->wait = 0;
                arg1->timer = 0;
                arg1->step++;
            }
        }
        break;
    case 0x3A:
        SzPad_SetAnalogInfo(0, 3, 0xFF);
        if (arg1->timer++ > 0x6) {
            arg1->wait = 0;
            arg1->timer = 0;
            arg1->step++;
        }
        break;
    case 0x3B:
        if (arg1->timer++ >= 0xC) {
            SzPad_SetAnalogInfo(0, 3, 0);
            if (arg1->timer > 0x12) {
                arg1->wait = 0;
                arg1->timer = 0;
                arg1->step++;
            }
        }
        break;
    case 0x3D:
        SzPad_SetAnalogInfo(0, 1, 0);
        if (arg1->timer++ > 0x6) {
            arg1->wait = 0;
            arg1->timer = 0;
            arg1->step++;
        }
        break;
    case 0x3F:
        if (arg1->timer++ >= 0xC) {
            SzPad_SetAnalogInfo(0, 1, 0xFF);
            if (arg1->timer > 0x12) {
                arg1->wait = 0;
                arg1->timer = 0;
                arg1->step++;
            }
        }
        break;
    default:
        return -1;
    }
    return 0;
}

