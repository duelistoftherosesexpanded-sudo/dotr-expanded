/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * VU0 loop for a null matrix; the other path is R5900 FPU multiply-accumulate code
 * (mula.s/madd.s/adda.s) and uses paddub to clear a counter. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_MulMatrixVectorArray
.type RhGeom_MulMatrixVectorArray, @function
RhGeom_MulMatrixVectorArray:
    bnez $a1, .L_0020CF38
    addiu $sp, $sp, -0x10
.L_0020CF08:
    lqc2 $vf8, 0x0($a2)
    vmulax.xyzw $ACC, $vf28, $vf8x
    vmadday.xyzw $ACC, $vf29, $vf8y
    vmaddaz.xyzw $ACC, $vf30, $vf8z
    vmaddw.xyzw $vf9, $vf31, $vf8w
    sqc2 $vf9, 0x0($a0)
    addi $a3, $a3, -0x1
    addi $a0, $a0, 0x10
    bne $zero, $a3, .L_0020CF08
    addi $a2, $a2, 0x10
    b .L_0020D060
    addiu $sp, $sp, 0x10
.L_0020CF38:
    addiu $t0, $sp, 0x0
    b .L_0020D050
    paddub $t1, $zero, $zero
.L_0020CF44:
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
    addiu $t1, $t1, 0x1
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
    lwc1 $f1, 0xC($a2)
    lwc1 $f4, 0x1C($a1)
    mula.s $f6, $f0
    lwc1 $f0, 0x3C($a1)
    mul.s $f2, $f3, $f2
    addiu $a2, $a2, 0x10
    madd.s $f4, $f5, $f4
    adda.s $f2, $f4
    madd.s $f0, $f1, $f0
    swc1 $f0, 0xC($sp)
    lwc1 $f3, 0x0($t0)
    lwc1 $f2, 0x4($t0)
    lwc1 $f1, 0x8($t0)
    lwc1 $f0, 0xC($t0)
    swc1 $f3, 0x0($a0)
    swc1 $f2, 0x4($a0)
    swc1 $f1, 0x8($a0)
    swc1 $f0, 0xC($a0)
    addiu $a0, $a0, 0x10
.L_0020D050:
    slt $v1, $t1, $a3
    bnez $v1, .L_0020CF44
    nop
    addiu $sp, $sp, 0x10
.L_0020D060:
    jr $ra
    nop
.size RhGeom_MulMatrixVectorArray, . - RhGeom_MulMatrixVectorArray
