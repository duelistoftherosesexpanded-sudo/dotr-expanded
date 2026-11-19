#ifndef DOTR_EE_FRAME_TEXTURE_H
#define DOTR_EE_FRAME_TEXTURE_H

/* Packet builders that describe the frame buffer as a texture through the buffer-format and
 * screen-size getters declared in draw_state.h.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/math-prims.md. */

#include "dotr/ee/draw_state.h"

/* 0x208870 ExFrameBuffer2Texture_Pipe (name from the Japanese build's symbol
 * table). Asks RhBase_GetBufferFormatEx(tbp != 0, &base, &format, 0) and
 * RhBase_GetScreenSize(&w, &h), and takes the smallest exponents with
 * (1 << e) >= w and >= h. For a non-null `c` (a DotrDrawBits64A): bits 0-13 =
 * tbp when positive, else base; bits 14-19 = w >> 6; bits 20-25 = format;
 * bits 26-29 / 30-33 = the two exponents; bits 34-60 = 0; bits 61-63 = 1.
 * For a non-null `d` (a DotrDrawBits64B): bit 5 and bits 6-8 = filter, the
 * other written fields 0. With tbp > 0 it also writes twelve 64-bit words at
 * `cursor` (a tag word pair, then four value/register pairs 0x50-0x53 that
 * name base and tbp with the same width and format, and w | h << 32) and
 * returns the address after them; otherwise it returns `cursor`.
 * Assumed: the declaration draw_state.h already carries (`c` and `d` are the
 * addresses the callers pass as words); the 64-bit view of the packet. */

#endif
