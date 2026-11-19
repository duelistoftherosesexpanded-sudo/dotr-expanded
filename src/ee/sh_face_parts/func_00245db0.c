#include "dotr/ee/sh_face_parts.h"

/* 0x245db0. The VIF cursor is data_00350fc0_shface, a DotrShFaceWord declared inside the body. */

void ShFace_Prim(void *arg0, void *arg1, int arg2, int arg3) {
    extern DotrShFaceWord data_00350fc0_shface;
    DotrShFaceObject *object = arg0;
    unsigned short *rect = arg1;
    unsigned int shift;
    unsigned short y;
    DotrShFacePacket packet __attribute__((aligned(8)));

    ((DotrShFaceTag *)packet.tag)->flags = 0x24;
    ((DotrShFaceTag *)packet.tag)->count = 9;
    ((DotrShFaceTag *)packet.tag)->registers = 0x424242421ULL;
    ((DotrShFaceTag *)packet.tag)->padding = 0;
    shift = 4;
    packet.r = packet.g = packet.b = packet.a = 0x80;
    packet.q = 1.0f;
    y = object->y + 0x720;
    packet.vertex[2].xyz.x = packet.vertex[0].xyz.x =
        ((object->x + 0x6C0) << 4) + (arg2 << shift);
    packet.vertex[1].xyz.y = packet.vertex[0].xyz.y = (y << 4) + (arg3 << shift);
    packet.vertex[3].xyz.x = packet.vertex[1].xyz.x =
        packet.vertex[0].xyz.x + ((rect[3] - rect[1]) << shift);
    packet.vertex[3].xyz.y = packet.vertex[2].xyz.y =
        packet.vertex[0].xyz.y + ((rect[2] - rect[0]) << shift);
    packet.vertex[0].xyz.z = packet.vertex[1].xyz.z = packet.vertex[2].xyz.z =
        packet.vertex[3].xyz.z = 0xFFFFF8;
    packet.vertex[0].xyz.fog = packet.vertex[1].xyz.fog = packet.vertex[2].xyz.fog =
        packet.vertex[3].xyz.fog = 0;
    ((DotrShFaceTag *)packet.tag)->flags |= 0x800;
    ((DotrShFaceUv *)&packet.vertex[0].uv)->u = rect[1] << 4;
    ((DotrShFaceUv *)&packet.vertex[0].uv)->v = rect[0] << 4;
    ((DotrShFaceUv *)&packet.vertex[1].uv)->u = rect[3] << 4;
    ((DotrShFaceUv *)&packet.vertex[1].uv)->v = rect[0] << 4;
    ((DotrShFaceUv *)&packet.vertex[2].uv)->u = rect[1] << 4;
    ((DotrShFaceUv *)&packet.vertex[2].uv)->v = rect[2] << 4;
    ((DotrShFaceUv *)&packet.vertex[3].uv)->u = rect[3] << 4;
    ((DotrShFaceUv *)&packet.vertex[3].uv)->v = rect[2] << 4;
    ExAssignTexture(packet.tag, object->texture, 1);
    ExAssignClamp(packet.tag, ((unsigned long long)rect[1] << 4) | 0xA | ((unsigned long long)rect[3] << 14) |
                                  ((unsigned long long)rect[0] << 24) | ((unsigned long long)rect[2] << 34));
    ExAssignAlpha(packet.tag, 0x44);
    if (data_00350fc0_shface != 0) {
        data_00350fc0_shface = ExSortPrim_Pipe(data_00350fc0_shface, (DotrShFaceWord)packet.tag, 0, 0);
    }
}
