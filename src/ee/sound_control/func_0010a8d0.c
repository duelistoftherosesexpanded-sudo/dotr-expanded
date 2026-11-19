#include "dotr/ee/sound_control.h"

/* Both helpers are assumed inline forms: the item search and the split read
 * appear twice each in the observed code; the original helpers are unknown. */
static inline int dotr_sound_bgm_find_item(int id)
{
    int i;
    for (i = 0; i < 256; i++) {
        if (g_SDValue[0]->items[i].unknown_004 == id) {
            return i;
        }
    }
    return -1;
}

/* The second request names the text through the manager's address plus 0x1210:
 * with the member spelling the compiler keeps the manager in another register. */
static inline void dotr_sound_bgm_read(int id, int position, int length)
{
    int rest;
    int first;
    int pos;

    rest = length;
    pos = position;
    func_0019b648((char *)g_SDValue[0]->unknown_1210, data_002b90a0, id);
    if (g_SDValue[0]->unknown_01d0 + length >= 0x100000) {
        first = 0x100000 - g_SDValue[0]->unknown_01d0;
        SD_AddReadToEeReq((char *)g_SDValue[0]->unknown_1210,
                          (int)(DotrSoundControlWord)(g_SDBuffer[0] + g_SDValue[0]->unknown_01d0 + 0x800),
                          pos + 0x800, first);
        rest -= first;
        pos += first;
        g_SDValue[0]->unknown_01d0 = 0;
    }
    if (rest) {
        SD_AddReadToEeReq((char *)g_SDValue[0] + 0x1210,
                          (int)(DotrSoundControlWord)(g_SDBuffer[0] + g_SDValue[0]->unknown_01d0 + 0x800),
                          pos + 0x800, rest);
        g_SDValue[0]->unknown_01d0 += rest;
    }
}

void _vLoadBGMFileToEE(int id, int position, int length)
{
    int index;
    int size;
    int loop_start;
    int loop_end;
    int over;
    DotrSoundQuad *p;

    index = dotr_sound_bgm_find_item(id);
    if (index < 0) {
        return;
    }
    loop_start = g_SDValue[0]->items[index].unknown_008 << 2;
    loop_end = g_SDValue[0]->items[index].unknown_00c << 2;
    size = g_SDValue[0]->items[index].unknown_000;
    size &= ~15;
    loop_start &= ~15;
    loop_end &= ~15;
    if (position >= size) {
        return;
    }
    func_0019b648((char *)g_SDValue[0]->unknown_1210, data_002b90a0, id);
    if (loop_end) {
        while (position + length > loop_end) {
            int part = loop_end - position;
            dotr_sound_bgm_read(id, position, part);
            length -= part;
            position = loop_start;
        }
    } else if (size < position + length) {
        over = position + length - size;
        length -= over;
        if (over > 0x20000) {
            over = 0x20000;
        }
        p = (DotrSoundQuad *)(g_SDBuffer[0] + 0x800);
        over >>= 4;
        p = (DotrSoundQuad *)((char *)p + (g_SDValue[0]->unknown_01d0 + length));
        for (; over; over--) {
            *p++ = 0;
            if ((char *)p >= g_SDBuffer[0] + 0x100800) {
                p = (DotrSoundQuad *)((char *)p - 0x100000);
            }
        }
    }
    dotr_sound_bgm_read(id, position, length);
    position += length;
    g_SDValue[0]->unknown_01cc = position;
}
