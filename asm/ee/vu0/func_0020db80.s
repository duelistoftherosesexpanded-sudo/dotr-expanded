/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeomEx_PopMatrix
.type RhGeomEx_PopMatrix, @function
RhGeomEx_PopMatrix:
    lui $at, %hi(data_0032a220)
    lw $a0, %lo(data_0032a220)($at)
    lui $v1, %hi(gRhGeom_rgl)
    addiu $v1, $v1, %lo(gRhGeom_rgl)
    addiu $a0, $a0, -1
    lui $at, %hi(data_0032a220)
    sw $a0, %lo(data_0032a220)($at)
    lui $at, %hi(data_0032a220)
    lw $a1, %lo(data_0032a220)($at)
    sll $a0, $a1, 3
    subu $a0, $a0, $a1
    sll $a0, $a0, 4
    addu $v1, $v1, $a0
    lqc2 $vf28, 0($v1)
    lqc2 $vf29, 16($v1)
    lqc2 $vf30, 32($v1)
    lqc2 $vf31, 48($v1)
    lqc2 $vf24, 64($v1)
    lqc2 $vf25, 80($v1)
    lqc2 $vf26, 96($v1)
    jr $ra
    lqc2 $vf27, 112($v1)
.size RhGeomEx_PopMatrix, . - RhGeomEx_PopMatrix
