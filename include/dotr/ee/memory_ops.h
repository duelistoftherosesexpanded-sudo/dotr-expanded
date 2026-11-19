#ifndef DOTR_EE_MEMORY_OPS_H
#define DOTR_EE_MEMORY_OPS_H

/* Six element-wise copy and fill loops. Each returns the destination pointer
 * advanced past the last element written. Parameter types are inferred from
 * the load/store widths. Evidence: docs/modules/render-files.md. */
#ifdef __MWERKS__
typedef __int128 DotrWord128;
#else
__extension__ typedef __int128 DotrWord128;
#endif

unsigned char *RhMem_CopyMemory(unsigned char *destination, unsigned char *source, int count);
int *RhMem_CopyMemory4(int *destination, int *source, int count);
DotrWord128 *RhMem_CopyMemory16(DotrWord128 *destination, DotrWord128 *source, int count);
unsigned char *RhMem_FillMemory(unsigned char *destination, int value, int count);
int *RhMem_FillMemory4(int *destination, int value, int count);
long *RhMem_FillMemory8(long *destination, long value, int count);

/* 0x3261f0: scratch bytes holding one element during 0x20b840. Its size is
 * unknown; the next referenced object bounds it only loosely. */
extern unsigned char data_003261f0[];

/* 0x20b840: heap sort of `count` elements of `size` bytes at `base`, ordered
 * by `compare` (negative when its first argument sorts before the second).
 * Builds the heap from element count / 2 down, then repeatedly moves the
 * last element to the scratch, the first to the end and sifts down. Copies
 * go through 0x20b300; the element being placed is always passed to
 * `compare` as the scratch address. Counts below 2 change nothing.
 * Assumed: byte-pointer element and comparison argument types (small_misc.h
 * declares the same function with int pointers for its one caller). */
void RhMath_HeapSortStruct(unsigned char *base, int count, int size, int (*compare)(unsigned char *, unsigned char *));

#endif
