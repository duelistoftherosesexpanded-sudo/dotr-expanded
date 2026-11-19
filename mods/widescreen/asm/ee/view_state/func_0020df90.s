/* Widescreen (16:9): RhView_SetAspect(float a, float b) continues in WsView_SetAspect
 * (src/ee/pad_toggles/func_001d95f0.c), which scales one of the two factors before storing them.
 * The arguments are passed on untouched, and that function returns to this one's caller. */
.set noreorder
.set noat
.text
.balign 16
.globl RhView_SetAspect
.type RhView_SetAspect, @function
RhView_SetAspect:
    j     WsView_SetAspect
    nop
.size RhView_SetAspect, . - RhView_SetAspect
