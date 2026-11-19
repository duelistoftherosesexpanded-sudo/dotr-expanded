#include "dotr/ee/shslot.h"

/* ShSlot_Flear (0x280060): for each of count flash objects whose ShFx timer runs, grows the
 * radius by the speed and draws a full circle as divisions * 4 triangles (divisions capped
 * at 9, 90 / divisions degrees each) from the centre colour to the rim colour; z 0xfffffe
 * and depth 0 with flags bit 0x80, else z 0xfffff8 and depth -1. The centre alpha fades in
 * over the first 8 frames and out over the last 8 unless flags bit 0x40 is set. Each
 * triangle goes to ExSortPrim_Pipe on the packet cursor at 0x351228. Returns 1.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
int ShSlot_Flear(void *objects, int count)
{
    struct DotrShSlotFlash *particle = objects;
    struct DotrShSlotFloats constants DOTR_SHSLOT_UNUSED = data_002b8760_shslot; /* copied, never read */
    struct DotrShSlotFlearPacket packet;
    float depth;
    float previousX, previousY;
    float offsetX, offsetY;
    float angleStep;
    int j;
    unsigned char *header;
    int divisions;
    int i;
    unsigned char remaining;

    for (i = 0; i < count; i++) {
        if (i != 0) {
            particle++;
        }
        header = (unsigned char *)particle;
        if (ShFx_isTimer(header) != 0) {
            particle->radius += particle->speed;
            if (header[7] > 9) {
                divisions = 9;
            } else {
                divisions = header[7];
            }
            angleStep = 90.0f / (float)divisions;
            if (header[4] & 0x80) {
                packet.vertex[0].position.bits.z = packet.vertex[1].position.bits.z = packet.vertex[2].position.bits.z = 0xFFFFFE;
                depth = 0.0f;
            } else {
                packet.vertex[0].position.bits.z = packet.vertex[1].position.bits.z = packet.vertex[2].position.bits.z = 0xFFFFF8;
                depth = -1.0f;
            }
            ((unsigned short *)&packet)[1] = 0x13;
            ((unsigned short *)&packet)[0] = 6;
            ((unsigned long long *)&packet)[1] = 0x414141;
            ((unsigned int *)&packet)[1] = 0;
            packet.vertex[0].r = particle->center_r;
            packet.vertex[1].r = packet.vertex[2].r = particle->r;
            packet.vertex[0].g = particle->center_g;
            packet.vertex[1].g = packet.vertex[2].g = particle->g;
            packet.vertex[0].b = particle->center_b;
            packet.vertex[1].b = packet.vertex[2].b = particle->b;
            packet.vertex[0].q = packet.vertex[1].q = packet.vertex[2].q = 1.0f;
            ExAssignAlpha(&packet, 0x48);
            packet.vertex[0].a = particle->alpha;
            packet.vertex[1].a = packet.vertex[2].a = 0;
            if (!(header[4] & 0x40)) {
                if (header[0] < 8) {
                    packet.vertex[0].a = (unsigned char)((particle->alpha >> 3) * header[0]);
                } else {
                    remaining = (unsigned char)(header[2] - header[0]);
                    if (remaining < 8) {
                        packet.vertex[0].a = (unsigned char)((particle->alpha >> 3) * remaining);
                    }
                }
            }
            offsetX = particle->radius * func_001a6898(0.0f);
            offsetY = particle->radius * func_001a6a88(0.0f);
            packet.vertex[0].position.bits.adc = packet.vertex[1].position.bits.adc = packet.vertex[2].position.bits.adc = 1;
            for (j = 1; j < divisions * 4 + 1; j++) {
                previousX = offsetX;
                previousY = offsetY;
                offsetX = particle->radius * func_001a6898((3.1415927f * (angleStep * (float)j)) / 180.0f);
                offsetY = particle->radius * func_001a6a88((3.1415927f * (angleStep * (float)j)) / 180.0f);
                packet.vertex[0].position.coordinates.x = (unsigned short)(func_001a22c0(particle->x) << 4);
                packet.vertex[0].position.coordinates.y = (unsigned short)(func_001a22c0(particle->y) << 4);
                packet.vertex[1].position.coordinates.x = (unsigned short)(func_001a22c0(particle->x + previousX) << 4);
                packet.vertex[1].position.coordinates.y = (unsigned short)(func_001a22c0(particle->y + previousY) << 4);
                packet.vertex[2].position.coordinates.x = (unsigned short)(func_001a22c0(particle->x + offsetX) << 4);
                packet.vertex[2].position.coordinates.y = (unsigned short)(func_001a22c0(particle->y + offsetY) << 4);
                data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &packet, 0, func_001a23b0(depth));
            }
        }
    }
    return 1;
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif
