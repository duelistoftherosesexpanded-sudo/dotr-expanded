#include "dotr/ee/ex_board.h"

/* Draws a window frame of nine tiles (tile set `style`, rows of gSzDuel_Icon_bytes from index
 * style * 9 + 140) at (x + 1728, y + 1824). With shadow >= 0 an untextured rectangle in colour
 * row `shadow` is sorted first, covering the whole frame (alpha value 0 for shadow >= 14, 2 for
 * shadow 12 or 13, none below). The middle column is repeated w times and the middle row h
 * times; a negative w or h is a pixel size, from which the corner tiles are subtracted. The
 * centre tile is not drawn, nor a tile whose width or height is not positive.
 * MATCHING DEVICE: the 8-byte alignment of the two packet locals gives the original's stack
 * offsets. */
DotrDrawWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrDrawWord packet, int colorIndex, int x, int y, int w, int h, int shadow,
                                             int style)
{
    unsigned char *state = (unsigned char *)&data_00317490;
    struct DotrExBoardTile *slices = &((struct DotrExBoardTile *)gSzDuel_Icon_bytes)[style * 9 + 0x8C];
    struct DotrExBoardColour *color;
    int originX;
    int originY;
    int width;
    int height;
    int xOffset;
    int yOffset;
    int column;
    struct DotrExBoardTile *slice;
    int row;
    int i;
    unsigned short y0;
    DotrExBoardSprite sprite __attribute__((aligned(8)));
    DotrExBoardFlat box __attribute__((aligned(8)));

    if (colorIndex < 0) {
        colorIndex = 2;
    }
    color = &gSzDuel_aaicolRgba_exboard[colorIndex];
    ((struct DotrExBoardHeader *)sprite.header)->unknown_02 = 0x26;
    ((struct DotrExBoardHeader *)sprite.header)->unknown_00 = 5;
    ((struct DotrExBoardHeader *)sprite.header)->unknown_08 = 0x42421;
    ((struct DotrExBoardHeader *)sprite.header)->unknown_04 = 0;
    ((struct DotrExBoardHeader *)sprite.header)->unknown_02 |= 0x800;
    ExAssignAlpha((struct DotrExBoardHeader *)sprite.header, ExGetAlphaVal(0));
    originX = x + 0x6C0;
    originY = y + 0x720;
    sprite.colour[0] = color->r;
    sprite.colour[1] = color->g;
    sprite.colour[2] = color->b;
    sprite.colour[3] = color->a;
    sprite.unknown_14 = 1.0f;
    sprite.vertex[0].xyz.z = *(int *)(state + 0xC58);
    sprite.vertex[0].xyz.f = 0;
    sprite.vertex[1].xyz.z = *(int *)(state + 0xC58);
    sprite.vertex[1].xyz.f = 0;
    if (shadow >= 0) {
        color = &gSzDuel_aaicolRgba_exboard[shadow];
        if (w >= 0) {
            width = (slices[2].u1 - slices[2].u0) +
                    ((slices[0].u1 - slices[0].u0) + w * (slices[1].u1 - slices[1].u0));
        } else {
            width = (w >= 0) ? w : -w;
        }
        if (h >= 0) {
            height = (slices[6].v1 - slices[6].v0) +
                     ((slices[0].v1 - slices[0].v0) + h * (slices[3].v1 - slices[3].v0));
        } else {
            height = (h >= 0) ? h : -h;
        }
        ((struct DotrExBoardHeader *)box.header)->unknown_02 = 6;
        ((struct DotrExBoardHeader *)box.header)->unknown_00 = 3;
        ((struct DotrExBoardHeader *)box.header)->unknown_08 = 0x441;
        ((struct DotrExBoardHeader *)box.header)->unknown_04 = 0;
        if (shadow >= 0xE) {
            ExAssignAlpha((struct DotrExBoardHeader *)box.header, ExGetAlphaVal(0));
        } else if (shadow >= 0xC) {
            ExAssignAlpha((struct DotrExBoardHeader *)box.header, ExGetAlphaVal(2));
        }
        box.colour[0] = color->r;
        box.colour[1] = color->g;
        box.colour[2] = color->b;
        box.colour[3] = color->a;
        box.unknown_14 = 1.0f;
        box.xyz[0].z = *(int *)(state + 0xC58);
        box.xyz[0].f = 0;
        box.xyz[1].z = *(int *)(state + 0xC58);
        box.xyz[1].f = 0;
        box.xyz[0].x = originX << 4;
        box.xyz[0].y = originY << 4;
        box.xyz[1].x = (originX + width) << 4;
        box.xyz[1].y = (originY + height) << 4;
        packet = ExSortPrim_Pipe(packet, &box, 0, 0);
    }
    if (w < 0) {
        w += (slices[0].u1 - slices[0].u0) + (slices[2].u1 - slices[2].u0);
    }
    if (h < 0) {
        h += (slices[0].v1 - slices[0].v0) + (slices[6].v1 - slices[6].v0);
    }
    yOffset = 0;
    for (row = 0; row < 3; row++) {
        xOffset = 0;
        y0 = (unsigned short)(originY + yOffset) * 16 - 8;
        for (column = 0; column < 3; column++) {
            i = row * 3 + column;
            slice = &slices[i];
            width = slice->u1 - slice->u0;
            height = slice->v1 - slice->v0;
            if (i == 1 || i == 4 || i == 7) {
                if (w > 0) {
                    width *= w;
                } else {
                    width = (w >= 0) ? w : -w;
                }
            }
            if ((unsigned int)(i - 3) < 3) {
                if (h > 0) {
                    height *= h;
                } else {
                    height = (h >= 0) ? h : -h;
                }
            }
            if (i != 4 && width > 0 && height > 0) {
                unsigned short columnX;

                /* the texture words at +0x6a0 of the control block hold addresses */
                ExAssignTexture((struct DotrExBoardHeader *)&sprite,
                                (int *)(DotrDrawWord)*(int *)(state + 0x6A0 + slice->texture * 4), 0);
                ((struct DotrExBoardUv64 *)&sprite.vertex[0].uv)->u = slice->u0 * 16;
                columnX = originX + xOffset;
                ((struct DotrExBoardUv64 *)&sprite.vertex[0].uv)->v = slice->v0 * 16;
                ((struct DotrExBoardUv64 *)&sprite.vertex[1].uv)->u = slice->u1 * 16;
                ((struct DotrExBoardUv64 *)&sprite.vertex[1].uv)->v = slice->v1 * 16;
                sprite.vertex[0].xyz.x = columnX * 16 - 8;
                sprite.vertex[0].xyz.y = y0;
                sprite.vertex[1].xyz.x = (width + columnX) * 16 - 8;
                sprite.vertex[1].xyz.y = (height + (unsigned short)(originY + yOffset)) * 16 - 8;
                packet = ExSortPrim_Pipe(packet, &sprite, 0, 0);
            }
            xOffset += width;
        }
        yOffset += height;
    }
    return packet;
}
