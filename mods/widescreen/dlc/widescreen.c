/* Widescreen: the view's aspect, and scaling of the 2D interface. Part of DLC.BIN (see
 * header.c); the executable forwards to these functions once the DLC is loaded.
 *
 * The picture is shown 4/3 wider than it is drawn, so 2D pieces look stretched. The functions
 * listed in mod.json call WsUi_SortPrim or WsUi_SortPrim3D in the executable instead of
 * ExSortPrim_Pipe; those land here. Both build the packet as before, then move its vertices.
 *
 * DlcWs_SortPrim, for the interface proper: each piece is drawn at WSUI_SIZE percent of its size
 * (default 80) with its width also multiplied by 3/4 to undo the stretch, and keeps its place
 * relative to an anchor point:
 *
 *   WSUI_ANCHOR 0 (default)  the centre of the screen: the whole layout shrinks in place.
 *   WSUI_ANCHOR 1            a border or the centre, chosen by WsUi_Anchor from where the
 *                            piece lies.
 *
 * A piece at least 600 pixels wide (a backdrop, a fade) is left alone.
 *
 * DlcWs_SortPrim3D, for 2D pieces that mark something in the 3D picture (the frame around a card
 * in the hand): the 3D picture's width is 3/4 of what it was around the centre and its height is
 * unchanged, so these pieces get exactly that. */

typedef unsigned long long u64;
typedef unsigned short u16;

extern int ExSortPrim_Pipe(int packet, int source, int table, int z);
extern int gRhView_nStackIndex[];
extern char gRhView_avl[];
extern int SzGame_GetMode(int which);

#ifndef WSUI_SIZE
#define WSUI_SIZE 80
#endif
#ifndef WSUI_ANCHOR
#define WSUI_ANCHOR 0
#endif
/* How far apart two pieces may be and still be one group, in pixels. */
#ifndef WSUI_GAP
#define WSUI_GAP 6
#endif
/* How much two pieces must overlap, in pixels both ways, to be one group even when they lie in
 * different thirds of the screen. */
#ifndef WSUI_OVERLAP
#define WSUI_OVERLAP 4
#endif

/* Screen coordinates in packets are 12.4 fixed point with the screen centre at 2048. */
#define CENTRE (2048 * 16)
#define HALF_W (320 * 16)
#define HALF_H (224 * 16)
/* Factors in 1/256. */
#define SCALE_X (256 * 3 * WSUI_SIZE / 400)
#define SCALE_Y (256 * WSUI_SIZE / 100)
/* GS registers 4, 5, 12 and 13 are the vertex positions (XYZF2, XYZ2, XYZF3, XYZ3). */
#define IS_VERTEX(q) (((unsigned int)(q)[1] & ~9u) == 4)

/* What is being drawn, set by the wrappers at the end of this file around game functions:
 *   WS_NORMAL  the interface: pieces scaled, touching pieces kept together;
 *   WS_SPLIT   the duel's information window (SzDuel_DrawWindow_Pipe): side-by-side boxes in
 *              different thirds go to their own borders (see WsUi_Anchor);
 *   WS_RAW     a card face (SzDuel_SetupCard_Pipe): left as the game draws it, because its frame
 *              is not scaled either (in the hand it follows the 3D picture already);
 *   WS_3D      the hand cursor (SzDuel_DrawHandCursor_Pipe): marks cards of the 3D picture, so it
 *              gets what the 3D picture got, 3/4 of the width around the centre;
 *   WS_CORNER  the campaign map's system panel (Taku_Camp_SYS_ICON): every piece keeps its
 *              distance from the bottom right corner, the panel open (it slides up) or closed, and
 *              its pieces never join the pieces drawn around it. */
#define WS_NORMAL 0
#define WS_SPLIT 1
#define WS_RAW 2
#define WS_3D 3
#define WS_CORNER 4
/*   WS_TEKI    the campaign map's opponent panel (Taku_Camp_TEKI_WINDOW): its pieces in the top
 *              rows and outside the right third (name bar, deck cost, the Deck Leader box) keep
 *              their distance from the left border, so the Deck Leader box still touches the
 *              backing of the leader's model (x 16 to 128, TkCamp_Draw_After) at x 128. */
#define WS_TEKI 5
#define TEKI_TOP 120   /* the Deck Leader box ends at 120; the place-name tip starts at 120 or below */
/*   WS_DIALOG  the message and choice dialogs (SzDuel_DrawDialog_Pipe): every piece scaled about
 *              the screen's centre, as one dialog, and never anchored to a panel drawn earlier
 *              (on the campaign map they lie over the left-anchored panels, which pulled the
 *              return-to-title dialog to the left and left its pieces apart). */
#define WS_DIALOG 6
static int ws_mode;

/* The deck builder (SzGame_GetMode 6) shrinks towards the top of the screen, which leaves room for
 * its longer card lists (mod.json: seven rows instead of five). */
#define DECK_BUILDER_MODE 6

struct WsGroup {
    int x0, x1, y0, y1, ax, ay;
};

/* The current group of pieces: its box in original coordinates and its anchor. */
static struct WsGroup group;

/* The groups started this frame (the last RING of them), for pieces drawn inside a panel after
 * other pieces came in between (the deck cost and its icon in the box of the campaign map's
 * opponent panel). The frame counter of the game block (0x317254) tells frames apart. */
#define RING 24
#define FRAME_COUNTER (*(volatile int *)0x317254)
static struct WsGroup ring[RING];
static int ring_count, ring_next, ring_current = -1, ring_frame;

static void forget_group(void)
{
    group.x0 = 0x7fffffff;
    group.x1 = -0x7fffffff;
    group.y0 = 0x7fffffff;
    group.y1 = -0x7fffffff;
    ring_current = -1;
}

/* Scales the vertices of the (value, register) pairs in [first, end) about (ax, ay) by sx and sy
 * (in 1/256); returns end. */
static int WsUi_Scale(u64 *first, u64 *end, int ax, int ay, int sx, int sy)
{
    u64 *q;

    for (q = first; q < end; q += 2) {
        if (IS_VERTEX(q)) {
            u16 *p = (u16 *)q;
            p[0] = ax + (((int)p[0] - ax) * sx >> 8);
            p[1] = ay + (((int)p[1] - ay) * sy >> 8);
        }
    }
    return (int)end;
}

/* Sets group.ax and group.ay for the piece with the box (x0..x1, y0..y1). By where the piece
 * lies, the left, middle or right third keeps its distance from the left border, centre or right
 * border, and the same for top, middle and bottom.
 *
 * Pieces drawn one after another form a group that keeps the anchor of its first piece, so a
 * panel lying across the border between two thirds is not pulled apart: a piece joins the group
 * when it touches it (within WSUI_GAP). In the duel's information window (WS_SPLIT) a touching
 * piece must also have the same anchor anyway, or really overlap the group (text on a backing, a
 * frame's corner on its edge): there the boxes that only stand side by side in different thirds
 * (the command box, card name and field type along the bottom) stay apart, each going to its own
 * border. Elsewhere (card details, deck builder, campaign map) side-by-side pieces are parts of
 * one panel, row or bar and stay together. */
static void WsUi_Anchor(int x0, int x1, int y0, int y1)
{
    int mx = (x0 + x1) >> 1;
    int my = (y0 + y1) >> 1;
    int ax = CENTRE;
    int ay = CENTRE;
    int i, best, area;

    if (FRAME_COUNTER != ring_frame) {
        ring_frame = FRAME_COUNTER;
        ring_count = 0;
        ring_next = 0;
        ring_current = -1;
    }
    if (mx < CENTRE - HALF_W / 3) ax = CENTRE - HALF_W;
    else if (mx > CENTRE + HALF_W / 3) ax = CENTRE + HALF_W;
    if (my < CENTRE - HALF_H / 3) ay = CENTRE - HALF_H;
    else if (my > CENTRE + HALF_H / 3) ay = CENTRE + HALF_H;
    if (ws_mode == WS_CORNER) {
        ax = CENTRE + HALF_W;
        ay = CENTRE + HALF_H;
    } else if (ws_mode == WS_DIALOG) {
        ax = CENTRE;
        ay = CENTRE;
    } else if (SzGame_GetMode(0) == DECK_BUILDER_MODE) {
        /* All about the centre across: its panels slide between the chest and the deck, and a
         * piece going from one third into another would jump to the other anchor on the way. */
        ax = CENTRE;
        ay = CENTRE - HALF_H;
    }

    if (x0 <= group.x1 + WSUI_GAP * 16 && x1 + WSUI_GAP * 16 >= group.x0
        && y0 <= group.y1 + WSUI_GAP * 16 && y1 + WSUI_GAP * 16 >= group.y0
        && (ws_mode != WS_SPLIT || (ax == group.ax && ay == group.ay)
            || (x0 <= group.x1 - WSUI_OVERLAP * 16 && x1 >= group.x0 + WSUI_OVERLAP * 16
                && y0 <= group.y1 - WSUI_OVERLAP * 16 && y1 >= group.y0 + WSUI_OVERLAP * 16))) {
        if (x0 < group.x0) group.x0 = x0;
        if (x1 > group.x1) group.x1 = x1;
        if (y0 < group.y0) group.y0 = y0;
        if (y1 > group.y1) group.y1 = y1;
    } else {
        group.x0 = x0;
        group.x1 = x1;
        group.y0 = y0;
        group.y1 = y1;
        group.ax = ax;
        group.ay = ay;
        /* A piece lying inside a group started earlier this frame takes that group's anchor
         * (the smallest such group). */
        if (ws_mode != WS_CORNER && ws_mode != WS_DIALOG) {
            best = -1;
            area = 0;
            for (i = 0; i < ring_count; i++) {
                struct WsGroup *r = &ring[i];
                int a = (r->x1 - r->x0) * (r->y1 - r->y0);
                if (x0 >= r->x0 - 32 && x1 <= r->x1 + 32 && y0 >= r->y0 - 32 && y1 <= r->y1 + 32
                    && (best < 0 || a < area)) {
                    best = i;
                    area = a;
                }
            }
            if (best >= 0) {
                group.ax = ring[best].ax;
                group.ay = ring[best].ay;
            }
        }
        if (ws_mode == WS_TEKI && mx <= CENTRE + HALF_W / 3
            && y0 < CENTRE - HALF_H + TEKI_TOP * 16) {
            group.ax = CENTRE - HALF_W;
        }
        ring_current = ring_next;
        ring_next = (ring_next + 1) % RING;
        if (ring_count < RING) {
            ring_count++;
        }
    }
    if (ring_current >= 0) {
        ring[ring_current] = group;
    }
}

extern void Dlc_LiveCheck(void);
extern void DlcButtons_Update(void);
extern void DlcSleeves_Update(void);

int DlcWs_SortPrim(int packet, int source, int table, int z)
{
    /* The packet starts with eight doublewords of header; pairs of (value, register) follow. */
    u64 *first = (u64 *)packet + 8;
    u64 *end;
    u64 *q;
    int x0 = 0xffff, x1 = 0, y0 = 0xffff, y1 = 0;

    Dlc_LiveCheck();
    DlcButtons_Update();
    DlcSleeves_Update();
    end = (u64 *)ExSortPrim_Pipe(packet, source, table, z);

    if (ws_mode == WS_RAW) {
        return (int)end;
    }
    if (ws_mode == WS_3D) {
        return WsUi_Scale(first, end, CENTRE, CENTRE, 192, 256);
    }

    for (q = first; q < end; q += 2) {
        if (IS_VERTEX(q)) {
            int x = ((u16 *)q)[0];
            int y = ((u16 *)q)[1];
            if (x < x0) x0 = x;
            if (x > x1) x1 = x;
            if (y < y0) y0 = y;
            if (y > y1) y1 = y;
        }
    }
    if (x1 < x0 || x1 - x0 >= 600 * 16) {
        return (int)end;
    }
#if WSUI_ANCHOR
    WsUi_Anchor(x0, x1, y0, y1);
    return WsUi_Scale(first, end, group.ax, group.ay, SCALE_X, SCALE_Y);
#else
    return WsUi_Scale(first, end, CENTRE, CENTRE, SCALE_X, SCALE_Y);
#endif
}

int DlcWs_SortPrim3D(int packet, int source, int table, int z)
{
    u64 *first = (u64 *)packet + 8;

    return WsUi_Scale(first, (u64 *)ExSortPrim_Pipe(packet, source, table, z), CENTRE, CENTRE, 192, 256);
}

/* RhView_SetAspect (now a jump to WsView_SetAspect in the executable, which forwards here).
 * Everywhere but on the campaign map the horizontal factor is scaled by 3/4: more is seen at the
 * sides and nothing is lost. The campaign map (game mode 11) ends not far outside the original
 * picture, so there the vertical factor is scaled by 4/3 instead: the sides stay where they were
 * and the picture loses a strip at the top and at the bottom.
 * The view table is 0x1a0 bytes an entry; the factors are at +0x158 and +0x15c.
 * The map counts while it is the mode asked for (0) or still the mode drawn (1, SzGame_ModeDraw):
 * on the way into a duel the mode asked for changes first, and the map's last frames (the fade)
 * would otherwise be drawn narrower, showing where the map ends. */
static int on_map(void)
{
    return SzGame_GetMode(0) == 11 || SzGame_GetMode(1) == 11;
}

int DlcWs_SetAspect(float a, float b)
{
    float *v = (float *)(gRhView_avl + gRhView_nStackIndex[0] * 0x1a0 + 0x158);

    Dlc_LiveCheck();
    if (on_map()) {
        b *= 4.0f / 3.0f;
    } else {
        a *= 0.75f;
    }
    v[0] = a;
    v[1] = b;
    return 1;
}

/* Wrappers (mod.json "wrap"): each sets ws_mode while the game's function runs, which it calls
 * through its trampoline (tools/mod.py). These take their arguments in registers only. */
typedef int (*WsDraw)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_SzDuel_SetupCard_Pipe[];
extern unsigned int DlcTramp_SzDuel_DrawWindow_Pipe[];
extern unsigned int DlcTramp_SzDuel_DrawHandCursor_Pipe[];

static int wrapped(unsigned int *trampoline, int mode, int a0, int a1, int a2, int a3, int a4, int a5,
                   int a6, int a7)
{
    int old = ws_mode;
    int result;

    if (mode == WS_CORNER || mode == WS_DIALOG) {
        forget_group();
    }
    ws_mode = mode;
    result = ((WsDraw)trampoline)(a0, a1, a2, a3, a4, a5, a6, a7);
    ws_mode = old;
    if (mode == WS_CORNER || mode == WS_DIALOG) {
        forget_group();
    }
    return result;
}

extern unsigned int DlcTramp_Taku_Camp_SYS_ICON[];

/* For pieces the DLC draws itself as one centred box (the TROPHIES window, achievements.c): scaled
 * about the screen's centre, joined to nothing drawn before. Returns what to give DlcWs_DialogEnd. */
int DlcWs_DialogBegin(void)
{
    int old = ws_mode;

    forget_group();
    ws_mode = WS_DIALOG;
    return old;
}

void DlcWs_DialogEnd(int old)
{
    ws_mode = old;
    forget_group();
}

extern int DlcMapDebug_Draw(int cursor);
extern int DlcAch_DrawPopup(int cursor);
extern int DlcAch_SysIcon(int cursor);
extern int DlcAch_DrawResults(int cursor);
extern int DlcBosses_Draw2D(int cursor);

extern int DlcPort_Taku_Camp_TEKI_WINDOW(int cursor);
extern void DlcBosses_WindowBegin(void);   /* bosses.c: the second world's bosses in the panel */
extern void DlcBosses_WindowEnd(void);

int DlcWs_TekiWindow(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int cursor;

    DlcBosses_WindowBegin();
    /* The ported copy (mod.json): the Deck Masters' panel (bosses.c DlcBosses_PanelText ...). */
    cursor = wrapped((unsigned int *)DlcPort_Taku_Camp_TEKI_WINDOW, WS_TEKI, a0, a1, a2, a3, a4, a5, a6, a7);
    DlcBosses_WindowEnd();
    return cursor;
}

/* Taku_Camp_Teki_Model_Disp (ported, mod.json) draws the opponent's deck leader into a picture of
 * the whole screen and shows a piece of it in the panel's box. That piece now goes through
 * DlcWs_SortPrim like the box, so it is scaled with it; and the picture is drawn with the shape of
 * the original screen (the map's view is 4/3 taller, DlcWs_SetAspect), so the model keeps its
 * shape in the smaller piece. Called on the view pushed for the model alone. */
void DlcWs_ModelAspect(void)
{
    float *v = (float *)(gRhView_avl + gRhView_nStackIndex[0] * 0x1a0 + 0x158);

    if (on_map()) {
        v[1] *= 0.75f;
    }
}

int DlcWs_SysIcon(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int cursor = wrapped(DlcTramp_Taku_Camp_SYS_ICON, WS_CORNER, a0, a1, a2, a3, a4, a5, a6, a7);
    int old = ws_mode;

    /* The trophy over the panel's "?" and its label (achievements.c), anchored as the panel is. */
    ws_mode = WS_CORNER;
    forget_group();
    cursor = DlcAch_SysIcon(cursor);
    forget_group();
    ws_mode = old;

    /* The map debug overlay, in screen units as the projection gives them. */
    ws_mode = WS_RAW;
    cursor = DlcMapDebug_Draw(cursor);
    cursor = DlcBosses_Draw2D(cursor);
    /* An achievement earned in the last duel (achievements.c), scaled about the centre as one box. */
    ws_mode = WS_DIALOG;
    forget_group();
    cursor = DlcAch_DrawResults(cursor);
    forget_group();
    cursor = DlcAch_DrawPopup(cursor);
    forget_group();
    ws_mode = old;
    return cursor;
}

/* The deck builder's background (lSzDuel_DrawBuildDeck_Base_Pipe, two gradients) becomes the
 * wallpaper of the mod (deck_wallpaper.png), when there is one: its top and bottom halves are two
 * pictures after the new cards' (card_pictures.c), tiled across the screen at their own shape (a
 * screen unit is 3 pixels wide and 2.41 high on a 16:9 picture). The original is drawn while the
 * pictures are still loading. */
extern unsigned int DlcTramp_lSzDuel_DrawBuildDeck_Base_Pipe[];
extern int dlc_wallpaper;
extern int dlc_wallpaper_width;            /* screen units of one tile */
extern unsigned char dlc_wallpaper_colours[8];  /* its darker and lighter colours, b g r a */
extern void *SzText_GetReadyItem(int group, int id);
extern int lSzDuel_DrawWindow_PicItem_Pipe(int cursor, int id, int x, int y, int width, int height, int z);
extern void DlcPic_Ensure(void);
extern int DlcPic_Wallpaper(void);
extern int DlcShop_IsOpen(void);
extern int DlcShop_DrawBackground(int cursor);
/* The two colours of the original background (lSzDuel_DrawBuildDeck_Base_Pipe), b g r a. */
#define BASE_COLOURS ((unsigned char *)0x2be850)

int DlcWs_DeckBase(int cursor, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int top;
    int x, old, i;

    DlcPic_Ensure();
    if (DlcShop_IsOpen()) {
        /* The card shop (shop.c): the Kame Game shop, over the whole screen. */
        old = ws_mode;
        ws_mode = WS_RAW;
        cursor = DlcShop_DrawBackground(cursor);
        ws_mode = old;
        return cursor;
    }
    top = DlcPic_Wallpaper();
    if (!dlc_wallpaper || SzText_GetReadyItem(1, top) == 0 || SzText_GetReadyItem(1, top + 1) == 0) {
        /* Still loading: the original background in the wallpaper's own colours. */
        if (dlc_wallpaper) {
            for (i = 0; i < 8; i++) {
                BASE_COLOURS[i] = dlc_wallpaper_colours[i];
            }
        }
        return ((WsDraw)DlcTramp_lSzDuel_DrawBuildDeck_Base_Pipe)(cursor, a1, a2, a3, a4, a5, a6, a7);
    }
    old = ws_mode;
    ws_mode = WS_RAW;
    for (x = 0; x < 640; x += dlc_wallpaper_width) {
        cursor = lSzDuel_DrawWindow_PicItem_Pipe(cursor, top, x, 0, dlc_wallpaper_width, 224,
                                                 *(int *)0x3180e8);
        cursor = lSzDuel_DrawWindow_PicItem_Pipe(cursor, top + 1, x, 224, dlc_wallpaper_width, 224,
                                                 *(int *)0x3180e8);
    }
    ws_mode = old;
    return cursor;
}

extern int DlcFrame_Before(int cursor, const unsigned char *unit, const unsigned char *draw);
extern void DlcFrame_After(void);
extern void DlcSleeves_ForCard(unsigned char *unit);
extern void DlcSleeves_NoUnit(void);
extern void DlcSleeves_Drawn(void);
extern unsigned int DlcTramp_SzDuel_DrawDialog_Pipe[];

int DlcWs_DrawDialog(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    extern int DlcAch_DuelWindow(int cursor);
    int cursor = wrapped(DlcTramp_SzDuel_DrawDialog_Pipe, WS_DIALOG, a0, a1, a2, a3, a4, a5, a6, a7);
    int old = ws_mode;

    /* The star chips a won campaign duel pays, in its YOU WIN window (achievements.c). */
    ws_mode = WS_DIALOG;
    forget_group();
    cursor = DlcAch_DuelWindow(cursor);
    forget_group();
    ws_mode = old;
    return cursor;
}

int DlcWs_SetupCard(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    /* The card back: the player's sleeve or the game's swirl, by the card's owner (sleeves.c).
     * A card showing its back comes without its unit (SzDuel_GetCardNode said its face is turned
     * away): the field's cards were set before that (SzDuel_DrawCardRequest_Pipe, ported), so
     * the setting is left as it is then. */
    int result;

    if (a1 != 0) {
        DlcSleeves_ForCard((unsigned char *)a1);
    } else {
        DlcSleeves_NoUnit();
    }
    /* The card's frame colour (frames.c): set up before, undone after. */
    a0 = DlcFrame_Before(a0, (const unsigned char *)a1, (const unsigned char *)a2);
    result = wrapped(DlcTramp_SzDuel_SetupCard_Pipe, WS_RAW, a0, a1, a2, a3, a4, a5, a6, a7);
    DlcFrame_After();
    DlcSleeves_Drawn();
    return result;
}

int DlcWs_DrawWindow(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return wrapped(DlcTramp_SzDuel_DrawWindow_Pipe, WS_SPLIT, a0, a1, a2, a3, a4, a5, a6, a7);
}

int DlcWs_DrawHandCursor(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return wrapped(DlcTramp_SzDuel_DrawHandCursor_Pipe, WS_3D, a0, a1, a2, a3, a4, a5, a6, a7);
}
