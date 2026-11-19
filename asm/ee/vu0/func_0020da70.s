/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * Normal matrix vf24-vf26 from the current rows scaled by vrsqrt of their squared lengths
 * (constant at 0x295060). */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeomEx_SetMatrixForPointLight
.type RhGeomEx_SetMatrixForPointLight, @function
RhGeomEx_SetMatrixForPointLight:
    lui $v1, %hi(vecNormal)
    addiu $v1, $v1, %lo(vecNormal)
    vmul.xyz $vf1, $vf28, $vf28
    vmul.xyz $vf2, $vf29, $vf29
    vmul.xyz $vf3, $vf30, $vf30
    vmove.x $vf5, $vf1
    vmove.x $vf6, $vf2
    vmove.x $vf7, $vf3
    vmr32.xyzw $vf1, $vf1
    vmr32.xyzw $vf2, $vf2
    vmr32.xyzw $vf3, $vf3
    vadd.x $vf5, $vf5, $vf1
    vadd.x $vf6, $vf6, $vf2
    vadd.x $vf7, $vf7, $vf3
    vmr32.xyzw $vf1, $vf1
    vmr32.xyzw $vf2, $vf2
    vmr32.xyzw $vf3, $vf3
    vadd.x $vf5, $vf5, $vf1
    vadd.x $vf6, $vf6, $vf2
    vadd.x $vf7, $vf7, $vf3
    lqc2 $vf8, 0x0($v1)
    vrsqrt $Q, $vf8x, $vf5x
    vsub.w $vf24, $vf0, $vf0
    vwaitq
    vmulq.xyz $vf24, $vf28, $Q
    vrsqrt $Q, $vf8x, $vf6x
    vsub.w $vf25, $vf0, $vf0
    vwaitq
    vmulq.xyz $vf25, $vf29, $Q
    vrsqrt $Q, $vf8x, $vf7x
    vsub.w $vf26, $vf0, $vf0
    vwaitq
    jr $ra
    vmulq.xyz $vf26, $vf30, $Q
.size RhGeomEx_SetMatrixForPointLight, . - RhGeomEx_SetMatrixForPointLight
