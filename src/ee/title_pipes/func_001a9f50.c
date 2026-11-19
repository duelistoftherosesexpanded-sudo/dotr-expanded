#include "dotr/ee/title_pipes.h"

/* lSzGame_TEXTURE_TEST (0x1a9f50): debug view of one texture record: uploads it after the
 * user GS area plus the CLUT space (SzText_Load, with texture flushes around it) and draws it
 * as one sprite centred on (x + 0x800, y + 0x800), at double size when the flag is set, with
 * colour 0x80, depth 0xfffff8 and texture coordinates scaled to the next power of two (at
 * least 16) of its width and height.
 * SzText_InitClutManage is declared at block scope; the int casts of pointers are written with
 * the cursor word type (int on the target). */
void lSzGame_TEXTURE_TEST(void *arg0, int arg1, int arg2, int arg3)
{
    DotrTitleTexSprite sprite __attribute__((aligned(8)));
    int centerX;
    int centerY;
    int halfWidth;
    int halfHeight;
    unsigned short width;
    unsigned short height;
    int texture;
    unsigned short textureWidth;
    unsigned short textureHeight;
    extern int SzText_InitClutManage();

    width = *(unsigned short *)((unsigned char *)arg0 + 0x54);
    height = *(unsigned short *)((unsigned char *)arg0 + 0x56);
    centerX = arg1 + 0x800;
    centerY = arg2 + 0x800;
    halfWidth = width >> 1;
    halfHeight = height >> 1;
    if (arg3 != 0) {
        halfWidth *= 2;
        halfHeight *= 2;
    }
    ExFlushTexture();
    texture = RhBase_GetUserGsAddress();
    texture += SzText_InitClutManage(texture, 1);
    SzText_Load((DotrTitleWord)arg0, texture, 0);
    ExFlushTexture();
    ((DotrTitleTexTag *)&sprite.tag)->primitive = 0x26;
    ((DotrTitleTexTag *)&sprite.tag)->loops = 5;
    ((DotrTitleTexTag *)&sprite.tag)->registers = 0x42421;
    ((DotrTitleTexTag *)&sprite.tag)->flags = 0;
    ExAssignAlpha(&sprite, ExGetAlphaVal(0));
    ExAssignTexture(&sprite, arg0, 0);
    sprite.r = 0x80;
    sprite.g = 0x80;
    sprite.b = 0x80;
    sprite.a = 0x80;
    sprite.q = 1.0f;
    textureWidth = 0x10;
    textureHeight = 0x10;
    while (width > textureWidth) {
        textureWidth <<= 1;
    }
    while (height > textureHeight) {
        textureHeight <<= 1;
    }
    sprite.s0 = 0.0f;
    sprite.t0 = 0.0f;
    sprite.s1 = (float)width / (float)textureWidth;
    sprite.t1 = (float)height / (float)textureHeight;
    sprite.xyz0.x = ((centerX - halfWidth) << 4) - 8;
    sprite.xyz0.y = ((centerY - halfHeight) << 4) - 8;
    sprite.xyz0.z = 0xFFFFF8;
    sprite.xyz0.fog = 0;
    sprite.xyz1.x = ((centerX + halfWidth) << 4) - 8;
    sprite.xyz1.y = ((centerY + halfHeight) << 4) - 8;
    sprite.xyz1.z = 0xFFFFF8;
    sprite.xyz1.fog = 0;
    ExSortPrim((DotrTitleWord)&sprite, 0, 0);
}
