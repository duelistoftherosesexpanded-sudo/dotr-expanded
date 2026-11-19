/* Synthetic host checks for the allocation wrappers and the group mapping. */
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/resource_support.h"

unsigned short AlternateCardIds[854];
unsigned short gSzDuel_CardDefaultCamDist[854];
static int notify_calls, notify_first, notify_second;

int SzData_GetOption(int unknown_0, int unknown_1)
{
    notify_calls++;
    notify_first = unknown_0;
    notify_second = unknown_1;
    return 0x66;
}

enum { LENGTH = 1, ALLOCATE, RELEASE, COPY };
static int trace[8], trace_count, allocate_alignment, allocate_size, allocate_result, released;
static char block[32];

int func_00197990(int alignment, int byte_count)
{
    allocate_alignment = alignment;
    allocate_size = byte_count;
    trace[trace_count++] = ALLOCATE;
    return allocate_result;
}

void func_00197c18(int address)
{
    released = address;
    trace[trace_count++] = RELEASE;
}

/* Assumed to return a text length and to copy a text, like the standard routines. */
int func_0019bef0(const char *string)
{
    trace[trace_count++] = LENGTH;
    return (int)strlen(string);
}

char *func_0019bdd8(char *destination, const char *source)
{
    trace[trace_count++] = COPY;
    return strcpy(destination, source);
}

int main(void)
{
    intptr_t block_address = (intptr_t)block;
    int i;

    /* Positive values add five; nonpositive values preserve the second word. */
    assert(SzDuel_Type2Attr(-123, 456) == 456);
    assert(SzDuel_Type2Attr(0, -789) == -789);
    assert(SzDuel_Type2Attr(1, 999) == 6);
    assert(SzDuel_Type2Attr(INT_MAX - 5, -1) == INT_MAX);

    /* 0x1de020 maps 1..5 to 1, 1, 2, 2, 3 and passes every other value through. */
    {
        static const int mapped[] = {1, 1, 2, 2, 3};
        for (i = 1; i <= 5; i++) {
            assert(SzDuel_Kind2Type(i) == mapped[i - 1]);
            /* 0x1de0b0 adds five to the positive mapped value. */
            assert(SzDuel_Kind2Attr(i, -77) == mapped[i - 1] + 5);
        }
    }
    assert(SzDuel_Kind2Type(0) == 0 && SzDuel_Kind2Type(6) == 6 && SzDuel_Kind2Type(-1) == -1);
    assert(SzDuel_Kind2Type(INT_MIN) == INT_MIN && SzDuel_Kind2Type(INT_MAX) == INT_MAX);
    assert(SzDuel_Kind2Attr(6, -77) == 11 && SzDuel_Kind2Attr(INT_MAX - 5, -77) == INT_MAX);
    assert(SzDuel_Kind2Attr(0, -77) == -77 && SzDuel_Kind2Attr(-1, 456) == 456 && SzDuel_Kind2Attr(INT_MIN, 0) == 0);

    /* Tail calls: fixed alignment 16, argument and result forwarded. */
    allocate_result = 0x1234;
    assert(SzMem_Alloc(100) == 0x1234 && allocate_alignment == 16 && allocate_size == 100);
    SzMem_Free(0x4321);
    assert(released == 0x4321 && trace_count == 2);

    /* Text copy: null text and a failed request do nothing further. */
    trace_count = 0;
    assert(SzMem_StrDup(0) == 0 && trace_count == 0);
    allocate_result = 0;
    assert(SzMem_StrDup("abc") == 0);
    assert(trace_count == 2 && trace[0] == LENGTH && trace[1] == ALLOCATE && allocate_size == 4);

    /* Group mapping on a synthetic table. */
    for (i = 0; i < 854; i++) {
        AlternateCardIds[i] = (unsigned short)(853 - i);
    }
    AlternateCardIds[10] = 671;   /* replaced by 999 */
    AlternateCardIds[11] = 854;   /* out of range */
    AlternateCardIds[12] = 0xffff;
    AlternateCardIds[20] = 5;     /* duplicate of the value at index 848 */
    assert(SzDuel_Xax2New(-1) == 999 && SzDuel_Xax2New(854) == 999);
    assert(SzDuel_Xax2New(0) == 853 && SzDuel_Xax2New(853) == 0);
    assert(SzDuel_Xax2New(10) == 999 && SzDuel_Xax2New(11) == 999 && SzDuel_Xax2New(12) == 999);
    assert(SzDuel_Xax2New(20) == 5);

    /* Inverse: first index holding the value; 671 and out-of-range values give 999. */
    assert(SzDuel_New2Xax(853) == 0 && SzDuel_New2Xax(0) == 853);
    assert(SzDuel_New2Xax(5) == 20);
    assert(SzDuel_New2Xax(671) == 999 && SzDuel_New2Xax(-1) == 999 && SzDuel_New2Xax(854) == 999);
    assert(SzDuel_New2Xax(843) == 999); /* index 10 was overwritten */
    assert(SzDuel_New2Xax(999) == 999);

    /* Adjacent index helpers: the call precedes the range test in 0x1de210. */
    assert(SzDuel_GetMsgName(0) == 320 && SzDuel_GetMsgName(853) == 1173);
    assert(notify_calls == 2 && notify_first == 0 && notify_second == 0);
    assert(SzDuel_GetMsgName(-1) == -1 && SzDuel_GetMsgName(854) == -1 && notify_calls == 4);
    gSzDuel_CardDefaultCamDist[0] = 0xffff;
    gSzDuel_CardDefaultCamDist[853] = 7;
    assert(SzDuel_GetCamDist(0) == 0xffff && SzDuel_GetCamDist(853) == 7);
    assert(SzDuel_GetCamDist(-1) == -1 && SzDuel_GetCamDist(854) == -1);
    assert(SzDuel_GetOption3DBattle() == 0x66 && notify_calls == 5 && notify_second == 1);

    /* Successful copy: needs the block address to fit a 32-bit word. */
    if (block_address != (int)block_address) {
        puts("skipped: host data addresses do not fit a 32-bit word");
        return 77;
    }
    allocate_result = (int)block_address;
    trace_count = 0;
    memset(block, '#', sizeof(block));
    assert(SzMem_StrDup("text") == (int)block_address);
    assert(trace_count == 3 && trace[0] == LENGTH && trace[1] == ALLOCATE && trace[2] == COPY);
    assert(allocate_alignment == 16 && allocate_size == 5);
    assert(strcmp(block, "text") == 0 && block[5] == '#');

    puts("resource_support: ok");
    return 0;
}
