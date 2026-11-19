/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeomEx_PushMatrix
.type RhGeomEx_PushMatrix, @function
RhGeomEx_PushMatrix:
    lui $at, %hi(data_0032a220)
    lw $a1, %lo(data_0032a220)($at)
    lui $v1, %hi(gRhGeom_rgl)
    addiu $v1, $v1, %lo(gRhGeom_rgl)
    sll $a0, $a1, 3
    subu $a0, $a0, $a1
    sll $a0, $a0, 4
    addu $v1, $v1, $a0
    sqc2 $vf28, 0($v1)
    sqc2 $vf29, 16($v1)
    sqc2 $vf30, 32($v1)
    sqc2 $vf31, 48($v1)
    sqc2 $vf24, 64($v1)
    sqc2 $vf25, 80($v1)
    sqc2 $vf26, 96($v1)
    sqc2 $vf27, 112($v1)
    lui $at, %hi(data_0032a220)
    lw $v1, %lo(data_0032a220)($at)
    addiu $v1, $v1, 1
    lui $at, %hi(data_0032a220)
    jr $ra
    sw $v1, %lo(data_0032a220)($at)
.size RhGeomEx_PushMatrix, . - RhGeomEx_PushMatrix
