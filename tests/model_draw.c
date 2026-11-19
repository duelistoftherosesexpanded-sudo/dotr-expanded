/* Synthetic host tests for the model_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/model_draw.h"

struct DotrModelPad dotr_model_pad;

/* Recording stubs: every call is logged with its pointer/integer and float arguments. */
struct Call {
    char kind;
    void *p[5];
    int i[2];
    float f[4];
};
static struct Call calls[16];
static int call_count;
static DotrModelQuad block[32];
static DotrModelQuad work[64];
static DotrModelQuad zbuffer_out[2];

static struct Call *record(char kind)
{
    struct Call *call = &calls[call_count++];
    assert(call_count <= 16);
    memset(call, 0, sizeof(*call));
    call->kind = kind;
    return call;
}

void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    struct Call *call = record('C');
    call->p[0] = destination; call->p[1] = source; call->i[0] = count;
    return destination;
}
void lRhModel_DynamicAddressBind(struct DotrPatchHead *b, unsigned char *first, unsigned char *second, int flag)
{
    struct Call *call = record('B');
    call->p[0] = b; call->p[1] = first; call->p[2] = second; call->i[0] = flag;
}
DotrModelAddress RhBase_SetZBufferMode(DotrModelAddress cursor, int a)
{
    struct Call *call = record('Z');
    call->p[0] = (void *)cursor; call->i[0] = a;
    return (DotrModelAddress)&zbuffer_out[a];
}
void lRhModel_SetupTexture(struct DotrPatchHead *head, unsigned char *first, unsigned char *second, int index)
{
    struct Call *call = record('T');
    call->p[0] = head; call->p[1] = first; call->p[2] = second; call->i[0] = index;
}
void lRhModel_SetupOptionObject(struct DotrPatchHead *b, unsigned char *first, unsigned char *second, int flag, int e)
{
    struct Call *call = record('O');
    call->p[0] = b; call->p[1] = first; call->p[2] = second; call->i[0] = flag; call->i[1] = e;
}
void lRhModel_SetMaterialIntensity(unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *d,
                                   struct DotrModelDraw *draw)
{
    struct Call *call = record('I');
    call->p[0] = a; call->p[1] = b; call->p[2] = c; call->p[3] = d; call->p[4] = draw;
}
void lRhModel_SetupNoMotion(struct DotrPatchHead *b)
{
    record('N')->p[0] = b;
}
void lRhModel_SetupMultiMotion(struct DotrPatchHead *b, int a0, int a1, float f0, float f1, float f2, float f3)
{
    struct Call *call = record('M');
    call->p[0] = b; call->i[0] = a0; call->i[1] = a1;
    call->f[0] = f0; call->f[1] = f1; call->f[2] = f2; call->f[3] = f3;
}
void lRhModel_SetupFloatMotion(struct DotrPatchHead *b, int a0, float f)
{
    struct Call *call = record('F');
    call->p[0] = b; call->i[0] = a0; call->f[0] = f;
}
void lRhModel_SetupPulseMotion(struct DotrPatchHead *b, int a0, float f)
{
    struct Call *call = record('P');
    call->p[0] = b; call->i[0] = a0; call->f[0] = f;
}

enum { MOTION = 1, MULTI = 2, FLOATING = 4, OPTION = 8, TEXTURE = 16, INTENSITY = 32, LIGHT_0 = 64, LIGHT_1 = 128, ZBUFFER = 256 };

/* 0x20faf0: branch coverage. One run per outcome of each test: the 16144/16145 bound, a second
 * stretch of size 0 and 16, the z-buffer bit, the texture and option bits, each of the three
 * bits that select the intensity call (and none), and the four motion paths. */
static void check_draw(int flags, int count, int skip, int second_size)
{
    struct DotrModelHead head;
    struct DotrModelDraw draw;
    struct DotrModelPad *pad = &dotr_model_pad;
    unsigned char *first = (unsigned char *)work;
    unsigned char *second = first + 0x60;
    unsigned char *third;
    unsigned char *fourth;
    struct DotrPatchHead *b = (struct DotrPatchHead *)block;
    int n = 0;

    memset(&head, 0, sizeof(head));
    memset(&draw, 0, sizeof(draw));
    memset(pad, 0, sizeof(*pad));
    memset(work, 0, sizeof(work));
    call_count = 0;
    head.first_offset = 0x20;
    head.first_size = 0x60;
    head.second_offset = 0x90;
    head.second_size = second_size;
    head.unknown_54 = 2.5f;
    head.unknown_58 = count;
    block[2] = ((DotrModelQuad)0x1122334455667788 << 64) | 0x99aabbccddeeff00;
    pad->first = first;
    pad->skip = skip;
    pad->head = &head;
    pad->unknown_b0 = 99;
    pad->third = first;
    pad->fourth = first;
    draw.block = b;
    draw.motion = (flags & MOTION) != 0;
    draw.multi = (flags & MULTI) != 0;
    draw.floating = (flags & FLOATING) != 0;
    draw.option = (flags & OPTION) != 0;
    draw.texture = (flags & TEXTURE) != 0;
    draw.intensity = (flags & INTENSITY) != 0;
    draw.light_0 = (flags & LIGHT_0) != 0;
    draw.light_1 = (flags & LIGHT_1) != 0;
    draw.zbuffer = (flags & ZBUFFER) != 0;
    draw.unknown_08 = 0x100;
    draw.unknown_0c = 1.5f;
    draw.unknown_10 = 0x200;
    draw.unknown_14 = 3.5f;
    draw.unknown_18 = 4.5f;
    draw.unknown_1c = 5.5f;
    draw.unknown_20 = 0x33;
    draw.unknown_24 = 0x44;
    draw.unknown_28 = 0x55;

    lRhModel_DrawCommonPart(&draw);

    assert(pad->unknown_b4 == (skip + count * 256 > 16144 ? first : pad->buffer));
    assert(work[0] == block[2]);
    assert(calls[n].kind == 'C' && calls[n].p[0] == first + skip && calls[n].i[0] == (0x60 - skip) >> 4);
    assert(calls[n++].p[1] == (unsigned char *)block + 0x20 + skip);
    assert(calls[n].kind == 'B' && calls[n].p[0] == b && calls[n].p[1] == first && calls[n].p[2] == first);
    assert(calls[n++].i[0] == 0);
    assert((unsigned char *)pad->second == second);
    if (second_size > 0) {
        if (flags & ZBUFFER) {
            assert(calls[n].kind == 'Z' && calls[n].p[0] == second && calls[n++].i[0] == 0);
            third = (unsigned char *)&zbuffer_out[0];
        } else {
            assert(pad->second[0] == 0x20000000 && pad->second[2] == 0 && pad->second[3] == 0);
            assert(pad->second[1] == (DotrModelAddress)second + second_size + 16);
            third = (unsigned char *)(pad->second + 4);
        }
        assert(calls[n].kind == 'C' && calls[n].p[0] == third && calls[n].p[1] == (unsigned char *)block + 0x90);
        assert(calls[n++].i[0] == second_size >> 4);
        assert(calls[n].kind == 'B' && calls[n].p[0] == b && calls[n].p[1] == first && calls[n].p[2] == third);
        assert(calls[n++].i[0] == 1);
        fourth = third + second_size;
        if (flags & ZBUFFER) {
            assert(calls[n].kind == 'Z' && calls[n].p[0] == fourth && calls[n++].i[0] == 1);
            fourth = (unsigned char *)&zbuffer_out[1];
        }
    } else {
        third = NULL;
        fourth = NULL;
    }
    assert(pad->third == third && pad->fourth == fourth);
    assert(calls[n].kind == 'T' && calls[n].p[0] == b && calls[n].p[1] == first && calls[n].p[2] == third);
    assert(calls[n++].i[0] == ((flags & TEXTURE) ? 0x44 : -1));
    assert(calls[n].kind == 'O' && calls[n].p[0] == b && calls[n].p[1] == first && calls[n].p[2] == third);
    assert(calls[n].i[0] == ((flags & OPTION) ? 0 : 1) && calls[n++].i[1] == 0x33);
    if (flags & (INTENSITY | LIGHT_0 | LIGHT_1)) {
        assert(calls[n].kind == 'I' && calls[n].p[0] == first && calls[n].p[1] == first + 0x60);
        assert(calls[n].p[2] == third && calls[n].p[3] == third + second_size && calls[n++].p[4] == &draw);
    }
    assert(pad->unknown_b8 == 0x55);
    if (!(flags & MOTION)) {
        assert(calls[n].kind == 'N' && calls[n++].p[0] == b);
        assert(pad->unknown_b0 == 99 && pad->unknown_bc == 0.0f);
    } else {
        assert(pad->unknown_b0 == 0 && pad->unknown_bc == 2.5f);
        if (flags & MULTI) {
            assert(calls[n].kind == 'M' && calls[n].i[0] == 0x10c && calls[n].i[1] == 0x20c);
            assert(calls[n].f[1] == 3.5f && calls[n].f[2] == 4.5f && calls[n].f[3] == 5.5f);
        } else if (flags & FLOATING) {
            assert(calls[n].kind == 'F' && calls[n].i[0] == 0x10c);
        } else {
            assert(calls[n].kind == 'P' && calls[n].i[0] == 0x10c);
        }
        assert(calls[n].p[0] == b && calls[n++].f[0] == 1.5f);
    }
    assert(n == call_count);
}

/* Stubs and checks for RhModel_Draw (0x20f4e0). */
int require_coordinate_index;
static int light_result, volume_result;
static float light_range, light_distance;

DotrModelAddress RhVu1Mac_SetFogOnOff(DotrModelAddress cursor, int a, int b)
{
    struct Call *call = record('G');
    call->p[0] = (void *)cursor; call->i[0] = a; call->i[1] = b;
    return cursor + 0x10;
}
void RhGeom_PushMatrix(int unknown)
{
    record('U')->i[0] = unknown;
}
void RhGeom_PopMatrix(void)
{
    record('O');
}
void RhGeom_MulMatrixVector(struct DotrModelVector *out, void *m, struct DotrModelVector *v)
{
    struct Call *call = record('X');
    call->p[0] = out; call->p[1] = m; call->p[2] = v;
}
int RhView_GetSampleZ(struct DotrModelVector *in, float dz)
{
    struct Call *call = record('Z');
    call->p[0] = in; call->f[0] = dz;
    return 77;
}
int RhVu1Mac_GetPointLight(struct DotrModelVector *a, struct DotrModelVector *b, float *c, float *d, float *e)
{
    struct Call *call = record('L');
    call->p[0] = a; call->p[1] = b; call->p[2] = c; call->p[3] = d; call->p[4] = e;
    *d = light_range;
    return light_result;
}
float RhGeom_GetPointsDistance(struct DotrModelVector *a, struct DotrModelVector *b)
{
    struct Call *call = record('D');
    call->p[0] = a; call->p[1] = b;
    return light_distance;
}
int RhView_CheckViewVolume(struct DotrModelVector *centre, float radius)
{
    struct Call *call = record('V');
    call->p[0] = centre; call->f[0] = radius;
    return volume_result;
}
DotrModelAddress lRhModel_DrawNormalMethod(struct DotrModelDraw *draw, int lit)
{
    struct Call *call = record('N');
    call->p[0] = draw; call->i[0] = lit;
    return 0x5000;
}
DotrModelAddress lRhModel_DrawFastMethod(struct DotrModelDraw *draw)
{
    record('F')->p[0] = draw;
    return 0x6000;
}

enum { M_LIGHT = 1, M_FOG = 2, M_MOTION = 4, M_BIT10 = 8, M_OWN = 16 };

/* Runs one call and checks the call order (one letter per stub), the result and what every
 * path stores; the arguments are checked per call kind. */
static void check_model(int flags, int second_size, const char *want, DotrModelAddress result, int lit)
{
    struct DotrModelDraw draw;
    struct DotrModelPad *pad = &dotr_model_pad;
    struct DotrModelHead *head = (struct DotrModelHead *)((unsigned char *)block + 16);
    struct DotrModelNode *node = (struct DotrModelNode *)((unsigned char *)block + 0x100);
    int both = (flags & M_FOG) && (flags & M_BIT10);
    DotrModelAddress cursor = 0x1000;
    int i;

    memset(block, 0, sizeof(block));
    memset(&draw, 0, sizeof(draw));
    memset(pad, 0, sizeof(*pad));
    call_count = 0;
    head->unknown_00[0] = 0x100;
    head->first_offset = 0x80;
    head->second_size = second_size;
    ((unsigned int *)((unsigned char *)block + 0x80))[1] = 0xf0000123;
    node->sphere.x = 1.0f; node->sphere.y = 2.0f; node->sphere.z = 3.0f; node->sphere.w = 4.0f;
    draw.block = (struct DotrPatchHead *)block;
    draw.bit_0 = (flags & M_LIGHT) != 0;
    draw.bit_1 = (flags & M_FOG) != 0;
    draw.motion = (flags & M_MOTION) != 0;
    draw.bit_10 = (flags & M_BIT10) != 0;
    draw.own_matrix = (flags & M_OWN) != 0;
    require_coordinate_index = 5;
    assert(RhModel_Draw(cursor, &draw) == result);
    assert((int)strlen(want) == call_count);
    for (i = 0; i < call_count; i++) {
        struct Call *call = &calls[i];
        assert(call->kind == want[i]);
        switch (call->kind) {
        case 'G':
            if (i == 0) {
                assert(call->p[0] == (void *)cursor && call->i[0] == draw.bit_1 && call->i[1] == draw.bit_10);
                cursor += 0x10;
            } else {
                assert(call->i[0] == 1 && call->i[1] == 1);
            }
            break;
        case 'U': assert(call->i[0] == 0); break;
        case 'C': assert(call->p[0] == &pad->work && call->p[1] == &draw.matrix && call->i[0] == 1); break;
        case 'X': assert(call->p[0] == &pad->work && call->p[1] == 0 && call->p[2] == &pad->centre); break;
        case 'Z': assert(call->p[0] == &pad->work && call->f[0] == 4.0f); break;
        case 'L':
            assert(call->p[0] == 0 && call->p[1] == &pad->light_at && call->p[2] == &pad->light[0]);
            assert(call->p[3] == &pad->light[1] && call->p[4] == &pad->light[2]);
            break;
        case 'D': assert(call->p[0] == &pad->light_at && call->p[1] == &pad->work); break;
        case 'V': assert(call->p[0] == &pad->centre && call->f[0] == 4.0f); break;
        case 'N': assert(call->p[0] == &draw && call->i[0] == lit); break;
        case 'F': assert(call->p[0] == &draw); break;
        }
    }
    assert((want[0] == 'G') == !both && (want[call_count - 1] == 'G') == !both);
    assert(pad->unknown_c0 == (int)cursor && pad->head == head && pad->skip == 0x123);
    assert(pad->centre.x == 1.0f && pad->centre.y == 2.0f && pad->centre.z == 3.0f && pad->centre.w == 1.0f);
    assert(draw.sample_z == 77 && require_coordinate_index == 0);
    assert(pad->unknown_b0 == (strchr(want, 'V') ? volume_result : 0));
}

/* Branch coverage: each fog-flag outcome, both matrix paths, the light conditions with the
 * distance at and just past the limit, and each outcome of the view-volume test. */
static void check_models(void)
{
    light_result = 0; light_range = 6.0f; light_distance = 10.0f; volume_result = 1;
    check_model(M_FOG | M_BIT10 | M_OWN | M_MOTION, 0, "UCZNO", 0x5000, 0);
    check_model(M_BIT10, 0, "GUXZVFOG", 0x6010, 0);
    check_model(M_FOG, 0, "GUXZVFOG", 0x6010, 0);
    check_model(M_FOG | M_BIT10, 0, "UXZVFO", 0x6000, 0);
    check_model(M_FOG | M_BIT10, 1, "UXZVNO", 0x5000, 0);
    volume_result = 5;
    check_model(M_FOG | M_BIT10, 0, "UXZVNO", 0x5000, 0);
    volume_result = 4;
    check_model(M_FOG | M_BIT10, 0, "UXZVO", 0x1000, 0);
    check_model(0, 0, "GUXZVOG", 0x1020, 0);
    volume_result = 1;
    check_model(M_FOG | M_BIT10 | M_LIGHT, 0, "UXZLVFO", 0x6000, 0);
    light_result = 1;
    check_model(M_FOG | M_BIT10 | M_LIGHT, 0, "UXZLDVNO", 0x5000, 1);
    light_distance = 10.5f;
    check_model(M_FOG | M_BIT10 | M_LIGHT, 0, "UXZLDVFO", 0x6000, 0);
    light_distance = 9.0f;
    check_model(M_FOG | M_BIT10 | M_LIGHT | M_MOTION, 0, "UXZLDNO", 0x5000, 1);
}

int main(void)
{
    check_draw(0, 63, 16, 0);
    check_draw(0, 63, 17, 0);
    check_draw(TEXTURE | OPTION, 0, 0x20, 16);
    check_draw(ZBUFFER | INTENSITY, 0, 0x20, 16);
    check_draw(ZBUFFER | LIGHT_0 | MOTION, 100, 0, 0);
    check_draw(LIGHT_1 | MOTION | MULTI | FLOATING, 0, 0, 0x40);
    check_draw(MOTION | FLOATING, 0, 0, 0x40);
    check_draw(MULTI | FLOATING, 0, 0, 0x40);
    check_models();
    puts("model_draw: ok");
    return 0;
}
