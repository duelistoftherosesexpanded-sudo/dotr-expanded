/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_GetMatrix
.type RhGeom_GetMatrix, @function
RhGeom_GetMatrix:
    sqc2 $vf28, 0($a0)
    sqc2 $vf29, 16($a0)
    sqc2 $vf30, 32($a0)
    jr $ra
    sqc2 $vf31, 48($a0)
.size RhGeom_GetMatrix, . - RhGeom_GetMatrix
