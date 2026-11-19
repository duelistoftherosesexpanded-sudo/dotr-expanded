/* VU0 routine written as whole-function assembly (docs/topics/vu0-assembly.md): it works on
 * matrix state kept in the VU0 macro-mode registers between calls, which the C compiler cannot
 * reasonably represent.
 * Null argument: sets the current matrix vf28-vf31 to the identity; otherwise stores the
 * identity at the argument. */
.set noreorder
.set noat
.text
.balign 16
.globl RhGeom_UnitMatrix
.type RhGeom_UnitMatrix, @function
RhGeom_UnitMatrix:
    bne $a0, $zero, .Lstore
    nop
    vsub.xyzw $vf31, $vf0, $vf0
    vadd.w $vf31, $vf31, $vf0
    vmr32.xyzw $vf30, $vf31
    vmr32.xyzw $vf29, $vf30
    beq $zero, $zero, .Ldone
    vmr32.xyzw $vf28, $vf29
.Lstore:
    vsub.xyzw $vf4, $vf0, $vf0
    vadd.w $vf4, $vf4, $vf0
    vmr32.xyzw $vf5, $vf4
    vmr32.xyzw $vf6, $vf5
    vmr32.xyzw $vf7, $vf6
    sqc2 $vf4, 48($a0)
    sqc2 $vf5, 32($a0)
    sqc2 $vf6, 16($a0)
    sqc2 $vf7, 0($a0)
.Ldone:
    jr $ra
    nop
.size RhGeom_UnitMatrix, . - RhGeom_UnitMatrix
