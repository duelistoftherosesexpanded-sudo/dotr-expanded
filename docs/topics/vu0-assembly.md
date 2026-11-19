# EE VU0 macro-register routines (assembly)

- Target: NTSC-U `SLUS_205.15`, executable SHA-256
  `1df7759b68e6aee73d8deab2efd2e556ec553d952af00fe8bf0f499a57298719`.
- Module and address space: **ee-main**, EE virtual addresses. These are EE instruction streams
  using VU0 macro-mode registers, not VU microprogram addresses.
- Classification: **assembly** (`match_kind: asm`), under the architecture-specific rule of
  [DECOMPILATION.md](../DECOMPILATION.md). Persistent COP2/VU register loads, stores and vector
  operations are observable effects the compiler cannot reasonably be made to produce from C.
  That the original source was handwritten is not proven. Ordinary neighbouring routines are C.
- Sources: all 13 are written in GNU assembler syntax from the original instructions.
  [CREDITS.md](../CREDITS.md) lists where five of them (`RhGeom_MulMatrix`, `RhGeom_MulMatrixRev`,
  `RhGeom_MulMatrixVector`, `RhGeom_MulMatrixVectorArray`, `RhGeomEx_SetMatrixForPointLight`)
  come from.

## Byte comparisons

Mnemonic sources are in `asm/ee/vu0/`, with `.set noreorder`/`.set noat` and
explicit function sizes. There are no literal instruction-word directives.
They are assembled with the pinned GNU binutils 2.38 (`-EL -march=r5900 -mabi=eabi`), and
`tools/match.py` compares every full range without masks.

| Function | EE entry | Bytes | Original/rebuilt SHA-256 |
| --- | --- | ---: | --- |
| `RhGeom_SetMatrix` | `0x20ba90` | 56 | `21b0a301292dbbcfe9ed89e40ae42c813cc141428743460045a4a3fb88b67688` |
| `RhGeom_GetMatrix` | `0x20bad0` | 20 | `7093314e2bbbe913005f5dc438898788d50e2ae4df4496b38627830fd6dac6b8` |
| `RhGeom_PushMatrix` | `0x20baf0` | 96 | `03cdd44d0bc0d6943d274b2d6a00778d24bd2af97aba065b5b1360089ec41510` |
| `RhGeom_PopMatrix` | `0x20bb50` | 72 | `266f050d146017130c8ac0f9a62d130c2ff8052b7890c49d84131e950701786f` |
| `RhGeom_UnitMatrix` | `0x20bbb0` | 76 | `8efb27a7aaa26e921789a2a5e15d7bebfafa76fb7c6ba3ebfe55334c6e5fe431` |
| `RhGeomEx_GetMatrix` | `0x20db00` | 32 | `1002c953dac1ff0619e8f7c3f22bc510fed59766bbf5363f1c1b29eae682bdcb` |
| `RhGeomEx_PushMatrix` | `0x20db20` | 88 | `e6d39bf29bb4c5999a986ed8f968d70860ee4393e246ddabe187a9fdbfdcb577` |
| `RhGeomEx_PopMatrix` | `0x20db80` | 88 | `a1bec06b4e021af07885d6a0c91199db1c80142503f37398e2f10f23f321a3ab` |
| `RhGeom_MulMatrix` | `0x20bc00` | 228 | `e126a06588c02ef738bb3932f767666b53e3ddfdff175210a929d2b0e48dfefd` |
| `RhGeom_MulMatrixRev` | `0x20bcf0` | 228 | `00f8d8af23b2f1a37f0b728ae6763e378453336f06c7dff3c0f8e9248642438b` |
| `RhGeom_MulMatrixVector` | `0x20cdd0` | 304 | `24da13167ecb7362abb900e1bb040a2da82099847bb49d2c64dffe0c94333d31` |
| `RhGeom_MulMatrixVectorArray` | `0x20cf00` | 360 | `0143d6c5800b9822aed3432e92ad7fd58facfe55a517143c64e3cc7eab36b0e4` |
| `RhGeomEx_SetMatrixForPointLight` | `0x20da70` | 136 | `e3b11de52276fb89166c85b937cdb61e06ef66262f9940ca46b4ab564993a2e8` |

The total is **13 assembly functions, 1,784 bytes**, counted separately from C. Return delay
slots are included, for example `0x20bbf8` in the 76-byte `0x20bbb0` range and `0x20dbd4` in the
88-byte `0x20db80` range. Alignment gaps and following padding are excluded.

## What each routine does

- `RhGeom_SetMatrix` (`0x20ba90`): a non-null argument loads vf28..vf31 from its offsets 0..48; a null argument resets vf28..vf31 with a `vsub`/`vadd`/`vmr32` sequence.
- `RhGeom_GetMatrix` (`0x20bad0`): stores the persistent vf28..vf31 at argument offsets 0, 16, 32 and 48.
- `RhGeom_PushMatrix` (`0x20baf0`): saves vf28..vf31 at `0x326a20` + counter * 112; a non-null argument then loads vf28..vf31; reloads and increments the counter at `0x32a220`. No bounds check.
- `RhGeom_PopMatrix` (`0x20bb50`): decrements and reloads the counter at `0x32a220`; restores vf28..vf31 from `0x326a20` + counter * 112. No bounds check.
- `RhGeom_UnitMatrix` (`0x20bbb0`): a null argument resets vf28..vf31 with VU macro operations; a non-null one prepares vf4..vf7 with the same operations and stores them in reverse order at offsets 0..48. The persistent register effects are part of the behaviour.
- `RhGeomEx_GetMatrix` (`0x20db00`): stores vf28..vf31, then vf24..vf26, at argument offsets 0..96. It does not store vf27.
- `RhGeomEx_PushMatrix` (`0x20db20`): stores vf28..vf31 and vf24..vf27, through offset 112, at `0x326a20` + counter * 112, then reloads and increments the counter. The 128 bytes written exceed the 112-byte stride; the bounds of the storage are unknown.
- `RhGeomEx_PopMatrix` (`0x20db80`): decrements and reloads the counter, restores vf28..vf31 and vf24..vf27 from `0x326a20` + counter * 112 through offset 112. No bounds check; the bounds of the storage are unknown.
- `RhGeom_MulMatrix` (`0x20bc00`): with a null first argument, vf28..vf31 times the 4x4 matrix at the second argument, result back in vf28..vf31 (through vf20..vf23); otherwise the first argument's matrix times the second, stored over the first.
- `RhGeom_MulMatrixRev` (`0x20bcf0`): as `RhGeom_MulMatrix` with the operands in the other order.
- `RhGeom_MulMatrixVector` (`0x20cdd0`): with a null matrix, vf28..vf31 times the vector at the third argument, stored at the first. Otherwise a 16-byte stack temporary filled by R5900 FPU multiply-accumulate code (`mula.s`/`madd.s`/`adda.s`) and copied out, with four of the loads as `lwc1`.
- `RhGeom_MulMatrixVectorArray` (`0x20cf00`): as `RhGeom_MulMatrixVector` over a count of vectors (fourth argument); the VU0 loop for a null matrix, the FPU loop otherwise (counter cleared with `paddub`).
- `RhGeomEx_SetMatrixForPointLight` (`0x20da70`): the normal matrix vf24..vf26 from vf28..vf30 scaled by `vrsqrt` of each row's squared length, against the vector constant at `0x295060` (Japanese name `vecNormal$331`), with their w lanes cleared.

The counter at `0x32a220` and the storage at `0x326a20` are known by address only; their
declarations and bounds are unknown. The 112-byte stride and the 128-byte extent of the larger
save and restore routines are kept as they are, overlap included, and no bounds checks are
added. Host builds do not assemble these routines (callers' tests stub them), and they were not
run in an emulator: exact bytes say nothing about higher-level game behaviour or handwritten
provenance.

Reproduce with `make match`; reports are written to
`out/research/ntsc-u/matches/<function>/comparison.json`.
