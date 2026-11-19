#include "dotr/ee/ex_board.h"

/* lSzDuel_DrawWindow_Base_Pipe (0x1b62f0): tiles one sprite over a columns by rows grid starting
 * at (x + 0x6c0, y + 0x720): the tile is row 189 of the tile table, drawn in colour
 * 0x80,0x80,0x80,0x80 with alpha word 0x8000000044 and the control block's depth; each cell is
 * one ExSortPrim_Pipe call, columns stepping by the tile's width and rows by its height.
 * Returns the advanced cursor.
 * Its two data symbols are declared in the header. */
DotrDrawWord lSzDuel_DrawWindow_Base_Pipe(DotrDrawWord packet, int x, int y, int columns, int rows)
{
    DotrExBoardBaseTexture *texture;
    int width;
    int xOffset;
    int column;
    unsigned short y0;
    unsigned short y1;
    int originY;
    int originX;
    int height;
    int yOffset;
    int row;
    DotrExBoardBasePacket sprite __attribute__((aligned(8)));
    DotrExBoardBaseUV *uv0;
    DotrExBoardBaseUV *uv1;

    originY = y + 0x720;
    texture = (DotrExBoardBaseTexture *)data_002907e2_exboard;
    ((DotrExBoardBaseHeader *)sprite.header)->flags = 0x26;
    ((DotrExBoardBaseHeader *)sprite.header)->count = 5;
    ((DotrExBoardBaseHeader *)sprite.header)->registers = 0x42421;
    ((DotrExBoardBaseHeader *)sprite.header)->padding = 0;
    originX = x + 0x6C0;
    ((DotrExBoardBaseHeader *)sprite.header)->flags |= 0x800;
    func_002086d0_base(&sprite, 0x8000000044ULL);
    sprite.r = sprite.g = sprite.b = 0x80;
    sprite.a = 0x80;
    sprite.q = 1.0f;
    sprite.xyz0.z = data_003180e8_exboard;
    sprite.xyz0.padding = 0;
    sprite.xyz1.z = data_003180e8_exboard;
    sprite.xyz1.padding = 0;
    func_00208700_base(&sprite, data_00317b30_exboard[texture->texture], 0);
    uv0 = (DotrExBoardBaseUV *)&sprite.uv0;
    uv1 = (DotrExBoardBaseUV *)&sprite.uv1;
    uv0->u = texture->left * 16;
    yOffset = 0;
    row = 0;
    uv0->v = texture->top * 16;
    uv1->u = texture->right * 16;
    uv1->v = texture->bottom * 16;
    width = texture->right - texture->left;
    height = texture->bottom - texture->top;
    if (row < rows) {
        do {
            xOffset = 0;
            column = 0;
            if (column < columns) {
                unsigned short rowY = originY + yOffset;
                y0 = rowY * 16 - 8;
                y1 = (height + rowY) * 16 - 8;
                do {
                    unsigned short columnX = originX + xOffset;
                    sprite.xyz0.x = columnX * 16 - 8;
                    sprite.xyz0.y = y0;
                    sprite.xyz1.x = (width + columnX) * 16 - 8;
                    sprite.xyz1.y = y1;
                    packet = func_00207650_base(packet, (DotrDrawWord)&sprite, 0, 0);
                    column++;
                    xOffset += width;
                } while (column < columns);
            }
            yOffset += height;
            row++;
        } while (row < rows);
    }
    return packet;
}
