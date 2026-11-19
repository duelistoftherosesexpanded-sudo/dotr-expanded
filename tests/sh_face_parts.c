/* Synthetic host tests for the sh_face_parts group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/sh_face_parts.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

DotrShFaceQuad **gpShFaceIndex_shface[2];
DotrShFaceWord data_00350fc0_shface;
static int sorts, textures;

int ExAssignTexture(void *packet, void *texture, int a) { textures++; return 0; }
int ExAssignClamp(void *packet, long long value) { return 0; }
int ExAssignAlpha(void *packet, long long value) { return 0; }
DotrShFaceWord ExSortPrim_Pipe(DotrShFaceWord cursor, DotrShFaceWord packet, int a2, int a3)
{
    sorts++;
    return cursor + 1;
}

int main(void)
{
    /* Resource 1: object 0's index row starts at record 2; record 2 links to 3, record 3 ends
     * (link 0) and its byte 6 heads the animation chain at record 1. */
    static unsigned char rows[1][8] = {{0, 2}};
    static DotrShFaceFixRecord records[4];
    static DotrShFaceFixData data;
    DotrShFaceFixRecord out;
    DotrShFaceAnmRecord frame;
    static DotrShFaceObject object;
    static unsigned short rect[4] = {0, 0, 16, 16};

    memset(records, 0, sizeof records);
    records[2].x = 3;
    records[3].width = 77;
    ((unsigned char *)&records[3])[6] = 1;
    records[1].width = 55;
    data.indices = rows;
    data.records = records;
    gpShFaceIndex_shface[1] = (DotrShFaceQuad **)&data;

    /* Two steps from record 2: 2 -> 3, then 3 ends the chain. */
    ShAnime_GetFixInfo(1, 0, 2, &out);
    assert(out.width == 77);
    /* Animation: chain ends on record 3, its byte 6 gives record 1, no frames advanced. */
    ShAnime_GetAnmDat(1, 0, 2, 0, &frame);
    assert(frame.width == 55);

    /* ShFace_Prim sorts the packet when the cursor is set. */
    data_00350fc0_shface = 100;
    ShFace_Prim(&object, rect, 0, 0);
    assert(sorts == 1 && textures == 1 && data_00350fc0_shface == 101);
    return 0;
}
