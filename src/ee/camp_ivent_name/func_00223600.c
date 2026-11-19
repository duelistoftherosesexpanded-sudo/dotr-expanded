#include "dotr/ee/camp_ivent_name.h"

/* 0x223600. Compiled with #pragma conditional_move off and #pragma opt_dead_assignments off, set
 * directly before the function and reset after it. The name-list loop holds 41 repeated prototype
 * declarations, which generate no code; the work area is addressed by its literal through AT. */
#ifndef __MWERKS__
/* Host build only: the code turns the int cursor into a pointer for Taku_FT4_Set and back, and
 * leaves some locals unset on some paths, which the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wpointer-sign"
#endif

/* MATCHING DEVICE: the work area by its literal address (camp_ivent_name.h). */
#define AT(a) DOTR_IVENT_NAME_AT(a)

#ifdef __MWERKS__
#pragma conditional_move off
#pragma opt_dead_assignments off
#endif

int Taku_Camp_Ivent_Name(int packet) {
    extern int SzDuel_DrawWindow_Font_Pipe();
    extern int Taku_Obj_Set(int, void *, short, short);
    extern int Taku_G4_Set(int, void *, short, short);
    extern int SzMsg_DrawRect_Pipe();
    extern int SzMsg_GetInfo();
    extern int Taku_Bunsyou_Set(int, int, int, short, short, float, float, int, void *);
    short fromX;
    int font;
    short row;
    short textRow;
    short section;
    short page;
    short left;
    short right;
    short textOffset;
    short paletteRow;
    short nextPaletteRow;
    short cellBase;
    short sectionX;
    short (*entry)[3];
    short colourIndex;
    int slide;
    short fromY;
    DotrIventNameState *state;
    short shift;
    short scroll;
    short rowY;
    int selX;
    int selY;
    DotrIventNameTexture *texture;
    short charX;
    int texX;
    int texY;
    short col;
    short i;
    short k;
    short texCol;
    short texRow;
    DotrIventNameTexture *selTex;
    DotrIventNameSprite *sprite;
    int labelX;
    short labelLeft;
    short labelRight;
    short count;
    short x;
    short y;
    short dx;
    short band;
    short offset;
    short cellX;
    short toX;
    short group;
    signed char cell;
    short n;
    short lcol;
    short lrow;
    short lx;
    short ly;
    int ltx;
    int lty;
    int px;
    int sy;
    short py;
    short tx;
    int ty;
    DotrIventNameRect baseRect;
    DotrIventNameUv uv;
    DotrIventNamePalette palette;
    DotrIventNameSix decorations;
    DotrIventNameSprite tile;
    DotrIventNameThree labels;
    DotrIventNameSprite cursor;
    DotrIventNameRect listRect;
    DotrIventNameRect selectedRect;
    DotrIventNameImage image;
    unsigned char rgba[4];

    state = (DotrIventNameState *)AT(0x01ED0000);
    font = 0;
    uv = data_0029a7e0_iventname;
    palette = data_0029a7f0_iventname;
    image = data_0029a830_iventname;
    decorations = data_0029a840_iventname;
    tile = data_0029a930_iventname;
    labels = data_0029a960_iventname;
    cursor = data_0029a9e0_iventname;
    rgba[2] = 255; rgba[1] = 255; rgba[0] = 255; rgba[3] = 128;
    image.page = 2;
    image.w = 0x300;
    image.h = (state->slide * 404) / 100 + 16;
    if (state->mode == 1) {
        state->slide += 6;
        if (state->slide >= 100) {
            state->slide = 100;
            state->mode = 2;
        }
    }
    if (state->mode == 3) {
        state->slide -= 6;
        if (state->slide < 0) {
            state->slide = 0;
            state->mode = 4;
        }
    }
    packet = Taku_Window_Set(packet, &image, 2, 0, 0xFFC0, 16, 0, 128, 64);
    if (0 < state->mode) {
        slide = (state->slide * 640) / 100;
        left = 640 - slide;
        image.page = 2;
        image.w = 236;
        image.h = 32;
        right = slide - 640;
        x = left + 28;
        packet = Taku_Window_Set(packet, &image, 5, 0, x, 28, 0, 128, 128);
        x += 43;
        packet = SzDuel_DrawWindow_Font_Pipe(packet, x, 35, 2, 1, 10, data_0030f0b0_iventname, 0);
        image.page = 0;
        x = right + 176;
        image.w = 288;
        image.h = 40;
        packet = Taku_Window_Set(packet, &image, 2, 0, x, 88, 0, 128, 64);
        labelX = (short)x + 96;
        sprite = labels.item;
        i = 0;
        labelLeft = x - 12;
        labelRight = 288 + x;
        for (; i < 3; i++) {
            x = labelX;
            y = 74;
            switch (i) {
            case 0:
                break;
            case 1:
                x = labelLeft;
                y = 92;
                break;
            case 2:
                x = labelRight;
                y = 92;
                break;
            }
            packet = (int)Taku_FT4_Set((void *)packet, sprite, x, y, 0);
            sprite++;
        }
        x = right + (state->count * 22 + 185);
        if (0 <= state->blink) {
            y = (signed char)state->blink;
        } else {
            y = -(signed char)state->blink;
        }
        cursor.alpha3 = y;
        cursor.alpha2 = y;
        cursor.alpha1 = y;
        cursor.a = y;
        state->blink += 16;
        if (state->count < 12) {
            packet = Taku_G4_Set(packet, &cursor, x, 94);
        }
        x = 48 + left;
        image.page = 9;
        image.w = 540;
        image.h = 34;
        packet = Taku_Window_Set(packet, &image, 3, 0, x, 383, 0, 128, 64);
        sprite = decorations.item;
        for (i = 0; i < 6; i++) {
            x = left + 0;
            if (i != 3 && i != 4) {
                packet = Taku_Obj_Set(packet, sprite, x, 0);
            }
            sprite++;
        }
        dx = left;
        entry = data_002999a0_iventname;
        for (i = 0; i < 3; i++) {
            font = (*entry)[2];
            packet = Taku_Bunsyou_Set(packet, 1, font, (short)dx + (*entry)[0], (*entry)[1], 1.0f, 1.0f, 0, rgba);
            entry++;
        }
        if (state->scrolling != 0) {
            count = 3;
            scroll = (state->scroll * 640) / 100;
        } else {
            count = 1;
            scroll = 0;
        }
        for (page = 0; page < count; page++) {
            shift = 0;
            switch (page) {
            case 1:
                shift = -640;
                break;
            case 2:
                shift = 640;
                break;
            }
            for (section = 0, sectionX = 0; section < 3; section++) {
                band = 0;
                switch (state->order) {
                case 0:
                    switch (page) {
                    case 0:
                        break;
                    case 1:
                        band = 6;
                        break;
                    case 2:
                        band = 3;
                        break;
                    }
                    break;
                case 1:
                    switch (page) {
                    case 0:
                        band = 3;
                        break;
                    case 1:
                        break;
                    case 2:
                        band = 6;
                        break;
                    }
                    break;
                case 2:
                    switch (page) {
                    case 0:
                        band = 6;
                        break;
                    case 1:
                        band = 3;
                        break;
                    case 2:
                        band = 0;
                        break;
                    }
                    break;
                }
                paletteRow = section + band;
                nextPaletteRow = section + 1 + band;
                cellBase = sectionX + 0;
                for (row = 0, rowY = 0; row < 6; row++) {
                    if (row % 2 == 0) {
                        colourIndex = paletteRow;
                    } else {
                        colourIndex = nextPaletteRow;
                    }
                    if (row % 2 == 0) {
                        offset = right;
                    } else {
                        offset = left;
                    }
                    cellX = cellBase;
                    for (col = 0; col < 5; col++) {
                        tx = offset + (shift + cellX);
                        ty = rowY;
                        tile.r = (palette.rgb[(short)colourIndex][0] >> 1) * 3;
                        tile.g = (palette.rgb[(short)colourIndex][1] >> 1) * 3;
                        tile.b = (palette.rgb[(short)colourIndex][2] >> 1) * 3;
                        packet = Taku_Obj_Set(packet, &tile, (short)(tx + scroll), ty);
                        cellX = cellX + 32;
                    }
                    rowY += 36;
                }
                sectionX += 160;
            }
            for (textRow = 0; textRow < 3; textRow++) {
                switch (state->order) {
                case 0:
                    switch (page) {
                    case 0:
                        font = textRow + 11;
                        break;
                    case 1:
                        font = textRow + 5;
                        break;
                    case 2:
                        font = textRow + 14;
                        break;
                    }
                    break;
                case 1:
                    switch (page) {
                    case 0:
                        font = textRow + 14;
                        break;
                    case 1:
                        font = textRow + 11;
                        break;
                    case 2:
                        font = textRow + 5;
                        break;
                    }
                    break;
                case 2:
                    switch (page) {
                    case 0:
                        font = textRow + 5;
                        break;
                    case 1:
                        font = textRow + 14;
                        break;
                    case 2:
                        font = textRow + 11;
                        break;
                    }
                    break;
                }
                packet = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(packet, 1, font, 0, 22));
                texture = (DotrIventNameTexture *)SzMsg_GetInfo(1);
                baseRect = texture->rect;
                texY = baseRect.y;
                for (row = 0; row < 6; row++) {
                    if (row % 2 == 0) {
                        textOffset = right;
                    } else {
                        textOffset = left;
                    }
                    texX = baseRect.x;
                    for (col = 0, charX = 0; col < 5; col++) {
                        tx = textOffset + (tile.x + 5 + charX + textRow * 160);
                        py = tile.y + 5 + row * 36;
                        rgba[2] = 0; rgba[1] = 0; rgba[0] = 0; rgba[3] = 128;
                        texture->rect.x = texX;
                        texture->rect.y = texY;
                        texture->rect.w = 22;
                        texture->rect.h = 22;
                        packet = SzMsg_DrawRect_Pipe(packet, 1, scroll + (tx + shift), py, &uv, rgba);
                        tx = textOffset + (tile.x + 3 + charX + textRow * 160);
                        py = tile.y + 3 + row * 36;
                        rgba[2] = 255; rgba[1] = 255; rgba[0] = 255; rgba[3] = 128;
                        texture->rect.x = texX;
                        texture->rect.y = texY;
                        texture->rect.w = 22;
                        texture->rect.h = 22;
                        packet = SzMsg_DrawRect_Pipe(packet, 1, scroll + (tx + shift), py, &uv, rgba);
                        charX = charX + 32;
                        texX += 22;
                    }
                    texY += 22;
                }
                SzMsg_Free(1);
            }
        }
        if (state->selected == 1) {
            for (k = 0; k < 3; k++) {
                texCol = state->selCol % 5;
                texRow = state->selRow;
                if (state->selCol / 5 == k) {
                    switch (state->order) {
                    case 0:
                        font = k + 11;
                        break;
                    case 1:
                        font = k + 14;
                        break;
                    case 2:
                        font = k + 5;
                        break;
                    }
                    packet = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(packet, 1, font, 0, 22));
                    selTex = (DotrIventNameTexture *)SzMsg_GetInfo(1);
                    selectedRect = selTex->rect;
                    rgba[2] = 0; rgba[1] = 0; rgba[0] = 0; rgba[3] = 128;
                    toX = state->count * 22 + 190;
                    fromX = tile.x + 5 + state->selCol * 32;
                    fromY = tile.y + 5 + state->selRow * 36;
                    px = fromX + (short)((state->selMove * (toX - fromX)) / 100);
                    sy = fromY + (short)((state->selMove * (96 - fromY)) / 100);
                    selX = selectedRect.x + (short)texCol * 22;
                    selTex->rect.x = selX;
                    selY = selectedRect.y + (short)texRow * 22;
                    selTex->rect.y = selY;
                    selTex->rect.w = 22;
                    selTex->rect.h = 22;
                    packet = SzMsg_DrawRect_Pipe(packet, 1, px, sy, &uv, rgba);
                    rgba[2] = 255; rgba[1] = 255; rgba[0] = 255; rgba[3] = 128;
                    fromX = tile.x + 3 + state->selCol * 32;
                    fromY = tile.y + 3 + state->selRow * 36;
                    px = fromX + (short)((state->selMove * ((short)(state->count * 22 + 188) - fromX)) / 100);
                    sy = fromY + (short)((state->selMove * (94 - fromY)) / 100);
                    selTex->rect.x = selX;
                    selTex->rect.y = selY;
                    selTex->rect.w = 22;
                    selTex->rect.h = 22;
                    packet = SzMsg_DrawRect_Pipe(packet, 1, px, sy, &uv, rgba);
                    SzMsg_Free(1);
                }
            }
        }
        image.page = 10;
        image.w = 36;
        image.h = 36;
        x = (short)(left + (tile.x - 4 + state->cursorX * 32));
        y = (short)(tile.y - 4 + state->cursorY * 36);
        packet = Taku_Window_Set(packet, &image, 3, 0, x, y, 0, 128, 0);
        n = 0;
        lx = 190;
        ly = 188;
        for (; n < 12; n++) {
            /* 41 copies of header prototypes, which set up compiler state for the following
               Taku_Camp_Ivent_Window; they generate no code. */
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            void SzMsg_Free(int arg0);
            int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4);
            void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
            int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);
            int RhText_SetTextureFlush(int arg0);
            cell = state->listCell[n];
            lcol = cell % 5;
            lrow = state->listRow[n];
            group = cell / 5;
            switch (state->listKind[n]) {
            case 0:
                font = (short)group + 11;
                break;
            case 1:
                font = (short)group + 14;
                break;
            case 2:
                font = (short)group + 5;
                break;
            }
            packet = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(packet, 1, font, 0, 22));
            texture = (DotrIventNameTexture *)SzMsg_GetInfo(1);
            listRect = texture->rect;
            tx = right + lx;
            rgba[2] = 0; rgba[1] = 0; rgba[0] = 0; rgba[3] = 128;
            ltx = listRect.x + (short)lcol * 22;
            texture->rect.x = ltx;
            lty = listRect.y + (short)lrow * 22;
            texture->rect.y = lty;
            texture->rect.w = 22;
            texture->rect.h = 22;
            packet = SzMsg_DrawRect_Pipe(packet, 1, tx, 99, &uv, rgba);
            rgba[2] = 255; rgba[1] = 255; rgba[0] = 255; rgba[3] = 128;
            tx = right + ly;
            texture->rect.x = ltx;
            texture->rect.y = lty;
            texture->rect.w = 22;
            texture->rect.h = 22;
            packet = SzMsg_DrawRect_Pipe(packet, 1, tx, 97, &uv, rgba);
            SzMsg_Free(1);
            lx = lx + 22;
            ly = ly + 22;
        }
    }
    return packet;
}
#ifdef __MWERKS__
#pragma conditional_move reset
#pragma opt_dead_assignments reset
#endif
