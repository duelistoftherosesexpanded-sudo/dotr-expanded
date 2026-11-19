#include "dotr/ee/sh_face_parts.h"

/* 0x245a30. The face index table is gpShFaceIndex_shface, declared inside the body. */

void ShAnime_GetFixInfo(int arg0, int arg1, int arg2, void *arg3) {
    extern DotrShFaceQuad **gpShFaceIndex_shface[];
    unsigned char var_a1;
    unsigned char temp_a0;
    DotrShFaceFixRecord *records;
    int var_t1;

    var_t1 = 0;
    var_a1 = ((DotrShFaceFixData *)gpShFaceIndex_shface[arg0 & 0xFF])->indices[arg1 & 0xFF][1];
    if (arg2 != 0) {
        records = ((DotrShFaceFixData *)gpShFaceIndex_shface[arg0 & 0xFF])->records;
        while (var_t1 != (arg2 & 0xFF)) {
            temp_a0 = *(unsigned char *)&records[var_a1 & 0xFF];
            if (temp_a0 == 0 || temp_a0 == 0xFF) {
                break;
            }
            var_t1 += 1;
            var_a1 = temp_a0 & 0xFF;
        }
    }
    *(DotrShFaceFixRecord *)arg3 = ((DotrShFaceFixData *)gpShFaceIndex_shface[arg0 & 0xFF])->records[var_a1 & 0xFF];
}
