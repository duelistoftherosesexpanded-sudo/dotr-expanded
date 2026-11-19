#include "dotr/ee/cerem_sacrifice.h"

/* Prototype from the Japanese build's debug information (Magic.c). Each word appears to be a
 * search type in the low half with its extra value in the high half: a plain 0-25 is a monster
 * type or attribute, 0x1C (28) a card number (0x009B001C: card 155, Gaia the Fierce Knight),
 * 0x25 (37) attack at most the value (0x05DC0025: 1500 or less) and 0x39 (57) defence at
 * least the value (0x07D00039: 2000 or more). */
void Ta_GetCeremSacrifice(int *Buf, int Obj)
{
    Buf[0] = Buf[1] = Buf[2] = -1;
    switch (Obj) {
    case 189: /* Millennium Shield */
        Buf[0] = 0x07D00039;
        Buf[1] = 0x07D00039;
        Buf[2] = 0x07D00039;
        break;
    case 13: /* Yamadron */
        Buf[0] = 4;
        Buf[1] = 0;
        Buf[2] = 17;
        break;
    case 145: /* Gate Guardian */
        Buf[0] = 0x0063001C;
        Buf[1] = 0x023D001C;
        Buf[2] = 0x0216001C;
        break;
    case 143: /* Black Luster Soldier */
        Buf[0] = 0x009B001C;
        Buf[1] = 0x05DC0025;
        Buf[2] = 0x05DC0025;
        break;
    case 337: /* Zera The Mant */
        Buf[0] = 0x012C001C;
        Buf[1] = 7;
        Buf[2] = 7;
        break;
    case 248: /* Super War Lion */
        Buf[0] = 0x00F9001C;
        Buf[1] = 5;
        Buf[2] = 5;
        break;
    case 338: /* Fiends Mirror */
        Buf[0] = 0x0149001C;
        Buf[1] = 0x0121001C;
        Buf[2] = 7;
        break;
    case 2: /* Blue Eyes Ultimate Dragon */
        Buf[0] = 28;
        Buf[1] = 28;
        Buf[2] = 28;
        break;
    case 195: /* Performance of Sword */
        Buf[0] = 0x0236001C;
        Buf[1] = 6;
        Buf[2] = 3;
        break;
    case 147: /* Hungry Burger */
        Buf[0] = 0x00D2001C;
        Buf[1] = 0x0150001C;
        Buf[2] = 0x0295001C;
        break;
    case 220: /* Sengenjin */
        Buf[0] = 0x0279001C;
        Buf[1] = 4;
        Buf[2] = 4;
        break;
    case 135: /* Skull Guardian */
        Buf[0] = 0x0023001C;
        Buf[1] = 3;
        Buf[2] = 3;
        break;
    case 10: /* Tri Horned Dragon */
        Buf[0] = 0x0005001C;
        Buf[1] = 0x0125001C;
        Buf[2] = 0x0006001C;
        break;
    case 11: /* Serpent Night Dragon */
        Buf[0] = 0x0008001C;
        Buf[1] = 3;
        Buf[2] = 6;
        break;
    case 596: /* Crab Turtle */
        Buf[0] = 0x0248001C;
        Buf[1] = 16;
        Buf[2] = 16;
        break;
    case 77: /* Mask of Shine Dark */
        Buf[0] = 0x0131001C;
        Buf[1] = 1;
        Buf[2] = 1;
        break;
    case 352: /* Chakra */
        Buf[0] = 0x014A001C;
        Buf[1] = 0x03E80025;
        Buf[2] = 0x03E80025;
        break;
    case 353: /* Psycho Puppet */
        Buf[0] = 0x00AD001C;
        Buf[1] = 14;
        Buf[2] = 14;
        break;
    case 422: /* Javelin Beetle */
        Buf[0] = 0x018E001C;
        Buf[1] = 9;
        Buf[2] = 9;
        break;
    case 148: /* Garma Sword */
        Buf[0] = 0x0092001C;
        Buf[1] = 3;
        Buf[2] = 3;
        break;
    case 76: /* Cosmo Queen */
        Buf[0] = 0x029B001C;
        Buf[1] = 0x00CC001C;
        Buf[2] = 0x004F001C;
        break;
    case 128: /* Dokurorider */
        Buf[0] = 2;
        Buf[1] = 14;
        Buf[2] = 9;
        break;
    case 472: /* Fortress Whale */
        Buf[0] = 0x0214001C;
        Buf[1] = 12;
        Buf[2] = 12;
        break;
    case 78: /* Magician of Black Chaos */
        Buf[0] = 0x003C001C;
        Buf[1] = 0x05DC0025;
        Buf[2] = 0x05DC0025;
        break;
    default:
        Buf[0] = -1;
        Buf[1] = -1;
        Buf[2] = -1;
        break;
    }
}
