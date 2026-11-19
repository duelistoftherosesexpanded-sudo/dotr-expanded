/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_PushMatrix
.type RhGeom_PushMatrix, @function
RhGeom_PushMatrix:
    lui $at, %hi(data_0032a220)
    lw $a2, %lo(data_0032a220)($at)
    lui $v1, %hi(gRhGeom_rgl)
    addiu $v1, $v1, %lo(gRhGeom_rgl)
    sll $a1, $a2, 3
    subu $a1, $a1, $a2
    sll $a1, $a1, 4
    addu $v1, $v1, $a1
    sqc2 $vf28, 0($v1)
    sqc2 $vf29, 16($v1)
    sqc2 $vf30, 32($v1)
    beq $a0, $zero, .Ladvance
    sqc2 $vf31, 48($v1)
    lqc2 $vf28, 0($a0)
    lqc2 $vf29, 16($a0)
    lqc2 $vf30, 32($a0)
    lqc2 $vf31, 48($a0)
    nop
.Ladvance:
    lui $at, %hi(data_0032a220)
    lw $v1, %lo(data_0032a220)($at)
    addiu $v1, $v1, 1
    lui $at, %hi(data_0032a220)
    jr $ra
    sw $v1, %lo(data_0032a220)($at)
.size RhGeom_PushMatrix, . - RhGeom_PushMatrix
