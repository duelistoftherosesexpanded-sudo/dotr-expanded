#ifndef DOTR_EE_OVERLAY_H
#define DOTR_EE_OVERLAY_H

/* Fields used by EE helper 0x17b410. This prefix describes the target
 * pointer layout only when compiled for its 32-bit pointer ABI. Host tests use
 * native pointers to exercise call order and range behavior, not EE offsets.
 * Evidence: docs/modules/other.md and docs/topics/overlay-loader.md.
 */
typedef void (*DotrOverlayInitializer)(void);
typedef char DotrOverlayIntMustBe32Bits[(sizeof(unsigned int) == 4) ? 1 : -1];

struct DotrOverlayInitPrefix {
    unsigned int unconsumed_words_000[5];
    unsigned int zero_fill_size;
    DotrOverlayInitializer *initializers_begin;
    DotrOverlayInitializer *initializers_end;
};

/* Names from the Japanese debug data; the signatures are inferred. */
void mwOverlayInit(void *address, int initialized_size);
void __initialize_cpp_rts(DotrOverlayInitializer *begin, DotrOverlayInitializer *end,
                   int unused_2, int unused_3);

#endif
