#include "dotr/ee/window_frame.h"

/* lSzDuel_DrawWindow_Frame_Pipe (0x1b4780): the duel window frame. With a colour row (0 or
 * more) it first draws the backing as two vertical gradients between the row's two colours
 * (alpha mode from ExGetAlphaVal for rows 0xc and above); then the nine-tile frame of the
 * style (tile table rows 0x3b + 9 * style, margins from 0x2be9c0) as sprites at
 * brightness 0x80 or 0x30, sized by columns and rows (repeat counts, or pixel sizes when
 * negative), and for icon kinds the arrow, cursor and page marks as textured quads. Every
 * piece is sorted with ExSortPrim_Pipe at the control block's depth. Returns the cursor.
 * ExSortPrim_Pipe is declared inside the function body. */
int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows, long long bright)
{
    DotrFrameSprite sprite __attribute__((aligned(8)));
    DotrFrameGradient gradient __attribute__((aligned(8)));
    unsigned long long spare;
    DotrFrameQuad arrow __attribute__((aligned(8)));
    DotrFrameQuad cursor __attribute__((aligned(8)));
    DotrFrameQuad page __attribute__((aligned(8)));
    DotrFrameState *graphics;
    DotrFrameTexture *parts;
    DotrFrameTexture *part;
    DotrFrameTexture *icon;
    unsigned char *margin;
    int dx;
    int x0;
    int y0;
    int height;
    int index;
    int ih;
    int w;
    int width;
    int kind;
    int shift;
    int col;
    int dy;
    int px;
    int py;
    int row;
    int iw;
    unsigned char *rgb;
    extern int ExSortPrim_Pipe();
    unsigned int tmp;

    (void)&spare; 
    graphics = (DotrFrameState *)data_00317490_frame.b;
    tmp = style;
    parts = &gSzDuel_Icon_frame[tmp * 9 + 0x3B];
    margin = &aabFrameOfs_frame[tmp * 4];
    tmp = x + 0x6C0;
    style = y + 0x720;
    if (colorIndex >= 0) {
        rgb = &gSzDuel_aaicolRgba_frame[colorIndex * 8];
        if (cols >= 0) {
            width = ((parts[2].x1 - parts[2].x0) - margin[1]) + (((parts[0].x1 - parts[0].x0) - margin[0]) + cols * (parts[1].x1 - parts[1].x0));
        } else {
            width = dotr_frame_abs(cols);
        }
        if (rows >= 0) {
            height = ((parts[6].y1 - parts[6].y0) - margin[3]) + (((parts[0].y1 - parts[0].y0) - margin[2]) + rows * (parts[3].y1 - parts[3].y0));
        } else {
            height = dotr_frame_abs(rows);
        }
        height >>= 1;
        x0 = tmp + margin[0];
        y0 = style + margin[2];
        dotr_frame_tag((DotrFrameTag *)gradient.tag, 8, 0x14, 0x41414141);
        if (colorIndex >= 0xE) {
            ExAssignAlpha(&gradient, ExGetAlphaVal(0));
        } else if (colorIndex >= 0xC) {
            ExAssignAlpha(&gradient, ExGetAlphaVal(2));
        }
        gradient.v[0].color.r = gradient.v[1].color.r = rgb[2];
        gradient.v[0].color.g = gradient.v[1].color.g = rgb[1];
        gradient.v[0].color.b = gradient.v[1].color.b = rgb[0];
        gradient.v[0].color.a = gradient.v[1].color.a = rgb[3];
        gradient.v[0].color.q = gradient.v[1].color.q = 1.0f;
        gradient.v[2].color.r = gradient.v[3].color.r = rgb[6];
        gradient.v[2].color.g = gradient.v[3].color.g = rgb[5];
        gradient.v[2].color.b = gradient.v[3].color.b = rgb[4];
        gradient.v[2].color.a = gradient.v[3].color.a = rgb[7];
        gradient.v[2].color.q = gradient.v[3].color.q = 1.0f;
        gradient.v[0].xyz.x = x0 * 16;
        gradient.v[0].xyz.y = y0 * 16;
        gradient.v[0].xyz.z = graphics->depth;
        gradient.v[0].xyz.fog = 0;
        gradient.v[1].xyz.x = (x0 + width) * 16;
        gradient.v[1].xyz.y = y0 * 16;
        gradient.v[1].xyz.z = graphics->depth;
        gradient.v[1].xyz.fog = 0;
        gradient.v[2].xyz.x = x0 * 16;
        gradient.v[2].xyz.y = (y0 + height) * 16;
        gradient.v[2].xyz.z = graphics->depth;
        gradient.v[2].xyz.fog = 0;
        gradient.v[3].xyz.x = (x0 + width) * 16;
        gradient.v[3].xyz.y = (y0 + height) * 16;
        gradient.v[3].xyz.z = graphics->depth;
        gradient.v[3].xyz.fog = 0;
        packet = ExSortPrim_Pipe(packet, &gradient, 0, 0);
        gradient.v[0].color.r = gradient.v[1].color.r = rgb[6];
        gradient.v[0].color.g = gradient.v[1].color.g = rgb[5];
        gradient.v[0].color.b = gradient.v[1].color.b = rgb[4];
        gradient.v[0].color.a = gradient.v[1].color.a = rgb[7];
        gradient.v[0].color.q = gradient.v[1].color.q = 1.0f;
        gradient.v[2].color.r = gradient.v[3].color.r = rgb[2];
        gradient.v[2].color.g = gradient.v[3].color.g = rgb[1];
        gradient.v[2].color.b = gradient.v[3].color.b = rgb[0];
        gradient.v[2].color.a = gradient.v[3].color.a = rgb[3];
        gradient.v[2].color.q = gradient.v[3].color.q = 1.0f;
        gradient.v[0].xyz.x = x0 * 16;
        gradient.v[0].xyz.y = (y0 + height) * 16;
        gradient.v[0].xyz.z = graphics->depth;
        gradient.v[0].xyz.fog = 0;
        gradient.v[1].xyz.x = (x0 + width) * 16;
        gradient.v[1].xyz.y = (y0 + height) * 16;
        gradient.v[1].xyz.z = graphics->depth;
        gradient.v[1].xyz.fog = 0;
        gradient.v[2].xyz.x = x0 * 16;
        gradient.v[2].xyz.y = (y0 + height * 2) * 16;
        gradient.v[2].xyz.z = graphics->depth;
        gradient.v[2].xyz.fog = 0;
        gradient.v[3].xyz.x = (x0 + width) * 16;
        gradient.v[3].xyz.y = (y0 + height * 2) * 16;
        gradient.v[3].xyz.z = graphics->depth;
        gradient.v[3].xyz.fog = 0;
        packet = ExSortPrim_Pipe(packet, &gradient, 0, 0);
    }
    dotr_frame_tag((DotrFrameTag *)sprite.tag, 5, 0x26, 0x42421);
    ((DotrFrameTag *)sprite.tag)->flags |= 0x800;
    ExAssignAlpha(&sprite, 0x8000000044LL);
    sprite.color.r = sprite.color.g = sprite.color.b = dotr_frame_intensity(bright);
    sprite.color.a = 0x80;
    sprite.color.q = 1.0f;
    sprite.xyz0.z = graphics->depth;
    sprite.xyz0.fog = 0;
    sprite.xyz1.z = graphics->depth;
    sprite.xyz1.fog = 0;
    if (cols < 0) {
        cols += ((parts[0].x1 - parts[0].x0) - margin[0]) + ((parts[2].x1 - parts[2].x0) - margin[1]);
    }
    if (rows < 0) {
        rows += ((parts[0].y1 - parts[0].y0) - margin[2]) + ((parts[6].y1 - parts[6].y0) - margin[3]);
    }
    dy = 0;
    for (row = 0; row < 3; row++) {
        dx = 0;
        for (col = 0; col < 3; col++) {
            index = row * 3 + col;
            part = &parts[index];
            width = part->x1 - part->x0;
            height = part->y1 - part->y0;
            if (index == 1 || index == 4 || index == 7) {
                if (cols > 0) {
                    width *= cols;
                } else if (cols >= 0) {
                    width = cols;
                } else {
                    width = -cols;
                }
            }
            if ((unsigned int)(index - 3) < 3) {
                if (rows > 0) {
                    height *= rows;
                } else if (rows >= 0) {
                    height = rows;
                } else {
                    height = -rows;
                }
            }
            if (index != 4 && width > 0 && height > 0) {
                ExAssignTexture(&sprite, graphics->textures[part->index], 0);
                ((DotrFrameUV *)&sprite.uv0)->u = part->x0 * 16;
                ((DotrFrameUV *)&sprite.uv0)->v = part->y0 * 16;
                ((DotrFrameUV *)&sprite.uv1)->u = part->x1 * 16;
                ((DotrFrameUV *)&sprite.uv1)->v = part->y1 * 16;
                sprite.xyz0.x = (tmp + dx) * 16 - 8;
                sprite.xyz0.y = (style + dy) * 16 - 8;
                sprite.xyz1.x = (width + (tmp + dx)) * 16 - 8;
                sprite.xyz1.y = (height + (style + dy)) * 16 - 8;
                packet = ExSortPrim_Pipe(packet, &sprite, 0, 0);
            }
            dx += width;
        }
        dy += height;
    }
    if (icons >= 0) {
        do {
            kind = icons & 0xF;
            shift = abDecoOfs_frame[kind];
            icon = &gSzDuel_Icon_frame[kind + 0x2F];
            if ((unsigned int)(kind - 3) < 2) {
                dotr_frame_tag((DotrFrameTag *)arrow.tag, 9, 0x24, 0x24242421 | ((unsigned long long)4 << 32));
                ((DotrFrameTag *)arrow.tag)->flags |= 0x800;
                ExAssignAlpha(&arrow, 0x8000000044LL);
                ExAssignTexture(&arrow, graphics->textures[icon->index], 0);
                arrow.color.r = 0x80;
                arrow.color.g = 0x80;
                arrow.color.b = 0x80;
                arrow.color.a = 0x80;
                arrow.color.q = 1.0f;
                arrow.c[0].xyz.z = graphics->depth;
                arrow.c[0].xyz.fog = 0;
                arrow.c[1].xyz.z = graphics->depth;
                arrow.c[1].xyz.fog = 0;
                arrow.c[2].xyz.z = graphics->depth;
                arrow.c[2].xyz.fog = 0;
                arrow.c[3].xyz.z = graphics->depth;
                arrow.c[3].xyz.fog = 0;
                ((DotrFrameUV *)&arrow.c[0].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&arrow.c[0].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&arrow.c[1].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&arrow.c[1].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&arrow.c[2].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&arrow.c[2].uv)->v = icon->y1 * 16;
                ((DotrFrameUV *)&arrow.c[3].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&arrow.c[3].uv)->v = icon->y1 * 16;
                ih = icon->y1 - icon->y0;
                iw = icon->x1 - icon->x0;
                px = (tmp + margin[0]) - shift;
                py = style + margin[2];
                arrow.c[0].xyz.x = px * 16 + 8;
                arrow.c[0].xyz.y = (py + iw) * 16 + 8;
                arrow.c[1].xyz.x = px * 16 + 8;
                arrow.c[1].xyz.y = py * 16 + 8;
                arrow.c[2].xyz.x = (px + ih) * 16 + 8;
                arrow.c[2].xyz.y = (py + iw) * 16 + 8;
                arrow.c[3].xyz.x = (px + ih) * 16 + 8;
                arrow.c[3].xyz.y = py * 16 + 8;
                packet = ExSortPrim_Pipe(packet, &arrow, 0, 0);
                px = ((parts[0].x1 - parts[0].x0) - margin[0]) + ((parts[2].x1 - parts[2].x0) - margin[1]);
                w = cols > 0 ? cols * (parts[1].x1 - parts[1].x0) : (cols >= 0 ? cols : -cols);
                px += w;
                px = (px + (tmp + margin[0])) - ((icon->y1 - icon->y0) - shift);
                py = style + margin[2];
                arrow.c[0].xyz.x = (px + ih) * 16 - 0x18;
                arrow.c[0].xyz.y = (py + iw) * 16 + 8;
                arrow.c[1].xyz.x = (px + ih) * 16 - 0x18;
                arrow.c[1].xyz.y = py * 16 + 8;
                arrow.c[2].xyz.x = px * 16 - 0x18;
                arrow.c[2].xyz.y = (py + iw) * 16 + 8;
                arrow.c[3].xyz.x = px * 16 - 0x18;
                arrow.c[3].xyz.y = py * 16 + 8;
                packet = ExSortPrim_Pipe(packet, &arrow, 0, 0);
            } else if (kind == 5) {
                dotr_frame_tag((DotrFrameTag *)cursor.tag, 9, 0x24, 0x24242421 | ((unsigned long long)4 << 32));
                ((DotrFrameTag *)cursor.tag)->flags |= 0x800;
                ExAssignAlpha(&cursor, 0x8000000044LL);
                ExAssignTexture(&cursor, graphics->textures[icon->index], 0);
                cursor.color.r = 0x80;
                cursor.color.g = 0x80;
                cursor.color.b = 0x80;
                cursor.color.a = 0x80;
                cursor.color.q = 1.0f;
                cursor.c[0].xyz.z = graphics->depth;
                cursor.c[0].xyz.fog = 0;
                cursor.c[1].xyz.z = graphics->depth;
                cursor.c[1].xyz.fog = 0;
                cursor.c[2].xyz.z = graphics->depth;
                cursor.c[2].xyz.fog = 0;
                cursor.c[3].xyz.z = graphics->depth;
                cursor.c[3].xyz.fog = 0;
                ((DotrFrameUV *)&cursor.c[0].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&cursor.c[0].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&cursor.c[1].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&cursor.c[1].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&cursor.c[2].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&cursor.c[2].uv)->v = icon->y1 * 16;
                ((DotrFrameUV *)&cursor.c[3].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&cursor.c[3].uv)->v = icon->y1 * 16;
                ih = icon->y1 - icon->y0;
                iw = icon->x1 - icon->x0;
                px = ((parts[0].x1 - parts[0].x0) - margin[0]) + ((parts[2].x1 - parts[2].x0) - margin[1]);
                if (cols > 0) {
                    w = cols * (parts[1].x1 - parts[1].x0);
                } else if (cols >= 0) {
                    w = cols;
                } else {
                    w = -cols;
                }
                px += w;
                w = ih - shift;
                px = (px + (tmp + margin[0])) - w;
                (void)w; 
                py = style + margin[2];
                cursor.c[0].xyz.x = (px + ih) * 16 - 0x18;
                cursor.c[0].xyz.y = (py + iw) * 16 + 8;
                cursor.c[1].xyz.x = (px + ih) * 16 - 0x18;
                cursor.c[1].xyz.y = py * 16 + 8;
                cursor.c[2].xyz.x = px * 16 - 0x18;
                cursor.c[2].xyz.y = (py + iw) * 16 + 8;
                cursor.c[3].xyz.x = px * 16 - 0x18;
                cursor.c[3].xyz.y = py * 16 + 8;
                packet = ExSortPrim_Pipe(packet, &cursor, 0, 0);
                icon = &gSzDuel_Icon_frame[0x38];
                ((DotrFrameUV *)&cursor.c[0].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&cursor.c[0].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&cursor.c[1].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&cursor.c[1].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&cursor.c[2].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&cursor.c[2].uv)->v = icon->y1 * 16;
                ((DotrFrameUV *)&cursor.c[3].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&cursor.c[3].uv)->v = icon->y1 * 16;
                ih = icon->y1 - icon->y0;
                iw = icon->x1 - icon->x0;
                py += 8;
                cursor.c[0].xyz.x = (px + ih) * 16 - 0x18;
                cursor.c[0].xyz.y = (py + iw) * 16 + 8;
                cursor.c[1].xyz.x = (px + ih) * 16 - 0x18;
                cursor.c[1].xyz.y = py * 16 + 8;
                cursor.c[2].xyz.x = px * 16 - 0x18;
                cursor.c[2].xyz.y = (py + iw) * 16 + 8;
                cursor.c[3].xyz.x = px * 16 - 0x18;
                cursor.c[3].xyz.y = py * 16 + 8;
                packet = ExSortPrim_Pipe(packet, &cursor, 0, 0);
            } else if (kind == 6) {
                dotr_frame_tag((DotrFrameTag *)page.tag, 9, 0x24, 0x24242421 | ((unsigned long long)4 << 32));
                ((DotrFrameTag *)page.tag)->flags |= 0x800;
                ExAssignAlpha(&page, 0x8000000044LL);
                ExAssignTexture(&page, graphics->textures[icon->index], 0);
                page.color.r = 0x80;
                page.color.g = 0x80;
                page.color.b = 0x80;
                page.color.a = 0x80;
                page.color.q = 1.0f;
                page.c[0].xyz.z = graphics->depth;
                page.c[0].xyz.fog = 0;
                page.c[1].xyz.z = graphics->depth;
                page.c[1].xyz.fog = 0;
                page.c[2].xyz.z = graphics->depth;
                page.c[2].xyz.fog = 0;
                page.c[3].xyz.z = graphics->depth;
                page.c[3].xyz.fog = 0;
                ((DotrFrameUV *)&page.c[0].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&page.c[0].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&page.c[1].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&page.c[1].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&page.c[2].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&page.c[2].uv)->v = icon->y1 * 16;
                ((DotrFrameUV *)&page.c[3].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&page.c[3].uv)->v = icon->y1 * 16;
                ih = icon->y1 - icon->y0;
                iw = icon->x1 - icon->x0;
                px = (tmp + margin[0]) - shift;
                py = style + margin[2];
                page.c[0].xyz.x = px * 16 + 8;
                page.c[0].xyz.y = (py + iw) * 16 + 8;
                page.c[1].xyz.x = px * 16 + 8;
                page.c[1].xyz.y = py * 16 + 8;
                page.c[2].xyz.x = (px + ih) * 16 + 8;
                page.c[2].xyz.y = (py + iw) * 16 + 8;
                page.c[3].xyz.x = (px + ih) * 16 + 8;
                page.c[3].xyz.y = py * 16 + 8;
                packet = ExSortPrim_Pipe(packet, &page, 0, 0);
                icon = &gSzDuel_Icon_frame[0x39];
                ((DotrFrameUV *)&page.c[0].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&page.c[0].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&page.c[1].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&page.c[1].uv)->v = icon->y0 * 16;
                ((DotrFrameUV *)&page.c[2].uv)->u = icon->x0 * 16;
                ((DotrFrameUV *)&page.c[2].uv)->v = icon->y1 * 16;
                ((DotrFrameUV *)&page.c[3].uv)->u = icon->x1 * 16;
                ((DotrFrameUV *)&page.c[3].uv)->v = icon->y1 * 16;
                ih = icon->y1 - icon->y0;
                iw = icon->x1 - icon->x0;
                py += 8;
                px -= ih - shift;
                page.c[0].xyz.x = (px + ih) * 16 + 8;
                page.c[0].xyz.y = py * 16 + 8;
                page.c[1].xyz.x = (px + ih) * 16 + 8;
                page.c[1].xyz.y = (py + iw) * 16 + 8;
                page.c[2].xyz.x = px * 16 + 8;
                page.c[2].xyz.y = py * 16 + 8;
                page.c[3].xyz.x = px * 16 + 8;
                page.c[3].xyz.y = (py + iw) * 16 + 8;
                packet = ExSortPrim_Pipe(packet, &page, 0, 0);
            } else {
                px = ((parts[0].x1 - parts[0].x0) - margin[0]) + ((parts[2].x1 - parts[2].x0) - margin[1]);
                w = cols > 0 ? cols * (parts[1].x1 - parts[1].x0) : (cols >= 0 ? cols : -cols);
                px += w;
                px >>= 1;
                px -= (icon->x1 - icon->x0) >> 1;
                px = px + (tmp + margin[0]);
                py = (style + margin[2]) - shift;
                ExAssignTexture(&sprite, graphics->textures[icon->index], 0);
                ((DotrFrameUV *)&sprite.uv0)->u = icon->x0 * 16 + 8;
                ((DotrFrameUV *)&sprite.uv0)->v = icon->y0 * 16 + 8;
                ((DotrFrameUV *)&sprite.uv1)->u = icon->x1 * 16 + 8;
                ((DotrFrameUV *)&sprite.uv1)->v = icon->y1 * 16 + 8;
                iw = icon->x1 - icon->x0;
                ih = icon->y1 - icon->y0;
                sprite.xyz0.x = px * 16;
                sprite.xyz0.y = py * 16;
                sprite.xyz1.x = (px + iw) * 16;
                sprite.xyz1.y = (py + ih) * 16;
                packet = ExSortPrim_Pipe(packet, &sprite, 0, 0);
            }
            icons >>= 4;
        } while (icons != 0);
    }
    return packet;
}
