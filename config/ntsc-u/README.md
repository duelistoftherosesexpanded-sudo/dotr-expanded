# NTSC-U target configuration

| File | |
| --- | --- |
| `disc.json` | identity of the supported unmodified image (size, MD5, boot path); `make prepare` checks your image against it and records SHA-256 |
| `symbols.csv` | the symbol registry: every function and named data object, its range, status, source and description document ([rules](../../docs/DECOMPILATION.md)) |
| `function-builds.json` | how `make match` builds each function: source, headers, external symbol addresses, and optional warm build, placements and read-only data |
| `jp-names.csv`, `jp-data-names.csv`, `jp-source-files.csv` | names and source-file names carried over from the Japanese release ([how](../../docs/topics/jp-build-symbols.md)) |
