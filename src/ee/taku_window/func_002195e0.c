#include "dotr/ee/taku_window.h"

/* 0x2195e0. The switch's jump table, which this C also produces, lies at 0x30eac0. The cursor
 * is DotrTakuWindowWord and the work area is addressed by its literal through AT; the inline
 * helpers are in taku_window.h. */
#ifndef __MWERKS__
/* Host build only: this C leaves some arguments unread and some locals unset on paths
 * the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

/* MATCHING DEVICE: the work area by its literal address (taku_window.h). */
#define AT(a) DOTR_TAKU_WINDOW_AT(a)

DotrTakuWindowWord Taku_Window_Set(DotrTakuWindowWord p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx, unsigned short dy, unsigned short depth,
                    int unused, short alpha) {
    DotrTakuWindowRect *rect = rect_arg;
    int k;
    short w;
    DotrTakuWindowTex *e;
    short c;
    int hr;

    unsigned char b;
    short x0;
    short x1;
    int hg;
    short y1;
    int hb;
    unsigned char r;
    DotrTakuWindowPrim q;
    int t;
    unsigned char g;
    short y0;
    int i;

    if (style == 0) {
        r = 0x60;
        g = 0;
        b = 0;
    } else if (style == 1) {
        g = 0x60;
        r = 0;
        b = g;
    } else if (style == 2) {
        r = 0;
        g = 0;
        b = 0;
    } else if (style == 3) {
        r = 0x80;
        g = r;
        b = r;
    } else if (style == 4) {
        r = 0xFF;
        g = r;
        b = r;
    } else if (style == 5) {
        r = 0;
        g = 0;
        b = 0x60;
    } else if (style == 10) {
        t = (*(short *)AT(0x01ED006C) * 0x60) >> 8;
        g = t;
        r = 0x60 - t;
        b = g;
    } else if (style == 11) {
        t = (*(short *)AT(0x01ED006C) * 0x60) >> 8;
        g = 0x60 - t;
        r = t;
        b = g;
    }

    switch (rect->kind) {
    case 0:
    case 1:
    case 2:
    case 6:
    case 8:
    case 9:
    case 10:
        if (style == 2) {
            x0 = rect->x0 + 2;
            y0 = rect->y0 + 2;
            x1 = rect->x1 - 2;
            y1 = rect->y1 - 2;
            q.data[0] = q.data[4] = r;
            q.data[1] = q.data[5] = g;
            q.data[2] = q.data[6] = b;
            q.data[3] = q.data[7] = alpha;
            q.data[8] = q.data[12] = r;
            q.data[9] = q.data[13] = g;
            q.data[10] = q.data[14] = b;
            q.data[11] = q.data[15] = alpha;
            q.x0 = x0;
            q.y0 = y0;
            q.x1 = x1;
            q.y1 = y0;
            q.x2 = x0;
            q.y2 = y1;
            q.x3 = x1;
            q.y3 = y1;
            q.data[16] = mode;
            q.flag = 1;
            q.depth = depth;
            p = dotr_taku_window_quad(p, &q, dx, dy);
        } else if (style != 3) {
            hr = r >> 1;
            hg = g >> 1;
            hb = b >> 1;
            for (k = 0; k < 2; k++) {
                if (k == 0) {
                    x0 = rect->x0 + 2;
                    y0 = rect->y0 + 2;
                    x1 = rect->x1 - 2;
                    y1 = (rect->y0 + rect->y1) >> 1;
                    q.data[0] = q.data[4] = hr;
                    q.data[1] = q.data[5] = hg;
                    q.data[2] = q.data[6] = hb;
                    q.data[3] = q.data[7] = alpha;
                    q.data[8] = q.data[12] = r;
                    q.data[9] = q.data[13] = g;
                    q.data[10] = q.data[14] = b;
                    q.data[11] = q.data[15] = alpha;
                } else {
                    x0 = rect->x0 + 2;
                    y0 = (rect->y0 + rect->y1) >> 1;
                    x1 = rect->x1 - 2;
                    y1 = rect->y1 - 2;
                    q.data[0] = q.data[4] = r;
                    q.data[1] = q.data[5] = g;
                    q.data[2] = q.data[6] = b;
                    q.data[3] = q.data[7] = alpha;
                    q.data[8] = q.data[12] = hr;
                    q.data[9] = q.data[13] = hg;
                    q.data[10] = q.data[14] = hb;
                    q.data[11] = q.data[15] = alpha;
                }
                q.x0 = x0;
                q.y0 = y0;
                q.x1 = x1;
                q.y1 = y0;
                q.x2 = x0;
                q.y2 = y1;
                q.x3 = x1;
                q.y3 = y1;
                q.data[16] = mode;
                q.flag = 1;
                q.depth = depth;
                p = dotr_taku_window_quad(p, &q, dx, dy);
            }
        }
        for (i = 0; i < 8; i++) {
            q.index = window_tex_no[rect->kind][i];
            q.data[0] = 0x80;
            q.data[1] = 0x80;
            q.data[2] = 0x80;
            q.data[3] = 0x80;
            q.data[16] = 1;
            q.flag = 1;
            q.depth = depth;
            if (i <= 3) {
                if (i == 0) {
                    x0 = rect->x0;
                    e = &tex_uv_data[q.index];
                    y0 = rect->y0;
                    x1 = x0 + e->width;
                    y1 = y0 + e->height;
                } else if (i == 1) {
                    y0 = rect->y0;
                    x1 = rect->x1;
                    e = &tex_uv_data[q.index];
                    x0 = x1 - e->width;
                    y1 = y0 + e->height;
                } else if (i == 2) {
                    x0 = rect->x0;
                    y1 = rect->y1;
                    e = &tex_uv_data[q.index];
                    x1 = x0 + e->width;
                    y0 = y1 - e->height;
                } else {
                    x1 = rect->x1;
                    y1 = rect->y1;
                    e = &tex_uv_data[q.index];
                    x0 = x1 - e->width;
                    y0 = y1 - e->height;
                }
            } else {
                c = window_tex_no[rect->kind][0];
                if (i == 4) {
                    e = &tex_uv_data[c];
                    w = e->width;
                    x0 = rect->x0 + w;
                    x1 = rect->x1 - w;
                    y0 = rect->y0;
                    y1 = y0 + e->height;
                } else if (i == 5) {
                    y1 = rect->y1;
                    e = &tex_uv_data[c];
                    w = e->width;
                    x0 = rect->x0 + w;
                    x1 = rect->x1 - w;
                    y0 = y1 - e->height;
                } else if (i == 6) {
                    x0 = rect->x0;
                    e = &tex_uv_data[c];
                    x1 = x0 + e->width;
                    y0 = rect->y0 + e->height;
                    y1 = rect->y1 - e->height;
                } else {
                    e = &tex_uv_data[c];
                    x1 = rect->x1;
                    x0 = x1 - e->width;
                    y0 = rect->y0 + e->height;
                    y1 = rect->y1 - e->height;
                }
            }
            q.x0 = x0;
            q.y0 = y0;
            q.x1 = x1;
            q.y1 = y1;
            p = dotr_taku_window_sprite(p, &q, dx, dy);
        }
        break;
    case 3:
        for (i = 0; i < 3; i++) {
            q.index = window_tex_no[rect->kind][i];
            q.data[0] = r;
            q.data[1] = g;
            q.data[2] = b;
            q.data[3] = alpha;
            q.data[16] = mode;
            q.flag = 1;
            q.depth = depth;
            if (i <= 1) {
                if (i == 0) {
                    x0 = rect->x0;
                    y0 = rect->y0;
                    e = &tex_uv_data[q.index];
                    x1 = x0 + e->width;
                    y1 = y0 + e->height;
                } else {
                    x1 = rect->x1;
                    y1 = rect->y1;
                    e = &tex_uv_data[q.index];
                    x0 = x1 - e->width;
                    y0 = y1 - e->height;
                }
            } else {
                w = tex_uv_data[(short)window_tex_no[rect->kind][0]].width;
                x0 = rect->x0 + w;
                y0 = rect->y0;
                x1 = rect->x1 - w;
                y1 = rect->y1;
            }
            q.x0 = x0;
            q.y0 = y0;
            q.x1 = x1;
            q.y1 = y1;
            p = dotr_taku_window_sprite(p, &q, dx, dy);
        }
        break;
    case 4:
        hr = r >> 1;
        hg = g >> 1;
        hb = b >> 1;
        x0 = rect->x0 + 2;
        y0 = rect->y0 + 2;
        x1 = rect->x1 - 2;
        y1 = rect->y1;
        q.data[0] = q.data[4] = hr;
        q.data[1] = q.data[5] = hg;
        q.data[2] = q.data[6] = hb;
        q.data[3] = q.data[7] = alpha;
        q.data[8] = q.data[12] = hr;
        q.data[9] = q.data[13] = hg;
        q.data[10] = q.data[14] = hb;
        q.data[11] = q.data[15] = alpha;
        q.x0 = x0;
        q.y0 = y0;
        q.x1 = x1;
        q.y1 = y0;
        q.x2 = x0;
        q.y2 = y1;
        q.x3 = x1;
        q.y3 = y1;
        q.data[16] = 1;
        q.flag = 1;
        q.depth = depth;
        p = dotr_taku_window_quad(p, &q, dx, dy);
        for (i = 0; i < 4; i++) {
            q.index = window_tex_no[rect->kind][i];
            q.data[0] = 0x80;
            q.data[1] = 0x80;
            q.data[2] = 0x80;
            q.data[3] = 0x80;
            q.data[16] = 1;
            q.flag = 1;
            q.depth = depth;
            if (i <= 2) {
                if (i == 0) {
                    x0 = rect->x0;
                    y0 = rect->y0;
                    e = &tex_uv_data[q.index];
                    x1 = x0 + e->width;
                    y1 = y0 + e->height;
                } else if (i == 1) {
                    x1 = rect->x1;
                    y1 = rect->y1;
                    e = &tex_uv_data[q.index];
                    x0 = x1 - e->width;
                    y0 = y1 - e->height;
                } else {
                    w = tex_uv_data[(short)window_tex_no[rect->kind][0]].width;
                    y0 = rect->y0;
                    x0 = rect->x0 + w;
                    x1 = rect->x1 - w;
                    y1 = rect->y1;
                }
                q.x0 = x0;
                q.y0 = y0;
                q.x1 = x1;
                q.y1 = y1;
                p = dotr_taku_window_sprite(p, &q, dx, dy);
            } else {
                x0 = (rect->x0 + rect->x1) >> 1;
                y0 = (rect->y0 + rect->y1) >> 1;
                q.x0 = x0 - 0x10;
                q.y0 = y0 - 0xE;
                q.x1 = x0 + 0x10;
                q.y1 = y0 - 0xE;
                q.x2 = x0 - 0x10;
                q.y2 = y0 + 2;
                q.x3 = x0 + 0x10;
                q.y3 = y0 + 2;
                p = (DotrTakuWindowWord)Taku_FT4_Set((void *)p, &q, dx, dy, 0);
            }
        }
        break;
    case 5:
        if (style == 2) {
            x0 = rect->x0 + 6;
            y0 = rect->y0 + 6;
            x1 = rect->x1 - 6;
            y1 = rect->y1;
            q.data[0] = q.data[4] = r;
            q.data[1] = q.data[5] = g;
            q.data[2] = q.data[6] = b;
            q.data[3] = q.data[7] = alpha;
            q.data[8] = q.data[12] = r;
            q.data[9] = q.data[13] = g;
            q.data[10] = q.data[14] = b;
            q.data[11] = q.data[15] = alpha;
            q.x0 = x0;
            q.y0 = y0;
            q.x1 = x1;
            q.y1 = y0;
            q.x2 = x0;
            q.y2 = y1;
            q.x3 = x1;
            q.y3 = y1;
            q.data[16] = mode;
            q.flag = 1;
            q.depth = depth;
            p = dotr_taku_window_quad(p, &q, dx, dy);
        }
        for (i = 0; i < 16; i++) {
            q.data[i] = 0x80;
        }
        for (i = 0; i < 5; i++) {
            q.data[16] = 1;
            q.flag = 1;
            q.depth = depth;
            if (style == 0) {
                if (i <= 1) {
                    if (i == 0) {
                        q.index = window_tex_no[rect->kind][2];
                        y0 = rect->y0;
                        x0 = rect->x0;
                        e = &tex_uv_data[q.index];
                        x1 = x0 + e->width;
                        y1 = y0 + e->height;
                    } else {
                        q.index = window_tex_no[rect->kind][3];
                        y0 = rect->y0;
                        x1 = rect->x1;
                        e = &tex_uv_data[q.index];
                        x0 = x1 - e->width;
                        y1 = y0 + e->height;
                    }
                } else {
                    c = window_tex_no[rect->kind][0];
                    if (i == 2) {
                        q.index = window_tex_no[rect->kind][5];
                        y0 = rect->y0;
                        e = &tex_uv_data[c];
                        w = e->width;
                        x0 = rect->x0 + w;
                        x1 = rect->x1 - w;
                        y1 = y0 + e->height;
                    } else if (i == 3) {
                        q.index = window_tex_no[rect->kind][6];
                        x0 = rect->x0;
                        e = &tex_uv_data[c];
                        x1 = x0 + e->width;
                        y0 = rect->y0 + e->height;
                        y1 = rect->y1;
                    } else {
                        q.index = window_tex_no[rect->kind][7];
                        e = &tex_uv_data[c];
                        x1 = rect->x1;
                        x0 = x1 - e->width;
                        y0 = rect->y0 + e->height;
                        y1 = rect->y1;
                    }
                }
                q.x0 = x0;
                q.y0 = y1;
                q.x1 = x1;
                q.y1 = y0;
                p = dotr_taku_window_flipped(p, &q, dx, dy);
            } else {
                if (i <= 1) {
                    if (i == 0) {
                        q.index = window_tex_no[rect->kind][0];
                        x0 = rect->x0;
                        y0 = rect->y0;
                        e = &tex_uv_data[q.index];
                        x1 = x0 + e->width;
                        y1 = y0 + e->height;
                    } else {
                        q.index = window_tex_no[rect->kind][1];
                        y0 = rect->y0;
                        x1 = rect->x1;
                        e = &tex_uv_data[q.index];
                        x0 = x1 - e->width;
                        y1 = y0 + e->height;
                    }
                } else {
                    c = window_tex_no[rect->kind][0];
                    if (i == 2) {
                        q.index = window_tex_no[rect->kind][4];
                        e = &tex_uv_data[c];
                        w = e->width;
                        x0 = rect->x0 + w;
                        x1 = rect->x1 - w;
                        y0 = rect->y0;
                        y1 = y0 + e->height;
                    } else if (i == 3) {
                        q.index = window_tex_no[rect->kind][6];
                        x0 = rect->x0;
                        e = &tex_uv_data[c];
                        x1 = x0 + e->width;
                        y0 = rect->y0 + e->height;
                        y1 = rect->y1;
                    } else {
                        q.index = window_tex_no[rect->kind][7];
                        e = &tex_uv_data[c];
                        x1 = rect->x1;
                        x0 = x1 - e->width;
                        y0 = rect->y0 + e->height;
                        y1 = rect->y1;
                    }
                }
                q.x0 = x0;
                q.y0 = y0;
                q.x1 = x1;
                q.y1 = y1;
                p = dotr_taku_window_sprite(p, &q, dx, dy);
            }
        }
        break;
    case 7:
        if (style == 2) {
            x0 = rect->x0 + 6;
            y0 = rect->y0;
            x1 = rect->x1 - 6;
            y1 = rect->y1 - 6;
            q.data[0] = q.data[4] = r;
            q.data[1] = q.data[5] = g;
            q.data[2] = q.data[6] = b;
            q.data[3] = q.data[7] = alpha;
            q.data[8] = q.data[12] = r;
            q.data[9] = q.data[13] = g;
            q.data[10] = q.data[14] = b;
            q.data[11] = q.data[15] = alpha;
            q.x0 = x0;
            q.y0 = y0;
            q.x1 = x1;
            q.y1 = y0;
            q.x2 = x0;
            q.y2 = y1;
            q.x3 = x1;
            q.y3 = y1;
            q.data[16] = mode;
            q.flag = 1;
            q.depth = depth;
            p = dotr_taku_window_quad(p, &q, dx, dy);
        }
        for (i = 0; i < 16; i++) {
            q.data[i] = 0x80;
        }
        for (i = 0; i < 5; i++) {
            q.data[16] = 1;
            q.flag = 1;
            q.depth = depth;
            if (style == 0) {
                if (i < 2) {
                    if (i == 0) {
                        q.index = window_tex_no[rect->kind][0];
                        x0 = rect->x0;
                        y1 = rect->y1;
                        e = &tex_uv_data[q.index];
                        x1 = x0 + e->width;
                        y0 = y1 - e->height;
                    } else if (i == 1) {
                        q.index = window_tex_no[rect->kind][1];
                        x1 = rect->x1;
                        y1 = rect->y1;
                        e = &tex_uv_data[q.index];
                        x0 = x1 - e->width;
                        y0 = y1 - e->height;
                    }
                } else {
                    c = window_tex_no[rect->kind][0];
                    if (i == 2) {
                        q.index = window_tex_no[rect->kind][4];
                        e = &tex_uv_data[c];
                        w = e->width;
                        y1 = rect->y1;
                        x0 = rect->x0 + w;
                        x1 = rect->x1 - w;
                        y0 = y1 - e->height;
                    } else if (i == 3) {
                        q.index = window_tex_no[rect->kind][6];
                        e = &tex_uv_data[c];
                        x0 = rect->x0;
                        y0 = rect->y0;
                        x1 = x0 + e->width;
                        y1 = rect->y1 - e->height;
                    } else {
                        q.index = window_tex_no[rect->kind][7];
                        e = &tex_uv_data[c];
                        x1 = rect->x1;
                        y0 = rect->y0;
                        x0 = x1 - e->width;
                        y1 = rect->y1 - e->height;
                    }
                }
                q.x0 = x0;
                q.y0 = y1;
                q.x1 = x1;
                q.y1 = y0;
                p = dotr_taku_window_flipped(p, &q, dx, dy);
            } else {
                if (i <= 1) {
                    if (i == 0) {
                        q.index = window_tex_no[rect->kind][2];
                        x0 = rect->x0;
                        y1 = rect->y1;
                        e = &tex_uv_data[q.index];
                        x1 = x0 + e->width;
                        y0 = y1 - e->height;
                    } else {
                        q.index = window_tex_no[rect->kind][3];
                        x1 = rect->x1;
                        y1 = rect->y1;
                        e = &tex_uv_data[q.index];
                        x0 = x1 - e->width;
                        y0 = y1 - e->height;
                    }
                } else {
                    c = window_tex_no[rect->kind][0];
                    if (i == 2) {
                        q.index = window_tex_no[rect->kind][5];
                        e = &tex_uv_data[c];
                        w = e->width;
                        y1 = rect->y1;
                        x0 = rect->x0 + w;
                        x1 = rect->x1 - w;
                        y0 = y1 - e->height;
                    } else if (i == 3) {
                        q.index = window_tex_no[rect->kind][6];
                        e = &tex_uv_data[c];
                        x0 = rect->x0;
                        y0 = rect->y0;
                        x1 = x0 + e->width;
                        y1 = rect->y1 - e->height;
                    } else {
                        q.index = window_tex_no[rect->kind][7];
                        e = &tex_uv_data[c];
                        x1 = rect->x1;
                        y0 = rect->y0;
                        x0 = x1 - e->width;
                        y1 = rect->y1 - e->height;
                    }
                }
                q.x0 = x0;
                q.y0 = y0;
                q.x1 = x1;
                q.y1 = y1;
                p = dotr_taku_window_sprite(p, &q, dx, dy);
            }
        }
        break;
    }
    return p;
}
