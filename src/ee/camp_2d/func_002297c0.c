#include "dotr/ee/camp_2d.h"

/* 0x2297c0. Callees and data are declared inside the function body; the work area is addressed by
 * its literal through AT. */
#ifndef __MWERKS__
/* Host build only: the code turns the int cursor into a pointer and back. */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif

/* MATCHING DEVICE: the work area by its literal addresses (camp_2d.h). */
#define AT(a) DOTR_CAMP_2D_AT(a)

void *Taku_Camp_2D(int arg0) {
    extern unsigned char te_data_camp2d[];
    extern unsigned char data_002964c6_camp2d[];
    extern unsigned char data_002964ec_camp2d[];
    extern int tex_data[];
    extern int data_00299dd4_camp2d;
    extern short CutSceneNumber_camp2d;
    extern int SzText_LoadRequest();
    extern int SzText_InitClutManage();
    extern int Taku_Te_Set();
    int *cards = tex_data;
    unsigned char *state = (unsigned char *)&data_00299e10_camp2d;
    int sp7C;
    int temp_s2;
    short var_s3;
    int temp_s4;
    int var_s0;
    int temp_s5;
    unsigned short temp_v1;
    int result;

    temp_s2 = SzDuel_GetUserGsAddress();
    sp7C = RhBase_GetUserGsAddress();
    var_s3 = 0;
    temp_s4 = SzDuel_StartWindowSession_Pipe(arg0);
    tex_data[0] = SzDuel_GetWindowTexture(7);
    data_00299dd4_camp2d = SzDuel_GetWindowTexture(6);
    switch (CutSceneNumber_camp2d) {
    case 0x2B0C:
        cards[2] = SzText_LoadRequest(8, 0);
        cards[3] = SzText_LoadRequest(8, 1);
        cards[4] = (int) cards[2];
        cards[5] = (int) cards[3];
        var_s3 = 8;
        break;
    case 0x2B72:
        cards[2] = SzText_LoadRequest(8, 3);
        cards[3] = (int) cards[2];
        cards[4] = SzText_LoadRequest(8, 2);
        cards[5] = (int) cards[4];
        var_s3 = 8;
        break;
    case 0x2B86:
        cards[2] = SzText_LoadRequest(8, 5);
        cards[3] = (int) cards[2];
        cards[4] = SzText_LoadRequest(8, 4);
        cards[5] = (int) cards[4];
        var_s3 = 8;
        break;
    case 0x2B88:
        cards[2] = SzText_LoadRequest(8, 6);
        cards[3] = SzText_LoadRequest(8, 7);
        cards[4] = (int) cards[2];
        cards[5] = (int) cards[3];
        var_s3 = 8;
        break;
    case 0x2B8A:
        cards[2] = SzText_LoadRequest(8, 8);
        cards[3] = (int) cards[2];
        cards[4] = (int) cards[2];
        cards[5] = (int) cards[2];
        var_s3 = 8;
        break;
    case 0x2BAE:
        cards[2] = SzText_LoadRequest(8, 10);
        cards[3] = SzText_LoadRequest(8, 12);
        cards[4] = SzText_LoadRequest(8, 9);
        cards[5] = SzText_LoadRequest(8, 11);
        var_s3 = 8;
        break;
    case 0x2F7A:
        cards[2] = SzText_LoadRequest(8, 14);
        cards[3] = SzText_LoadRequest(8, 16);
        cards[4] = SzText_LoadRequest(8, 13);
        cards[5] = SzText_LoadRequest(8, 15);
        var_s3 = 8;
        break;
    }
    *(signed char *)AT(0x01ED5436) = 0;
    if ((*(short *)(state + 0xC2) >= 0x2724) && (*(short *)(state + 0xC2) < 0x2738) && (*(signed char *)AT(0x01ED537D) >= 3)) {
        cards[6] = SzText_LoadRequest(8, 20);
        cards[7] = SzText_LoadRequest(8, 22);
        cards[8] = SzText_LoadRequest(8, 19);
        cards[9] = SzText_LoadRequest(8, 18);
        cards[10] = SzText_LoadRequest(8, 21);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if ((*(short *)(state + 0xC2) >= 0x2738) && (*(short *)(state + 0xC2) < 0x276B)) {
        cards[6] = SzText_LoadRequest(8, 17);
        cards[7] = SzText_LoadRequest(8, 18);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if ((*(short *)(state + 0xC2) == 0x2AF8) ||
        (*(short *)(state + 0xC2) == 0x2EE0)) {
        cards[6] = SzText_LoadRequest(8, 17);
        cards[7] = SzText_LoadRequest(8, 18);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if ((*(short *)(state + 0xC2) >= 0x2B84) && (*(short *)(state + 0xC2) < 0x2B86)) {
        cards[6] = SzText_LoadRequest(8, 20);
        cards[7] = SzText_LoadRequest(8, 19);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if (*(short *)(state + 0xC2) == 0x2B8C) {
        cards[6] = SzText_LoadRequest(8, 22);
        cards[7] = SzText_LoadRequest(8, 19);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if ((*(short *)(state + 0xC2) >= 0x2B98) && (*(short *)(state + 0xC2) < 0x2B9B)) {
        cards[6] = SzText_LoadRequest(8, 18);
        cards[7] = SzText_LoadRequest(8, 23);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if (*(short *)(state + 0xC2) == 0x2BAC) {
        cards[6] = SzText_LoadRequest(8, 18);
        cards[7] = SzText_LoadRequest(8, 23);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if ((*(short *)(state + 0xC2) >= 0x2F62) && (*(short *)(state + 0xC2) < 0x2F65)) {
        cards[6] = SzText_LoadRequest(8, 18);
        cards[7] = SzText_LoadRequest(8, 23);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if (*(short *)(state + 0xC2) == 0x2F76) {
        cards[6] = SzText_LoadRequest(8, 18);
        cards[7] = SzText_LoadRequest(8, 23);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if (*(short *)(state + 0xC2) == 0x2F78) {
        cards[6] = SzText_LoadRequest(8, 18);
        *(signed char *)AT(0x01ED5436) = 1;
        var_s3 = 8;
    }
    if (var_s3) {
        temp_s2 += SzText_InitClutManage(temp_s2, 2);
        temp_s5 = (short)var_s3;
        arg0 = SzText_DoRequest_Pipe(temp_s4, temp_s5, temp_s2, &sp7C);
        var_s0 = 0;
        while (1) {
            SzText_FreeRequest(temp_s5, var_s0);
            var_s0++;
            if (var_s0 > 23) break;
        }
        temp_s4 = RhText_SetTextureFlush(arg0);
    }
    result = (int)Taku_Camp_SYS_ICON((void *)temp_s4);
    if ((*(signed char *)AT(0x01ED5374) == 0) && (*(signed char *)AT(0x01ED5481) != 0) &&
        (SzEffect_IsUse2(-1, -1) == 0) && (*(signed char *)AT(0x01ED5376) != 0) &&
        (*(signed char *)AT(0x01ED5482) == 0)) {
        temp_v1 = *(unsigned short *)AT(0x01ED0004);
        if (temp_v1 == 0) {
            result = Taku_Te_Set(result, te_data_camp2d, *(short *)AT(0x01ED0000), *(short *)AT(0x01ED0002));
        } else if (temp_v1 == 1) {
            result = Taku_Te_Set(result, data_002964c6_camp2d, *(short *)AT(0x01ED0000), *(short *)AT(0x01ED0002));
        } else {
            result = Taku_Te_Set(result, data_002964c6_camp2d, *(short *)AT(0x01ED0000), *(short *)AT(0x01ED0002));
            result = Taku_Te_Set(result, data_002964ec_camp2d, *(short *)AT(0x01ED0000), *(short *)AT(0x01ED0002));
        }
    }
    if (*(short *)(state + 0xC2) >= 0x2710) {
        result = Taku_Camp_Ivent_Window(Taku_Camp_Ivent_Ga(result));
    }
    result = Taku_Camp_ST_WINDOW(result);
    result = Taku_Camp_TEKI_WINDOW(result);
    result = Taku_Camp_TIKEI_WINDOW(result);
    if (*(short *)AT(0x01ED0080) == 3) {
        result = Taku_Camp_Help_Window(result);
    }
    result = Taku_Camp_SINEMA_CUT(result, 0);
    result = Taku_Camp_BUN_HYOUJI(result);
    if (*(signed char *)AT(0x01ED53B4) != 0 || *(signed char *)AT(0x01ED53B5) != 0) {
        result = Taku_Camp_BUN_DARE(result, *(signed char *)AT(0x01ED537E));
    }
    return (void *)Taku_DECK_CARD_DISP(SzDuel_FinishWindowSession_Pipe(Taku_Camp_BOTAN_MATI(result)));
}
