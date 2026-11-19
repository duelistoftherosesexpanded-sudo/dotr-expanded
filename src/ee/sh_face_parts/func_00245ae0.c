#include "dotr/ee/sh_face_parts.h"

/* 0x245ae0. The face index table is gpShFaceIndex_shface, declared inside the body. */

void ShAnime_GetAnmDat(int arg0, int arg1, int arg2, int arg3, void *arg4) {
    extern DotrShFaceQuad **gpShFaceIndex_shface[];
    DotrShFaceAnmRecord *table;
    unsigned char var_a1;
    unsigned char temp_a0;
    DotrShFaceAnmRecord *records;
    int var_t1;
    int count;

    var_t1 = 0;
    var_a1 = ((DotrShFaceAnmData *)gpShFaceIndex_shface[arg0 & 0xFF])->indices[arg1 & 0xFF][1];
    if (arg2 != 0) {
        records = ((DotrShFaceAnmData *)gpShFaceIndex_shface[arg0 & 0xFF])->u.records;
        while (var_t1 != (arg2 & 0xFF)) {
            temp_a0 = *(unsigned char *)&records[var_a1 & 0xFF];
            if (temp_a0 == 0 || temp_a0 == 0xFF) {
                break;
            }
            var_t1 += 1;
            var_a1 = temp_a0 & 0xFF;
        }
    }
    var_t1 = ((DotrShFaceAnmData *)gpShFaceIndex_shface[arg0 & 0xFF])->u.links[var_a1 & 0xFF][6];
    table = ((DotrShFaceAnmData *)gpShFaceIndex_shface[arg0 & 0xFF])->u.records;
    count = 0;
    if (arg3 != 0) {
        while (count != (arg3 & 0xFF)) {
            temp_a0 = *(unsigned char *)&table[var_t1 & 0xFF];
            if (temp_a0 == 0 || temp_a0 == 0xFF) {
                break;
            }
            count += 1;
            var_t1 = temp_a0 & 0xFF;
        }
    }
    *(DotrShFaceAnmRecord *)arg4 = table[var_t1 & 0xFF];
}
