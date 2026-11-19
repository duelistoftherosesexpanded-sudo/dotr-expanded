#ifndef DOTR_EE_MATERIAL_INTENSITY_H
#define DOTR_EE_MATERIAL_INTENSITY_H

/* Model packet walks that rewrite the lighting colours of material packets in place.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The material record's flag bits at +4 (bit 7 of byte 4: scale; byte 5 bit 0: add, read
 * directly as a byte; byte 5 bit 1: replace). The compiler allocates these bit-fields from the
 * least significant bit; on the host the same bit positions are spelled with a 32-bit unit
 * (bit-fields of unsigned char are an extension there). Assumed: the names, from what the
 * function does with each bit. The record's other fields as read: +0x30, +0x34, +0x38 scale
 * factors, +0x3c/+0x40/+0x44 a colour and +0x48 a fourth value. */
#ifdef __MWERKS__
typedef unsigned char DotrMaterialIntensityBits;
#else
typedef unsigned int DotrMaterialIntensityBits;
#endif
struct DotrMaterialIntensityFlags {
    DotrMaterialIntensityBits reserved0 : 7;
    DotrMaterialIntensityBits scale : 1;
    DotrMaterialIntensityBits add : 1;
    DotrMaterialIntensityBits replace : 1;
    DotrMaterialIntensityBits reserved1 : 6;
};

/* 0x212250 lRhModel_SetMaterialIntensity (name from the Japanese build's symbols): walks the
 * packet lists [arg0, arg1) and, when arg2 is not null, [arg2, arg3). Each 16-byte tag's top
 * nibble decides: 1 a packet of (halfword at +0) + 1 quadwords, 2 a jump to the address word at
 * +4, 3 a plain quadword, anything else ends the walk. A packet whose two first doublewords are
 * 0x10000003 and 0x6c03003c10000000 holds nine colour floats at +0x10..+0x38 (+0x1c skipped).
 * Assumed: the meanings; the jump word is a pointer-sized word on the host. */
void lRhModel_SetMaterialIntensity(unsigned char *arg0, unsigned char *arg1, unsigned char *arg2,
                                   unsigned char *arg3, unsigned char *modelFileBaseAddr);

#endif
