/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * Null first argument: the current matrix vf28-vf31 times the matrix at the second argument,
 * kept in vf28-vf31; otherwise the first argument's matrix times the second, stored over the first. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_MulMatrix
.type RhGeom_MulMatrix, @function
RhGeom_MulMatrix:
    bnez $a0, .L_0020BC6C
    nop
    lqc2 $vf8, 0x0($a1)
    lqc2 $vf9, 0x10($a1)
    lqc2 $vf10, 0x20($a1)
    lqc2 $vf11, 0x30($a1)
    vmulax.xyzw $ACC, $vf28, $vf8x
    vmadday.xyzw $ACC, $vf29, $vf8y
    vmaddaz.xyzw $ACC, $vf30, $vf8z
    vmaddw.xyzw $vf20, $vf31, $vf8w
    vmulax.xyzw $ACC, $vf28, $vf9x
    vmadday.xyzw $ACC, $vf29, $vf9y
    vmaddaz.xyzw $ACC, $vf30, $vf9z
    vmaddw.xyzw $vf21, $vf31, $vf9w
    vmulax.xyzw $ACC, $vf28, $vf10x
    vmadday.xyzw $ACC, $vf29, $vf10y
    vmaddaz.xyzw $ACC, $vf30, $vf10z
    vmaddw.xyzw $vf22, $vf31, $vf10w
    vmulax.xyzw $ACC, $vf28, $vf11x
    vmadday.xyzw $ACC, $vf29, $vf11y
    vmaddaz.xyzw $ACC, $vf30, $vf11z
    vmaddw.xyzw $vf23, $vf31, $vf11w
    vmove.xyzw $vf28, $vf20
    vmove.xyzw $vf29, $vf21
    vmove.xyzw $vf30, $vf22
    b .L_0020BCDC
    vmove.xyzw $vf31, $vf23
.L_0020BC6C:
    lqc2 $vf4, 0x0($a0)
    lqc2 $vf5, 0x10($a0)
    lqc2 $vf6, 0x20($a0)
    lqc2 $vf7, 0x30($a0)
    lqc2 $vf8, 0x0($a1)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x0($a0)
    lqc2 $vf8, 0x10($a1)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x10($a0)
    lqc2 $vf8, 0x20($a1)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x20($a0)
    lqc2 $vf8, 0x30($a1)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x30($a0)
.L_0020BCDC:
    jr $ra
    nop
.size RhGeom_MulMatrix, . - RhGeom_MulMatrix
