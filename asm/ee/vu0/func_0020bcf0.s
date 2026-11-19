/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * As RhGeom_MulMatrix with the operands in the other order. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_MulMatrixRev
.type RhGeom_MulMatrixRev, @function
RhGeom_MulMatrixRev:
    bnez $a0, .L_0020BD5C
    nop
    lqc2 $vf8, 0x0($a1)
    lqc2 $vf9, 0x10($a1)
    lqc2 $vf10, 0x20($a1)
    lqc2 $vf11, 0x30($a1)
    vmulax.xyzw $ACC, $vf8, $vf28x
    vmadday.xyzw $ACC, $vf9, $vf28y
    vmaddaz.xyzw $ACC, $vf10, $vf28z
    vmaddw.xyzw $vf20, $vf11, $vf28w
    vmulax.xyzw $ACC, $vf8, $vf29x
    vmadday.xyzw $ACC, $vf9, $vf29y
    vmaddaz.xyzw $ACC, $vf10, $vf29z
    vmaddw.xyzw $vf21, $vf11, $vf29w
    vmulax.xyzw $ACC, $vf8, $vf30x
    vmadday.xyzw $ACC, $vf9, $vf30y
    vmaddaz.xyzw $ACC, $vf10, $vf30z
    vmaddw.xyzw $vf22, $vf11, $vf30w
    vmulax.xyzw $ACC, $vf8, $vf31x
    vmadday.xyzw $ACC, $vf9, $vf31y
    vmaddaz.xyzw $ACC, $vf10, $vf31z
    vmaddw.xyzw $vf23, $vf11, $vf31w
    vmove.xyzw $vf28, $vf20
    vmove.xyzw $vf29, $vf21
    vmove.xyzw $vf30, $vf22
    b .L_0020BDCC
    vmove.xyzw $vf31, $vf23
.L_0020BD5C:
    lqc2 $vf4, 0x0($a1)
    lqc2 $vf5, 0x10($a1)
    lqc2 $vf6, 0x20($a1)
    lqc2 $vf7, 0x30($a1)
    lqc2 $vf8, 0x0($a0)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x0($a0)
    lqc2 $vf8, 0x10($a0)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x10($a0)
    lqc2 $vf8, 0x20($a0)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x20($a0)
    lqc2 $vf8, 0x30($a0)
    vmulax.xyzw $ACC, $vf4, $vf8x
    vmadday.xyzw $ACC, $vf5, $vf8y
    vmaddaz.xyzw $ACC, $vf6, $vf8z
    vmaddw.xyzw $vf9, $vf7, $vf8w
    sqc2 $vf9, 0x30($a0)
.L_0020BDCC:
    jr $ra
    nop
.size RhGeom_MulMatrixRev, . - RhGeom_MulMatrixRev
