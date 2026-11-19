/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * VU0 path for a null matrix; the other path is R5900 FPU multiply-accumulate code
 * (mula.s/madd.s/adda.s). */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_MulMatrixVector
.type RhGeom_MulMatrixVector, @function
RhGeom_MulMatrixVector:
    bnez $a1, .L_0020CDF4
    addiu $sp, $sp, -0x10
    lqc2 $vf8, 0x0($a2)
    vmulax.xyzw $ACC, $vf28, $vf8x
    vmadday.xyzw $ACC, $vf29, $vf8y
    vmaddaz.xyzw $ACC, $vf30, $vf8z
    vmaddw.xyzw $vf9, $vf31, $vf8w
    b .L_0020CEF8
    sqc2 $vf9, 0x0($a0)
.L_0020CDF4:
    lwc1 $f6, 0x4($a2)
    lwc1 $f0, 0x10($a1)
    lwc1 $f3, 0x8($a2)
    lwc1 $f2, 0x20($a1)
    lwc1 $f5, 0x0($a2)
    lwc1 $f4, 0x0($a1)
    lwc1 $f1, 0xC($a2)
    mula.s $f6, $f0
    lwc1 $f0, 0x30($a1)
    mul.s $f2, $f3, $f2
    addiu $v1, $sp, 0x0
    madd.s $f4, $f5, $f4
    adda.s $f2, $f4
    madd.s $f0, $f1, $f0
    swc1 $f0, 0x0($sp)
    lwc1 $f6, 0x0($a2)
    lwc1 $f0, 0x4($a1)
    lwc1 $f3, 0x8($a2)
    lwc1 $f2, 0x24($a1)
    lwc1 $f5, 0x4($a2)
    lwc1 $f4, 0x14($a1)
    lwc1 $f1, 0xC($a2)
    mula.s $f6, $f0
    lwc1 $f0, 0x34($a1)
    mul.s $f2, $f3, $f2
    madd.s $f4, $f5, $f4
    adda.s $f2, $f4
    madd.s $f0, $f1, $f0
    swc1 $f0, 0x4($sp)
    lwc1 $f6, 0x0($a2)
    lwc1 $f0, 0x8($a1)
    lwc1 $f3, 0x8($a2)
    lwc1 $f2, 0x28($a1)
    lwc1 $f5, 0x4($a2)
    lwc1 $f4, 0x18($a1)
    lwc1 $f1, 0xC($a2)
    mula.s $f6, $f0
    lwc1 $f0, 0x38($a1)
    mul.s $f2, $f3, $f2
    madd.s $f4, $f5, $f4
    adda.s $f2, $f4
    madd.s $f0, $f1, $f0
    swc1 $f0, 0x8($sp)
    lwc1 $f6, 0x0($a2)
    lwc1 $f0, 0xC($a1)
    lwc1 $f3, 0x8($a2)
    lwc1 $f2, 0x2C($a1)
    lwc1 $f5, 0x4($a2)
    lwc1 $f4, 0x1C($a1)
    lwc1 $f1, 0xC($a2)
    mula.s $f6, $f0
    lwc1 $f0, 0x3C($a1)
    mul.s $f2, $f3, $f2
    madd.s $f4, $f5, $f4
    adda.s $f2, $f4
    madd.s $f0, $f1, $f0
    swc1 $f0, 0xC($sp)
    lwc1 $f3, 0x0($v1)
    lwc1 $f2, 0x4($v1)
    lwc1 $f1, 0x8($v1)
    lwc1 $f0, 0xC($v1)
    swc1 $f3, 0x0($a0)
    swc1 $f2, 0x4($a0)
    swc1 $f1, 0x8($a0)
    swc1 $f0, 0xC($a0)
.L_0020CEF8:
    jr $ra
    addiu $sp, $sp, 0x10
.size RhGeom_MulMatrixVector, . - RhGeom_MulMatrixVector
