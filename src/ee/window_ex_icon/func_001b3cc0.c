#include "dotr/ee/window_ex_icon.h"

#ifndef NULL
#define NULL 0
#endif

/* lSzDuel_DrawWindow_ExIcon_Pipe (0x1b3cc0): draws one window icon into the packet cursor. */

static inline void tag_exicon(struct DotrExIconTag *tag, int count, int flags, unsigned long long registers) {
    tag->flags = flags;
    tag->count = count;
    tag->registers = registers;
    tag->reserved = 0;
}

static inline int abs_exicon(int value) {
    if (value >= 0) {
        return value;
    }
    return -value;
}

static inline int intensity_exicon(int bright) {
    if (bright != 0) {
        return 0x80;
    }
    return 0x30;
}

static inline int lower_exicon(int value) {
    if (value > 0) {
        return value;
    }
    return 0;
}

static inline int upper_exicon(int value) {
    if (value < 0x3FFF) {
        return value;
    }
    return 0x3FFF;
}

DotrExIconWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrExIconWord packet, unsigned short *src, int x, int y,
                                              int background, int image, int border, int shrink,
                                              int flipX, int flipY, int bright, unsigned char *rgb,
                                              int blend, unsigned char alpha) {
    struct DotrExIconSprite sprite __attribute__((aligned(8)));
    struct DotrExIconSolid solid __attribute__((aligned(8)));
    struct DotrExIconState *graphics;
    int width;
    int height;

    graphics = &data_00317490_exicon;
    width = abs_exicon(src[3] - src[1]);
    height = abs_exicon(src[4] - src[2]);
    if (shrink != 0) {
        width = (width * 3) >> 2;
        height = (height * 3) >> 2;
    }
    tag_exicon((struct DotrExIconTag *)sprite.tag, 5, 0x26, 0x42421);
    ((struct DotrExIconTag *)sprite.tag)->flags |= 0x800;
    ExAssignAlpha(&sprite, 0x8000000044LL);
    sprite.color.r = sprite.color.g = sprite.color.b = intensity_exicon(bright);
    sprite.color.a = alpha;
    sprite.color.q = 1.0f;
    sprite.xyz0.z = graphics->depth;
    sprite.xyz0.fog = 0;
    sprite.xyz1.z = graphics->depth;
    sprite.xyz1.fog = 0;
    tag_exicon((struct DotrExIconTag *)solid.tag, 3, 6, 0x441);
    solid.color.r = 0x80;
    solid.color.g = 0x80;
    solid.color.b = 0x80;
    solid.color.a = 0x80;
    solid.color.q = 1.0f;
    solid.xyz0.z = graphics->depth;
    solid.xyz0.fog = 0;
    solid.xyz1.z = graphics->depth;
    solid.xyz1.fog = 0;
    if (background != 0) {
        solid.color.r = solid.color.g = solid.color.b = 0;
        solid.xyz0.x = x * 16;
        solid.xyz0.y = y * 16;
        solid.xyz1.x = (x + width) * 16;
        solid.xyz1.y = (y + height) * 16;
        packet = ExSortPrim_Pipe(packet, (DotrExIconWord)&solid, 0, 0);
    }
    if (image != 0) {
        ExAssignTexture(&sprite, graphics->textures[((unsigned char *)src)[0]], 0);
        if (flipX != 0 && flipY != 0) {
            ((struct DotrExIconUv *)&sprite.uv0)->u = lower_exicon(src[3] * 16 - 8);
            ((struct DotrExIconUv *)&sprite.uv0)->v = lower_exicon(src[4] * 16 - 8);
            ((struct DotrExIconUv *)&sprite.uv1)->u = lower_exicon(src[1] * 16 - 8);
            ((struct DotrExIconUv *)&sprite.uv1)->v = lower_exicon(src[2] * 16 - 8);
        } else if (flipX != 0 && flipY == 0) {
            ((struct DotrExIconUv *)&sprite.uv0)->u = lower_exicon(src[3] * 16 - 8);
            ((struct DotrExIconUv *)&sprite.uv0)->v = upper_exicon(src[2] * 16 + 8);
            ((struct DotrExIconUv *)&sprite.uv1)->u = lower_exicon(src[1] * 16 - 8);
            ((struct DotrExIconUv *)&sprite.uv1)->v = upper_exicon(src[4] * 16 + 8);
        } else if (flipX == 0 && flipY != 0) {
            ((struct DotrExIconUv *)&sprite.uv0)->u = upper_exicon(src[1] * 16 + 8);
            ((struct DotrExIconUv *)&sprite.uv0)->v = lower_exicon(src[4] * 16 - 8);
            ((struct DotrExIconUv *)&sprite.uv1)->u = upper_exicon(src[3] * 16 + 8);
            ((struct DotrExIconUv *)&sprite.uv1)->v = lower_exicon(src[2] * 16 - 8);
        } else {
            ((struct DotrExIconUv *)&sprite.uv0)->u = upper_exicon(src[1] * 16 + 8);
            ((struct DotrExIconUv *)&sprite.uv0)->v = upper_exicon(src[2] * 16 + 8);
            ((struct DotrExIconUv *)&sprite.uv1)->u = upper_exicon(src[3] * 16 + 8);
            ((struct DotrExIconUv *)&sprite.uv1)->v = upper_exicon(src[4] * 16 + 8);
        }
        sprite.xyz0.x = x * 16;
        sprite.xyz0.y = y * 16;
        sprite.xyz1.x = (x + width) * 16;
        sprite.xyz1.y = (y + height) * 16;
        packet = ExSortPrim_Pipe(packet, (DotrExIconWord)&sprite, 0, 0);
    }
    if (border != 0) {
        packet = lSzDuel_DrawWindow_ExIcon_Pipe(packet, (unsigned short *)data_002909f4_exicon, x, y, 0, 1, 0, shrink,
                              0, 0, bright, NULL, -1, 0x80);
    }
    if (blend >= 0) {
        ExAssignAlpha(&solid, ExGetAlphaVal(blend));
        if (rgb != NULL) {
            solid.color.r = rgb[2];
            solid.color.g = rgb[1];
            solid.color.b = rgb[0];
        } else {
            solid.color.r = solid.color.g = solid.color.b = 0x80;
        }
        solid.color.a = data_00318118_exicon >> 8;
        solid.xyz0.x = x * 16;
        solid.xyz0.y = y * 16;
        solid.xyz1.x = (x + width) * 16;
        solid.xyz1.y = (y + height) * 16;
        packet = ExSortPrim_Pipe(packet, (DotrExIconWord)&solid, 0, 0);
    }
    return packet;
}
