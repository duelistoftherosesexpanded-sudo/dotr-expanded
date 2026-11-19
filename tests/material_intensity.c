/* Synthetic host tests for the material_intensity group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/material_intensity.h"

/* Quadword-aligned packet buffers. */
static union {
    unsigned long long d[64];
    float f[128];
    unsigned char b[512];
} list1, list2;

static union {
    float f[32];
    unsigned char b[128];
} mat;

/* Writes a lighting packet at offset off: tag 0x10000003 (three quadwords follow), the
 * matching second doubleword (or another one) and the nine colour floats from base. */
static void packet(unsigned char *buf, int off, int matching, float base, float second)
{
    unsigned long long tag = 0x10000003ULL;
    unsigned long long kind = matching ? 0x6c03003c10000000ULL : 0x6c03003c10000001ULL;
    float c[11];
    int i;

    memcpy(buf + off, &tag, 8);
    memcpy(buf + off + 8, &kind, 8);
    for (i = 0; i < 11; i++) {
        c[i] = base + (float)i;
    }
    c[3] = -99.0f; /* +0x1c is never touched */
    c[4] = second;
    memcpy(buf + off + 0x10, c, sizeof(c));
}

static float at(unsigned char *buf, int off)
{
    float v;

    memcpy(&v, buf + off, 4);
    return v;
}

static void tag(unsigned char *buf, int off, unsigned int word)
{
    memset(buf + off, 0, 16);
    memcpy(buf + off, &word, 4);
}

static void jump(unsigned char *buf, int off, unsigned char *to)
{
    tag(buf, off, 0x20000000u);
    memcpy(buf + off + 4, &to, sizeof(to));
}

static void material(int scale, int add, int replace)
{
    memset(&mat, 0, sizeof(mat));
    mat.b[4] = (unsigned char)(scale << 7);
    mat.b[5] = (unsigned char)(add | replace << 1);
    mat.f[0x30 / 4] = 2.0f;
    mat.f[0x34 / 4] = 0.5f;
    mat.f[0x38 / 4] = 3.0f;
    mat.f[0x3c / 4] = 10.0f;
    mat.f[0x40 / 4] = 20.0f;
    mat.f[0x44 / 4] = 30.0f;
    mat.f[0x48 / 4] = 40.0f;
}

static void test_replace(void)
{
    memset(&list1, 0, sizeof(list1));
    packet(list1.b, 0, 1, 1.0f, 5.0f);
    packet(list1.b, 0x40, 1, 1.0f, 0.0f);
    material(0, 0, 1);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x80, 0, 0, mat.b);
    assert(at(list1.b, 0x10) == 10.0f && at(list1.b, 0x14) == 20.0f && at(list1.b, 0x18) == 30.0f);
    assert(at(list1.b, 0x1c) == -99.0f);
    assert(at(list1.b, 0x20) == 10.0f && at(list1.b, 0x24) == 20.0f && at(list1.b, 0x28) == 30.0f);
    assert(at(list1.b, 0x2c) == 40.0f);
    assert(at(list1.b, 0x30) == 10.0f && at(list1.b, 0x34) == 20.0f && at(list1.b, 0x38) == 30.0f);
    /* A zero at +0x20 keeps the second colour. */
    assert(at(list1.b, 0x60) == 0.0f && at(list1.b, 0x64) == 6.0f && at(list1.b, 0x68) == 7.0f);
    assert(at(list1.b, 0x50) == 10.0f && at(list1.b, 0x6c) == 40.0f);
}

static void test_add_clamp(void)
{
    memset(&list1, 0, sizeof(list1));
    packet(list1.b, 0, 1, 240.0f, 250.0f);
    packet(list1.b, 0x40, 1, -100.0f, 0.0f);
    material(0, 1, 0);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x80, 0, 0, mat.b);
    /* 240 + 10 = 250 stays; 241 + 20, 242 + 30 clamp to 255. */
    assert(at(list1.b, 0x10) == 250.0f && at(list1.b, 0x14) == 255.0f && at(list1.b, 0x18) == 255.0f);
    assert(at(list1.b, 0x20) == 255.0f && at(list1.b, 0x2c) == 255.0f);
    /* Negative sums clamp to 0; with +0x20 zero only it is not added to. */
    assert(at(list1.b, 0x50) == 0.0f && at(list1.b, 0x54) == 0.0f && at(list1.b, 0x58) == 0.0f);
    assert(at(list1.b, 0x60) == 0.0f && at(list1.b, 0x64) == 0.0f);
    assert(at(list1.b, 0x6c) == 0.0f);
    assert(at(list1.b, 0x70) == 0.0f && at(list1.b, 0x78) == 0.0f);
    memset(&list1, 0, sizeof(list1));
    packet(list1.b, 0, 1, -100.0f, 4.0f);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x40, 0, 0, mat.b);
    /* -93 + 40 at +0x2c and -95 + 20 at +0x24 clamp to 0. */
    assert(at(list1.b, 0x20) == 14.0f && at(list1.b, 0x24) == 0.0f && at(list1.b, 0x2c) == 0.0f);
}

static void test_scale(void)
{
    memset(&list1, 0, sizeof(list1));
    packet(list1.b, 0, 1, 4.0f, 8.0f);
    packet(list1.b, 0x40, 1, 4.0f, 0.0f);
    material(1, 0, 0);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x80, 0, 0, mat.b);
    assert(at(list1.b, 0x10) == 2.0f && at(list1.b, 0x14) == 2.5f && at(list1.b, 0x18) == 3.0f);
    assert(at(list1.b, 0x20) == 16.0f && at(list1.b, 0x24) == 18.0f && at(list1.b, 0x28) == 20.0f);
    assert(at(list1.b, 0x2c) == 11.0f);
    assert(at(list1.b, 0x30) == 36.0f && at(list1.b, 0x34) == 39.0f && at(list1.b, 0x38) == 42.0f);
    /* +0x20 zero: no scaling at all. */
    assert(at(list1.b, 0x50) == 4.0f && at(list1.b, 0x70) == 12.0f);
}

static void test_walk(void)
{
    memset(&list1, 0, sizeof(list1));
    memset(&list2, 0, sizeof(list2));
    /* A non-matching packet (skipped by its length), a plain quadword and a jump to an
     * address above the end: the walk stops there without touching it. */
    packet(list1.b, 0, 0, 1.0f, 1.0f);
    tag(list1.b, 0x40, 0x30000000u);
    jump(list1.b, 0x50, list1.b + 0x100);
    packet(list1.b, 0x100, 1, 1.0f, 1.0f);
    material(0, 0, 1);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x60, 0, 0, mat.b);
    assert(at(list1.b, 0x10) == 1.0f);
    assert(at(list1.b, 0x110) == 1.0f);

    /* Jump within bounds: list1 continues at 0x100 of itself. */
    memset(&list1, 0, sizeof(list1));
    jump(list1.b, 0, list1.b + 0x100);
    packet(list1.b, 0x10, 1, 1.0f, 1.0f);
    packet(list1.b, 0x100, 1, 1.0f, 1.0f);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x140, 0, 0, mat.b);
    assert(at(list1.b, 0x20) == 1.0f);
    assert(at(list1.b, 0x110) == 10.0f);

    /* The second list is walked too. */
    memset(&list1, 0, sizeof(list1));
    memset(&list2, 0, sizeof(list2));
    packet(list1.b, 0, 1, 1.0f, 1.0f);
    packet(list2.b, 0, 1, 1.0f, 1.0f);
    tag(list2.b, 0x40, 0x30000000u);
    packet(list2.b, 0x50, 1, 1.0f, 1.0f);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x40, list2.b, list2.b + 0x90, mat.b);
    assert(at(list1.b, 0x10) == 10.0f && at(list2.b, 0x10) == 10.0f && at(list2.b, 0x60) == 10.0f);

    /* An unknown tag in the first list ends everything: the second list is not walked. */
    memset(&list1, 0, sizeof(list1));
    memset(&list2, 0, sizeof(list2));
    tag(list1.b, 0, 0x40000000u);
    packet(list1.b, 0x10, 1, 1.0f, 1.0f);
    packet(list2.b, 0, 1, 1.0f, 1.0f);
    lRhModel_SetMaterialIntensity(list1.b, list1.b + 0x50, list2.b, list2.b + 0x40, mat.b);
    assert(at(list1.b, 0x20) == 1.0f && at(list2.b, 0x10) == 1.0f);

    /* An unknown tag in the second list stops it. */
    memset(&list2, 0, sizeof(list2));
    tag(list2.b, 0, 0);
    packet(list2.b, 0x10, 1, 1.0f, 1.0f);
    lRhModel_SetMaterialIntensity(list1.b, list1.b, list2.b, list2.b + 0x50, mat.b);
    assert(at(list2.b, 0x20) == 1.0f);
}

int main(void)
{
    test_replace();
    test_add_clamp();
    test_scale();
    test_walk();
    return 0;
}
