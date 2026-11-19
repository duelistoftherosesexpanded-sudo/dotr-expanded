#include "dotr/ee/camp_tikei.h"

void TkCamp_Draw_After(void)
{
    signed char *camp = data_00299e10;
    short cardX = ((DOTR_TIKEI_CARD_X << 10) >> 12) - 512;
    short dx;
    short dy;
    int y;
    int x;
    unsigned int scroll;
    DotrTikeiWord cursor;
    struct DotrTikeiVector pos;
    struct DotrTikeiVector rot;

    if (Move_END_flg != 0) {
        return;
    }
    cursor = Taku_Camp_2D(RhBase_GetVifPacketArea());
    if (DOTR_TIKEI_SCROLL != 0) {
        /* MATCHING DEVICE: the scroll goes through an unsigned temporary before the
         * subtraction; a direct `DOTR_TIKEI_SCROLL - 256` compiles to two fewer instructions. */
        scroll = DOTR_TIKEI_SCROLL;
        dy = scroll - 256;
        dx = DOTR_TIKEI_LEADER_SLIDE * -640 / 100;
        if (DOTR_TIKEI_QUAD_ON != 0) {
            cursor = Taku_G4_Set(cursor, &teki_a_seru, dx, dy);
            DOTR_TIKEI_QUAD_ON = 0;
        }
        if (!SzModel_IsModelLoading(31) && DOTR_TIKEI_LEADER_STATE == 1 && DOTR_TIKEI_SCROLL != 0) {
            pos = data_0029a7b0;
            rot = data_0029a7c0;
            DOTR_TIKEI_LEADER = SzData_GetLeader(1, 0);
            SzModel_SetPos(31, &pos);
            SzModel_SetRot(31, &rot);
            SzModel_Load(31, -1, SzDuel_New2Xax(DOTR_TIKEI_LEADER), 1);
            DOTR_TIKEI_LEADER_STATE = 2;
            DOTR_TIKEI_LEADER_ALPHA = 0;
        }
        if (SzModel_IsModelStandby(31) && DOTR_TIKEI_LEADER_STATE == 2) {
            if (DOTR_TIKEI_LEADER_ALPHA < 128) {
                DOTR_TIKEI_LEADER_ALPHA += 8;
            }
            if (DOTR_TIKEI_LEADER_ALPHA >= 128) {
                DOTR_TIKEI_LEADER_ALPHA = 128;
            }
            y = teki_a_seru.w[1];
            x = teki_a_seru.w[0];
            cursor = Taku_Camp_Teki_Model_Disp(cursor, 31, x + dx, y + dy, teki_a_seru.w[2] - x,
                                               teki_a_seru.w[5] - y,
                                               SzDuel_GetCamDist(DOTR_TIKEI_LEADER),
                                               DOTR_TIKEI_LEADER_ALPHA);
        }
    }
    if (DOTR_TIKEI_CARD_ON != 0) {
        cursor = Taku_Camp_Card_Get(cursor, camp[0] ^ 1, cardX, 0.0f, DOTR_TIKEI_CARD);
    }
    DOTR_TIKEI_FLAG_5481 = 0;
    RhBase_SetVifPacketArea(cursor);
}
