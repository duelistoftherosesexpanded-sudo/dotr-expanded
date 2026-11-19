/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * Loads the matrix at the argument into the current matrix vf28-vf31, or sets that to the
 * identity for a null argument. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_SetMatrix
.type RhGeom_SetMatrix, @function
RhGeom_SetMatrix:
    beq $a0, $zero, .Lreset
    nop
    lqc2 $vf28, 0($a0)
    lqc2 $vf29, 16($a0)
    lqc2 $vf30, 32($a0)
    beq $zero, $zero, .Ldone
    lqc2 $vf31, 48($a0)
.Lreset:
    vsub.xyzw $vf31, $vf0, $vf0
    vadd.w $vf31, $vf31, $vf0
    vmr32.xyzw $vf30, $vf31
    vmr32.xyzw $vf29, $vf30
    vmr32.xyzw $vf28, $vf29
.Ldone:
    jr $ra
    nop
.size RhGeom_SetMatrix, . - RhGeom_SetMatrix
