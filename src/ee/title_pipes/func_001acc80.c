#include "dotr/ee/title_pipes.h"

/* lSzGame_DrawTitle_Pipe (0x1acc80): the title screen. Loads the two title images (group 9)
 * after a CLUT area at 0x37c0, then draws the logo and its parts as textured sprites, the
 * menu rows cut by the eight bytes at 0x28ff28 with the selected row lit, flat rectangles
 * for the shading, and the rose (lSzGame_DrawTitle_Rose_Pipe), following the title state at
 * 0x317250 (intensity, selection, alpha, shade, hidden flag). Returns the advanced cursor.
 * Compiled under #pragma opt_strength_reduction off and #pragma opt_propagation off (both reset
 * after). The callees are declared at block scope; the two image handles are cast to pointers
 * through the cursor word type (int on the target). */
#ifdef __MWERKS__
#pragma opt_strength_reduction off
#pragma opt_propagation off
#endif
int lSzGame_DrawTitle_Pipe(int arg0)
{
    int y;
    unsigned short x;
    extern int SzText_InitClutManage();
    extern int SzText_LoadRequest();
    extern int ExSortPrim_Pipe();
    int full;
    DotrTitleState *state;
    int rowOffset;
    int rowHeight;
    unsigned short left;
    unsigned short right;
    int uvTop;
    int uvBottom;
    unsigned short top;
    unsigned char *rowEdges;
    int row;
    int selectedY;
    int images[2];
    DotrTitleCuts cuts;
    DotrTitleRect rect __attribute__((aligned(8)));
    DotrTitleTexture texture __attribute__((aligned(8)));
    int length;
    int resourceOffset;
    int source;
    int result;
    int i;
    int *image;

    state = (DotrTitleState *)&gSzGame_gl_title;
    resourceOffset = 0x37C0;
    cuts = *(DotrTitleCuts *)data_0028ff28_title;
    resourceOffset += SzText_InitClutManage(resourceOffset, 2);
    image = images;
    i = 0;
    while (1) {
        *image = SzText_LoadRequest(9, i);
        image++;
        i++;
        if (i > 1) break;
    }
    source = SzText_DoRequest_Pipe(arg0, 9, resourceOffset, &length);
    SzText_FreeRequest(9, -1);
    result = RhText_SetTextureFlush(source);
    ((DotrTitleTag *)&texture.tag)->flags = 0x26;
    ((DotrTitleTag *)&texture.tag)->count = 5;
    ((DotrTitleTag *)&texture.tag)->registers = 0x42421;
    ((DotrTitleTag *)&texture.tag)->reserved = 0;
    ((DotrTitleTag *)&texture.tag)->flags |= 0x800;
    texture.color.r = 0x80;
    texture.color.g = 0x80;
    texture.color.b = 0x80;
    texture.color.a = 0x80;
    texture.color.q = 1.0f;
    texture.vertex[0].xyz.z = 0xFFFFF8;
    texture.vertex[0].xyz.f = 0;
    texture.vertex[1].xyz.z = 0xFFFFF8;
    texture.vertex[1].xyz.f = 0;
    ExAssignTexture(&texture, (void *)(DotrTitleWord)images[0], 0);
    ((DotrTitleUV *)&texture.vertex[0].uv)->u = 8;
    ((DotrTitleUV *)&texture.vertex[0].uv)->v = 8;
    ((DotrTitleUV *)&texture.vertex[1].uv)->u = 0x2808;
    ((DotrTitleUV *)&texture.vertex[1].uv)->v = 0x1C08;
    texture.vertex[0].xyz.x = 0x6C00;
    texture.vertex[0].xyz.y = 0x7200;
    texture.vertex[1].xyz.x = 0x9400;
    texture.vertex[1].xyz.y = 0x8E00;
    result = ExSortPrim_Pipe(result, &texture, 0, 0);
    ExAssignAlpha(&texture, ExGetAlphaVal(0));
    ExAssignTexture(&texture, (void *)(DotrTitleWord)images[1], 0);
    if (state->alpha < 0x80) {
        int x = 0x800;
        int y = 0x884;
        texture.color.r = texture.color.g = state->shade;
        texture.color.a = 0x80 - state->alpha;
        texture.color.a = (texture.color.a * state->intensity) >> 7;
        ((DotrTitleUV *)&texture.vertex[0].uv)->u = 8;
        ((DotrTitleUV *)&texture.vertex[0].uv)->v = 0xC08;
        ((DotrTitleUV *)&texture.vertex[1].uv)->u = 0xB88;
        ((DotrTitleUV *)&texture.vertex[1].uv)->v = 0xD08;
        texture.vertex[0].xyz.x = (x - 0x5C) << 4;
        texture.vertex[0].xyz.y = y << 4;
        texture.vertex[1].xyz.x = (0x5C + x) << 4;
        texture.vertex[1].xyz.y = (y + 0x10) << 4;
        result = ExSortPrim_Pipe(result, &texture, 0, 0);
        if (state->flags.hidden != 0) {
            return result;
        }
    }
    ((DotrTitleTag *)&rect.tag)->flags = 6;
    ((DotrTitleTag *)&rect.tag)->count = 3;
    ((DotrTitleTag *)&rect.tag)->registers = 0x441;
    ((DotrTitleTag *)&rect.tag)->reserved = 0;
    ExAssignAlpha(&rect, ExGetAlphaVal(2));
    rect.color.r = 0x80;
    rect.color.g = 0x80;
    rect.color.b = 0x80;
    rect.color.a = (state->alpha * 0x60) >> 7;
    rect.color.q = 1.0f;
    rect.xyz[0].x = 0x6C00;
    rect.xyz[0].y = 0x7200;
    rect.xyz[0].z = 0xFFFFF8;
    rect.xyz[0].f = 0;
    rect.xyz[1].x = 0x9400;
    rect.xyz[1].y = 0x8E00;
    rect.xyz[1].z = 0xFFFFF8;
    rect.xyz[1].f = 0;
    result = ExSortPrim_Pipe(result, &rect, 0, 0);
    full = 0x80;
    {
        int y = 0x740;
        unsigned short offset;
        int uvY = 0;
        unsigned short y0;
        unsigned short y1;
        int index = 0;
        int x = 0x800;
        texture.color.a = state->alpha;
        offset = index * 0x70;
        y0 = y + offset;
        y1 = offset + 0x30;
        left = (x - 0x80) << 4;
        right = (x + 0x80) << 4;
        do {
            if (state->selected == index) {
                texture.color.b = 0x80;
                texture.color.g = 0x80;
                texture.color.r = 0x80;
            } else {
                texture.color.b = 0x40;
                texture.color.g = 0x40;
                texture.color.r = 0x40;
            }
            ((DotrTitleUV *)&texture.vertex[0].uv)->u = 8;
            ((DotrTitleUV *)&texture.vertex[0].uv)->v = (uvY << 4) + 8;
            ((DotrTitleUV *)&texture.vertex[1].uv)->u = 0x1008;
            ((DotrTitleUV *)&texture.vertex[1].uv)->v = ((index + 1) * 0x300) + 8;
            texture.vertex[0].xyz.x = left;
            texture.vertex[0].xyz.y = y0 << 4;
            texture.vertex[1].xyz.x = right;
            texture.vertex[1].xyz.y = (y + y1) << 4;
            result = ExSortPrim_Pipe(result, &texture, 0, 0);
            y0 += 0x70;
            y1 += 0x70;
            index++;
            uvY += 0x30;
        } while (index < 4);
    }
    texture.color.b = full;
    texture.color.g = 0x80;
    texture.color.r = 0x80;
    {
        int y = state->selected * 0x70 - 0xA8;
        result = lSzGame_DrawTitle_Rose_Pipe(result, 0x22, -0x9B, y, 0x20, 0x20, 50.0f);
        result = lSzGame_DrawTitle_Rose_Pipe(result, 0x23, 0x9B, y, 0x20, 0x20, 50.0f);
    }
    rowOffset = 0;
    row = 0;
    selectedY = state->selected * 0x70 + 0x738;
    rowEdges = cuts.edge;
    do {
        int columnOffset;
        int uvY;
        unsigned short y;
        int column;
        int uvHeight;
        int columnWidth;
        unsigned short bottom;
        unsigned char *columnEdges;
        int cx = 0x778;
        uvY = rowEdges[4];
        uvHeight = rowEdges[5] - uvY;
        rowHeight = (1 == row) ? 0x40 - cuts.edge[5] * 2 : uvHeight;
        uvTop = ((uvY + 0xD0) << 4) + 8;
        columnOffset = 0;
        uvBottom = ((uvY + 0xD0 + uvHeight) << 4) + 8;
        column = 0;
        y = selectedY + rowOffset;
        columnEdges = cuts.edge;
        top = y << 4;
        bottom = (rowHeight + y) << 4;
        do {
            int uvX = columnEdges[0];
            int uvWidth = columnEdges[1] - uvX;
            columnWidth = (1 == column) ? 0x110 - cuts.edge[1] * 2 : uvWidth;
            ((DotrTitleUV *)&texture.vertex[0].uv)->u = (uvX << 4) + 8;
            ((DotrTitleUV *)&texture.vertex[0].uv)->v = uvTop;
            ((DotrTitleUV *)&texture.vertex[1].uv)->u = ((uvX + uvWidth) << 4) + 8;
            ((DotrTitleUV *)&texture.vertex[1].uv)->v = uvBottom;
            x = cx + columnOffset;
            texture.vertex[0].xyz.x = x << 4;
            texture.vertex[0].xyz.y = top;
            texture.vertex[1].xyz.x = (columnWidth + x) << 4;
            texture.vertex[1].xyz.y = bottom;
            result = ExSortPrim_Pipe(result, &texture, 0, 0);
            column++;
            columnOffset += columnWidth;
            columnEdges++;
        } while (column < 3);
        rowOffset += rowHeight;
        rowEdges++;
        row++;
    } while (row < 3);
    {
        int x = 0x752;
        y = state->selected * 0x70 + 0x738;
        ((DotrTitleUV *)&texture.vertex[0].uv)->u = 0xDA8;
        ((DotrTitleUV *)&texture.vertex[0].uv)->v = 0xC08;
        ((DotrTitleUV *)&texture.vertex[1].uv)->u = 0x1008;
        ((DotrTitleUV *)&texture.vertex[1].uv)->v = 0x1008;
        texture.vertex[0].xyz.x = x << 4;
        texture.vertex[0].xyz.y = y << 4;
        texture.vertex[1].xyz.x = (0x26 + x) << 4;
        texture.vertex[1].xyz.y = (0x40 + y) << 4;
        result = ExSortPrim_Pipe(result, &texture, 0, 0);
    }
    {
        int x = 0x888;
        int y = state->selected * 0x70 + 0x738;
        ((DotrTitleUV *)&texture.vertex[0].uv)->u = 0xD98;
        ((DotrTitleUV *)&texture.vertex[0].uv)->v = 0xC08;
        ((DotrTitleUV *)&texture.vertex[1].uv)->u = 0xFF8;
        ((DotrTitleUV *)&texture.vertex[1].uv)->v = 0x1008;
        texture.vertex[0].xyz.x = (0x26 + x) << 4;
        texture.vertex[0].xyz.y = y << 4;
        texture.vertex[1].xyz.x = x << 4;
        texture.vertex[1].xyz.y = (y + 0x40) << 4;
        result = ExSortPrim_Pipe(result, &texture, 0, 0);
    }
    return result;
}
#ifdef __MWERKS__
#pragma opt_strength_reduction reset
#pragma opt_propagation reset
#endif
