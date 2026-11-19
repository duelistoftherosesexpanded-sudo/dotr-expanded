#include "dotr/ee/slot_table.h"

/* 0x1fd680. The data are declared inside the body; three are views with their own types
 * (StringOffsetTable as an int array, the glyph table gSzMsgImage as bytes, the texture
 * context at 0x31c070 as its Context, which is gSzMsg_ml's first words). */
#ifndef __MWERKS__
/* Host build only: this C tests the output pointer's alignment through an int. */
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
/* `texture->used = 1` stores into a signed one-bit field (reads back as -1). */
#pragma GCC diagnostic ignored "-Woverflow"
#endif

DotrDrawWord SzMsg_LoadFont_Pipe(DotrDrawWord arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4)
{
    extern int StringOffsetTable_slotfont[];
    extern unsigned short StringBlob[];
    extern unsigned char gSzMsgImage_slotfont[];
    extern struct DotrSlotFontContext gSzMsg_ml_slotfont;
    struct DotrSlotFontContext *context = &gSzMsg_ml_slotfont;
    unsigned char *glyphs[17][53];
    unsigned char runs[17][53];
    unsigned short indices[17][53];
    unsigned short counts[17];
    unsigned short advances[17];
    unsigned short lineCount;
    long long *packet = (long long *)arg0;
    struct DotrSlotFontTexture *texture;
    unsigned short *stream;
    unsigned short width;
    unsigned short height;
    unsigned short *count;
    unsigned short *advance;
    int n;
    int clearIndex;
    unsigned short *clearCount;
    unsigned short *clearAdvance;
    unsigned char *data;
    unsigned char *out;
    unsigned short maxCount;
    int pixelHeight;
    int maxWidth;
    int glyphIndex;
    unsigned short *nextId;
    unsigned char **next;
    int num;
    int offset;
    unsigned short *id;
    int outputRow;
    unsigned char **nextGlyph;
    int rowSize;
    unsigned short *nextCount;
    int bytes;
    unsigned char **lineGlyphs;
    int lineIndex;
    unsigned char *run;
    unsigned char **glyph;
    int outputGlyph;
    unsigned short currentAdvance;
    int pixelWidth;
    int rows;
    unsigned char *lineRuns;
    long long dimensions;
    long long coordinates;
    int length;

    texture = (struct DotrSlotFontTexture *)SzMsg_GetInfo(msgInfoIndex);
    stream = &StringBlob[StringOffsetTable_slotfont[stringIndex]];
    width = 16;
    height = 16;
    lineCount = 0;
    if (texture == 0) {
        return arg0;
    }
    if (texture->used) {
        return arg0;
    }
    count = counts;
    advance = advances;
    clearIndex = 0;
    clearCount = count;
    clearAdvance = advance;
    do {
        clearIndex++;
        *clearCount++ = 0;
        *clearAdvance++ = 0;
    } while (clearIndex < 17);

    do {
        n = lSzMsg_FontTrace(stream, (char (*)[53])runs, (short (*)[53])indices, counts, advances, &lineCount);
        if (stream[n - 1] & 0x8000) {
            break;
        }
        stream += n;
    } while (1);

    lineCount++;
    lineGlyphs = glyphs[0];
    id = indices[0];
    maxWidth = 0;
    maxCount = maxWidth;
    lineIndex = 0;
    nextGlyph = lineGlyphs;
    nextCount = count;
    while (lineIndex < lineCount) {
        num = *nextCount;
        glyphIndex = 0;
        nextId = id;
        next = nextGlyph;
        goto glyph_test;
    glyph_loop:
        *next = gSzMsgImage_slotfont + *nextId * 242;
        glyphIndex++;
        nextId++;
        next++;
    glyph_test:
        if (glyphIndex < num) {
            goto glyph_loop;
        }
        if ((unsigned short)maxWidth < (currentAdvance = *advance)) {
            maxCount = *nextCount;
            maxWidth = (unsigned short)currentAdvance;
        }
        id += 53;
        nextGlyph += 53;
        nextCount++;
        advance++;
        lineIndex++;
    }

    pixelHeight = (unsigned short)(lineCount * 22);
    pixelWidth = (unsigned short)maxWidth;
    while (pixelWidth > (unsigned short)width) {
        width = (unsigned short)(width * 2);
    }
    rows = (unsigned short)pixelHeight;
    while (rows > (unsigned short)height) {
        height = (unsigned short)(height * 2);
    }

    dimensions = (long long)(unsigned short)maxWidth | ((long long)(unsigned short)pixelHeight << 32);
    texture->used = 1;
    texture->count = maxCount;
    texture->width = width;
    texture->height = height;
    texture->x = arg3;
    texture->y = arg4;
    texture->pixelWidth = pixelWidth;
    texture->pixelHeight = rows;

    packet[1] = 0;
    packet[0] = 0;
    ((unsigned char *)packet)[3] = 16;
    packet[2] = 4 | ((long long)0x10000000 << 32);
    packet[3] = 14;
    coordinates = ((long long)arg3 << 32) | ((long long)arg4 << 48);
    packet[4] = ((long long)context->tbp << 32) |
                      ((long long)(context->width >> 6) << 48) |
                      ((long long)0x14000000 << 32);
    packet[5] = 0x50;
    packet[6] = coordinates;
    packet[7] = 0x51;
    packet[8] = dimensions;
    packet[9] = 0x52;
    packet[10] = 0;
    packet[11] = 0x53;
    n = pixelWidth >> 1;
    packet[12] = (long long)((n * rows + 15) >> 4) |
                       (0x8000 | ((long long)0x08000000 << 32));
    out = (unsigned char *)(packet + 14);
    packet[13] = 0;
    lineRuns = runs[0];
    lineIndex = 0;
    while (lineIndex < lineCount) {
        outputRow = 0;
        do {
            rowSize = 0;
            outputGlyph = 0;
            run = lineRuns;
            glyph = lineGlyphs;
            while (outputGlyph < *count) {
                bytes = *run;
                offset = 0;
                if (0 < bytes) {
                    do {
                        *out++ = (*glyph)[offset++];
                    } while (offset < (int)bytes);
                }
                rowSize += bytes;
                run++;
                outputGlyph++;
                *glyph += 11;
                glyph++;
            }
            goto padding_test;
        padding_loop:
            *out++ = 0;
            rowSize++;
        padding_test:
            if (rowSize < n) {
                goto padding_loop;
            }
            outputRow++;
        } while (outputRow < 22);
        lineRuns += 53;
        lineGlyphs += 53;
        count++;
        lineIndex++;
    }
    while ((int)out % 16 != 0) {
        out++;
    }
    length = (((long long *)out - packet) >> 1) - 1;
    *(unsigned short *)packet = length;
    ((int *)packet)[2] = 0x11000000;
    ((int *)packet)[3] = (unsigned short)length | 0x51000000;
    return (DotrDrawWord)out;
}
