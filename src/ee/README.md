# Emotion Engine source

One function per file, `<group>/func_<address>.c`, with the function under its original name.
Groups collect related functions (by subject or shared data); they are not the original source
files (see [ROADMAP.md](../../docs/ROADMAP.md)). Each group has a header in
`include/dotr/ee/<group>.h` and a host test in `tests/<group>.c` (the one exception, `gt4_box`,
is tested with `paint_prims`).

A few files hold several functions as one comparison unit (for example
`resource_submit/unit_wait.c`); the four original-file units hold a function's original
predecessors before it ([rules](../../docs/DECOMPILATION.md#original-file-units)).
Run `make match` after changing any source.
