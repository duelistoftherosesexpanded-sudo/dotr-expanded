#include "dotr/ee/sound_file.h"

/* Between func_001843e0 and func_00184428: marks the current 314944-byte
 * item with state 2, counts it and advances the index modulo the capacity. */
void voBufIncCount(struct DotrSoundBigQueue *p)
{
    func_001843e0();
    p->items[p->index].state = 2;
    p->count++;
    p->index = (p->index + 1) % p->capacity;
    func_00184428();
}
