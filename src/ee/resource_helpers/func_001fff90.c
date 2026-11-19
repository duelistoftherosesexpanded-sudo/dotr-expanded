/* Directory lookup with a fallback: the 0x197548 answer for `label`, or the text at
 * 0x2d0655 when that is null. The library callee's name is unknown.
 * Evidence: docs/modules/engine.md.
 */
extern char *func_00197548(const char *label);
extern char data_002d0655[];

char *SzFile_GetEnv(const char *label)
{
    char *result = func_00197548(label);
    if (!result) {
        result = data_002d0655;
    }
    return result;
}
