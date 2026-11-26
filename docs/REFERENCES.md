# References

| Reference | Used for | Notes |
| --- | --- | --- |
| [kira-502/dotr-decomp](https://github.com/kira-502/dotr-decomp) | C for 166 functions ([CREDITS.md](CREDITS.md)); the warm-compile finding; original-file units; comparison of matches | CC0-1.0. A parallel decompilation of the same executable that builds the whole executable and all 762 overlay modules byte-identical |
| The Japanese release (`SLPM_650.50`) | original function and data names, some parameter, local and structure names, source file names for the rules, effect and part of the AI code | its executable keeps its symbol table and DWARF 1 debug data; see [topics/jp-build-symbols.md](topics/jp-build-symbols.md) |
| batzpup's Ghidra project for `SLUS_205.15` (Ghidra 9.0, 2019) | hand-made labels, structure and enum definitions, parameter names and comments; the name `useSoftFocus` | the Japanese symbols stay authoritative for function names; types are a reverse engineer's reading |
| [GenericMadScientist/DotR-Documentation](https://github.com/GenericMadScientist/DotR-Documentation) | early function and RAM maps, card and file formats | cited at commit `cb5bf0b70767ede26550e801b9519f6b99fad21a`; reported facts, rechecked here |
| [batzpup/DotrModdingToolRedux](https://github.com/batzpup/DotrModdingToolRedux) | data-editing context | GPL-3.0; no code copied |
| DOTR-Rebuilt (a model-extraction project) | the disc's size, MD5, serial and entry point, from which the identity profile started | rechecked by `make verify` |
| [decomp.me compilers](https://github.com/decompme/compilers/releases/tag/compilers), [wibo](https://github.com/decompals/wibo) | the Metrowerks compilers tried, and running them on Linux | pinned by hash in `config/tooling-lock.json` |
