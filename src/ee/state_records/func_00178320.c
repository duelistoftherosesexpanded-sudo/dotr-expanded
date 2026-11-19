#include "dotr/ee/state_records.h"

void TaTuto_SetDeck035(unsigned short n)
{
    switch (n) {
    case 37:
    case 59:
    case 208:
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 321);
        SzDuel_AddHand(0, 7);
        SzDuel_AddHand(0, 156);
        SzDuel_AddHand(0, 249);
        SzDuel_AddHand(0, 294);
        break;
    case 72:
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 15);
        SzDuel_AddHand(0, 5);
        SzDuel_AddHand(0, 156);
        SzDuel_AddHand(0, 249);
        SzDuel_AddHand(0, 294);
        break;
    case 93:
    case 172:
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 321);
        SzDuel_AddHand(0, 7);
        SzDuel_AddHand(0, 156);
        SzDuel_AddHand(0, 249);
        SzDuel_AddHand(0, 766);
        break;
    case 113:
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 321);
        SzDuel_AddHand(0, 7);
        SzDuel_AddHand(0, 156);
        SzDuel_AddHand(0, 249);
        SzDuel_AddHand(0, 752);
        break;
    case 135:
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 321);
        SzDuel_AddHand(0, 7);
        SzDuel_AddHand(0, 156);
        SzDuel_AddHand(0, 766);
        SzDuel_AddHand(0, 766);
        break;
    case 152:
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 15);
        SzDuel_AddHand(0, 5);
        SzDuel_AddHand(0, 294);
        SzDuel_AddHand(0, 766);
        SzDuel_AddHand(0, 766);
        break;
    }
    SzDuel_PackHand(0);
}
