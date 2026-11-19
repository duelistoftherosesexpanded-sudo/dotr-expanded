/* Evidence: docs/modules/game-modes.md. Packet fields and prototypes provisional. */
#include "dotr/ee/movie_start.h"

extern int func_00186120(int);
extern int func_00184e10(PacketQuadword *, short, short, short, int, int);
extern int func_00184ff8(PacketQuadword *, int, int, int, int, int, int, int, int, int, int);
extern void func_0017bb60(int);
extern int func_00185578(int, int);
extern void func_00186470(int, PacketQuadword *);

void SzGame_ClearGsMem(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    unsigned char packet[320];
    int channel = func_00186120(2);
    PacketQuadword *p = (PacketQuadword *)packet;
    p->words[0] = 0x10000010;
    p->words[1] = 0;
    p->words[2] = 0;
    p->words[3] = 0;
    p++;
    p->halves[0] = 0x1000000000000008ULL;
    p->halves[1] = 14;
    p++;
    p += func_00184e10(p, (short)arg0, (short)arg4, (short)arg5, 0, 0);
    p->halves[0] = 0x1000000000008006ULL;
    p->halves[1] = 14;
    p++;
    p += func_00184ff8(p, 2, 0, 0, 0xfff, 0xfff, arg1, arg2, arg3, 0, 0);
    p->words[0] = 0x70000000;
    p->words[1] = 0;
    p->words[2] = 0;
    p->words[3] = 0;
    func_0017bb60(0);
    func_00185578(0, 0);
    func_00186470(channel, (PacketQuadword *)packet);
    func_00185578(0, 0);
}
