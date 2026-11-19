# Roadmap

## Done

| Milestone | State |
| --- | --- |
| Disc and executable identity, inventory of sections, overlay archives and IOP modules | done |
| Compiler identified (Metrowerks 2.3.3) and flags evidenced | done |
| Every game function byte-matching: 2,568 functions, 1,405,804 bytes (13 of them VU0 assembly) | done |
| Original names from the Japanese build's symbol table | done where the Japanese build has the function |
| Whole executable assembled from rebuilt code, byte-identical (`make rebuild`, stage 1) | done |

Stage 1 places each rebuilt function at its original address and still takes the data,
headers and VU1 microcode from your executable. It proves the code, not a build you can change
freely.

## Next

1. **A linked build (stage 2).** One link in which the linker resolves every symbol by name:
   game code from C, the library block and startup code as assembly, data as blobs at first, then
   compared whole. This is what lets functions change size, and what modding needs.
2. **Source files as in the original.** Group functions into their original translation units
   (the Japanese debug data names some of them), which also removes the special handling of the
   original-file units and the `__<address>` suffixes on duplicate names.
3. **Typed data.** Card, deck, duelist and campaign tables declared as C structures instead of
   blobs, each verified by the build.
4. **Readability.** One structure per piece of storage instead of per-group views; field names;
   names for the remaining `data_` objects; fuller host tests for the functions that
   only have smoke tests.
5. **Beyond the main executable.** The 762 overlay modules (per-monster battle animations and
   effects) and the Konami IOP sound driver are separate programs, not yet decompiled; see
   [MODULE_INVENTORY.md](MODULE_INVENTORY.md).

## Known limitations

Alternate-typed views of one address (a single linked image allows one definition), callee
prototypes repeated across group headers, groups formed for testing rather than following the
original source files, constructs present only to steer the compiler, and descriptions that are
readings of the code rather than evidence from running the game.
