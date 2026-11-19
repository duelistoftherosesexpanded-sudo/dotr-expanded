/* A debug overlay for the campaign map (dlc_options.h DLC_DEBUG_MAP): world coordinates for
 * placing new opponent podiums.
 *
 *   L3          show or hide the overlay
 *   (nothing)   the probe follows the map's hand cursor: the point of the ground under it
 *   right stick move the probe yourself (x and z; hold R1 to move ten times faster)
 *   R3          follow the hand cursor again
 *
 * It shows the probe's x and z with a + where it is, and the nearest location's number, x and z.
 * The locations are the 22 records of 52 bytes at 0x298ba0 (x, height, z as floats); their name
 * plates are placed through Taku_3D_2D_Set at height + 8.65, as here. Drawn after the system panel
 * (Taku_Camp_SYS_ICON, wrapped in widescreen.c), in screen units (640 x 448). */

#include "dlc_options.h"

extern int SzPad_Check(int pad, int mode, int mask);
extern int SzPad_GetAnalogInfo(int pad, int which);
extern int SzPad_IsAnalog(int pad);
extern void Taku_3D_2D_Set(int *x, int *y, void *point);
extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int font, int flags, int count,
                                       signed char *text, unsigned char *colour);
extern int SzGame_GetMode(int which);
extern int DlcBosses_Debug(int cursor, int x, int y);

#define LOCATIONS ((float *)0x298ba0)   /* 13 floats a record */
#define SELECTED (*(unsigned char *)(0x299e10 + 198))
#define CAMPAIGN_MODE 11
#define PAD_R1 0x8
#define PAD_L3 0x200
#define PAD_R3 0x400

static int shown;
static int placed;
static float probe[4];

/* Writes value as text (up to five digits, M in front when negative: the game's font has only
 * A-Z, 0-9 and ?, lSzDuel_GetFontIndex); returns the length. */
static int number(signed char *out, int value)
{
    char digits[8];
    int n = 0, length = 0;

    if (value < 0) {
        out[length++] = 'M';
        value = -value;
    }
    do {
        digits[n++] = '0' + value % 10;
        value /= 10;
    } while (value != 0 && n < 6);
    while (n > 0) {
        out[length++] = digits[--n];
    }
    return length;
}

/* Flags 1: proportional, starting at x (3 would centre the text on x). */
static int text(int cursor, int x, int y, signed char *s, int length)
{
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 0, 1, length, s, 0);
}

static int line(int cursor, int x, int y, const char *label, int a, const char *label2, int b,
                const char *label3, int c)
{
    signed char s[64];
    int n = 0;
    const char *p;

    for (p = label; *p; p++) s[n++] = *p;
    n += number(s + n, a);
    for (p = label2; *p; p++) s[n++] = *p;
    n += number(s + n, b);
    if (label3 != 0) {
        for (p = label3; *p; p++) s[n++] = *p;
        n += number(s + n, c);
    }
    return text(cursor, x, y, s, n);
}

static int stick(int axis)
{
    int d = (SzPad_GetAnalogInfo(0, axis) & 0xff) - 0x80;

    if (d > 0x30) return d - 0x30;
    if (d < -0x30) return d + 0x30;
    return 0;
}

/* The map's hand cursor, in screen units (Taku_Camp_2D draws it from these). */
#define HAND_X (*(short *)0x1ed0000)
#define HAND_Y (*(short *)0x1ed0002)
#define GROUND 8.65f

static int follow = 1;

static int project(float x, float z, int *sx, int *sy)
{
    float p[4];

    p[0] = x;
    p[1] = GROUND;
    p[2] = z;
    p[3] = 1.0f;
    Taku_3D_2D_Set(sx, sy, p);
    return *sx >= 0 && *sy >= 0;
}

/* Moves the probe on the ground until it is drawn at (tx, ty): a few Newton steps, the slopes
 * measured by projecting points two units away. */
static void seek(int tx, int ty)
{
    int i, ax, ay, bx, by, cx, cy;
    float a, b, c, d, det, dx, dz;

    for (i = 0; i < 8; i++) {
        if (!project(probe[0], probe[2], &ax, &ay) || !project(probe[0] + 2.0f, probe[2], &bx, &by)
            || !project(probe[0], probe[2] + 2.0f, &cx, &cy)) {
            return;
        }
        if (ax == tx && ay == ty) {
            return;
        }
        a = (bx - ax) / 2.0f;
        b = (cx - ax) / 2.0f;
        c = (by - ay) / 2.0f;
        d = (cy - ay) / 2.0f;
        det = a * d - b * c;
        if (det > -0.01f && det < 0.01f) {
            return;
        }
        dx = (d * (tx - ax) - b * (ty - ay)) / det;
        dz = (a * (ty - ay) - c * (tx - ax)) / det;
        if (dx > 20.0f) dx = 20.0f;
        if (dx < -20.0f) dx = -20.0f;
        if (dz > 20.0f) dz = 20.0f;
        if (dz < -20.0f) dz = -20.0f;
        probe[0] += dx;
        probe[2] += dz;
    }
}

int DlcMapDebug_Draw(int cursor)
{
#if DLC_DEBUG_MAP
    float *loc;
    int sx, sy, speed, i, near, mx, mz;
    float best, dx, dz, dist;

    if (SzGame_GetMode(0) != CAMPAIGN_MODE) {
        return cursor;
    }
    if (SzPad_Check(0, 0, PAD_L3)) {
        shown = !shown;
    }
    if (!shown) {
        return cursor;
    }
    if (!placed) {
        loc = &LOCATIONS[13 * (SELECTED < 22 ? SELECTED : 0)];
        probe[0] = loc[0];
        probe[2] = loc[2];
        placed = 1;
    }
    probe[1] = GROUND;
    probe[3] = 1.0f;
    if (SzPad_Check(0, 0, PAD_R3)) {
        follow = 1;
    }
    mx = stick(0);
    mz = stick(1);
    if (SzPad_IsAnalog(0) && (mx != 0 || mz != 0)) {
        /* Right stick: axis 0 across, 1 up and down (0 at the top); it takes over from the hand. */
        follow = 0;
        speed = SzPad_Check(0, 1, PAD_R1) ? 10 : 1;
        probe[0] += mx * speed / 64.0f;
        probe[2] -= mz * speed / 64.0f;
    }
    if (follow) {
        seek(HAND_X, HAND_Y);
    }
    /* The location nearest the probe. */
    near = 0;
    best = 1e30f;
    for (i = 0; i < 22; i++) {
        dx = LOCATIONS[13 * i] - probe[0];
        dz = LOCATIONS[13 * i + 2] - probe[2];
        dist = dx * dx + dz * dz;
        if (dist < best) {
            best = dist;
            near = i;
        }
    }
    loc = &LOCATIONS[13 * near];
    cursor = line(cursor, 40, 64, follow ? "CURSOR X " : "PROBE X ", (int)probe[0], "  Z ", (int)probe[2], 0, 0);
    cursor = line(cursor, 40, 80, "NEAREST ", near, "  X ", (int)loc[0], "  Z ", (int)loc[2]);
    cursor = DlcBosses_Debug(cursor, 40, 96);
    if (!follow && project(probe[0], probe[2], &sx, &sy)) {
        signed char mark = '?';
        cursor = text(cursor, sx - 4, sy - 8, &mark, 1);
    }
#endif
    return cursor;
}
