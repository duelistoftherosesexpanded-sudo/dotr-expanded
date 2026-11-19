#include "dotr/ee/window_font.h"

/* SzDuel_DrawWindow_Font_Pipe: draws `count` characters of `text` at window position (x, y) in
 * font `font` and returns the advanced cursor. Each character goes through
 * lSzDuel_GetFontIndex: a negative answer is a line feed ('\n': back to the left edge, down
 * one cell height) or a blank (advance by the cell width, half of it when proportional);
 * otherwise the glyph's texture and texel rectangle are put into a four-vertex packet and
 * sorted with ExSortPrim_Pipe. flags: 1 proportional (advance by the glyph width instead of
 * centring the glyph in the cell), 2 centred (the widest line is measured first and half of
 * it is taken off x), 4 slanted (the top edge is shifted right by a sixth of the cell
 * width), 8 / 16 alpha mode 2 / 3 instead of 0. A null colour selects the bytes at 0x2bea00.
 * The 8-byte alignment of the packet local gives the original's stack offsets. */
DotrDrawWord SzDuel_DrawWindow_Font_Pipe(DotrDrawWord packet, int x, int y, int font, int flags, int count,
                                         const char *text, unsigned char *color)
{
    unsigned char *state;
    int width;
    int lineHeight;
    int originX;
    int cursorX;
    int cursorY;
    int proportional;
    int centered;
    int italic;
    int additive;
    int subtractive;
    int i;
    long blend;
    DotrFontPacket sprite __attribute__((aligned(8)));

    originX = x + 0x6C0;
    state = (unsigned char *)&data_00317490;
    width = gSzDuel_abFontSize_bytes[font * 2];
    cursorY = y + 0x720;
    lineHeight = data_002be951[font * 2];
    proportional = 0;
    centered = 0;
    italic = 0;
    additive = 0;
    subtractive = 0;
    if (flags & 1) {
        proportional = 1;
    }
    if (flags & 2) {
        centered = 1;
    }
    if (flags & 4) {
        italic = 1;
    }
    if (flags & 8) {
        additive = 1;
    }
    if (flags & 0x10) {
        subtractive = 1;
    }
    if (color == 0) {
        color = icolRgba_2580;
    }
    ((struct DotrCursorPacketHeader *)sprite.header)->unknown_02 = 0x24;
    ((struct DotrCursorPacketHeader *)sprite.header)->unknown_00 = 9;
    ((struct DotrCursorPacketHeader *)sprite.header)->unknown_08 = 0x424242421;
    ((struct DotrCursorPacketHeader *)sprite.header)->unknown_04 = 0;
    ((struct DotrCursorPacketHeader *)sprite.header)->unknown_02 |= 0x800;
    if (additive) {
        blend = ExGetAlphaVal(2);
    } else if (subtractive) {
        blend = ExGetAlphaVal(3);
    } else {
        blend = ExGetAlphaVal(0);
    }
    ExAssignAlpha((struct DotrCursorPacketHeader *)sprite.header, blend);
    sprite.colour[0] = color[2];
    sprite.colour[1] = color[1];
    sprite.colour[2] = color[0];
    sprite.colour[3] = color[3];
    sprite.unknown_14 = 1.0f;
    sprite.vertex[0].xyz.z = *(int *)(state + 0xC58);
    sprite.vertex[0].xyz.f = 0;
    sprite.vertex[1].xyz.z = *(int *)(state + 0xC58);
    sprite.vertex[1].xyz.f = 0;
    sprite.vertex[2].xyz.z = *(int *)(state + 0xC58);
    sprite.vertex[2].xyz.f = 0;
    sprite.vertex[3].xyz.z = *(int *)(state + 0xC58);
    sprite.vertex[3].xyz.f = 0;
    if (centered) {
        int maxWidth = 0;
        int lineWidth = 0;

        for (i = 0; i < count; i++) {
            const char *character = text + i;
            int glyph = lSzDuel_GetFontIndex(*character);
            struct DotrFontGlyph *entry;

            if (glyph < 0) {
                if (*character == '\n') {
                    if (maxWidth < lineWidth) {
                        maxWidth = lineWidth;
                    }
                    lineWidth = 0;
                } else {
                    lineWidth += proportional ? (width >> 1) : width;
                }
            } else {
                entry = &data_00290b70[glyph + font * 37];
                if (proportional) {
                    lineWidth += entry->u1 - entry->u0;
                } else {
                    lineWidth += width;
                }
            }
        }
        if (maxWidth < lineWidth) {
            maxWidth = lineWidth;
        }
        originX -= maxWidth >> 1;
    }
    cursorX = originX;
    for (i = 0; i < count; i++) {
        const char *character = text + i;
        int glyph = lSzDuel_GetFontIndex(*character);
        int offset = 0;
        int shear = 0;
        struct DotrFontGlyph *entry;
        int glyphWidth;
        int glyphHeight;
        unsigned short top;
        unsigned short left;
        unsigned short right;

        if (glyph < 0) {
            if (*character == '\n') {
                cursorX = originX;
                cursorY += lineHeight;
            } else {
                cursorX += proportional ? (width >> 1) : width;
            }
        } else {
            entry = &data_00290b70[glyph + font * 37];
            /* the texture words at +0x6a0 of the control block hold addresses */
            ExAssignTexture((struct DotrCursorPacketHeader *)&sprite,
                            (int *)(DotrDrawWord)*(int *)(state + 0x6A0 + entry->texture * 4), 0);
            ((struct DotrFontUv *)&sprite.vertex[0].uv)->u = entry->u0 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[0].uv)->v = entry->v0 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[1].uv)->u = entry->u1 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[1].uv)->v = entry->v0 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[2].uv)->u = entry->u0 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[2].uv)->v = entry->v1 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[3].uv)->u = entry->u1 * 16 + 8;
            ((struct DotrFontUv *)&sprite.vertex[3].uv)->v = entry->v1 * 16 + 8;
            glyphWidth = entry->u1 - entry->u0;
            glyphHeight = entry->v1 - entry->v0;
            if (!proportional) {
                offset = (width - glyphWidth) * 8;
            }
            if (italic) {
                shear = (width * 16) / 6;
            }
            top = cursorY * 16;
            left = (cursorX << 4) + offset;
            right = offset + (cursorX + glyphWidth) * 16;
            sprite.vertex[0].xyz.x = shear + left;
            sprite.vertex[0].xyz.y = top;
            sprite.vertex[1].xyz.x = shear + right;
            sprite.vertex[1].xyz.y = top;
            sprite.vertex[2].xyz.x = left;
            sprite.vertex[2].xyz.y = (cursorY + glyphHeight) * 16;
            sprite.vertex[3].xyz.x = right;
            sprite.vertex[3].xyz.y = (cursorY + glyphHeight) * 16;
            packet = ExSortPrim_Pipe(packet, &sprite, 0, 0);
            cursorX += proportional ? glyphWidth : width;
        }
    }
    return packet;
}
