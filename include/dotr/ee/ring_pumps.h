#ifndef DOTR_EE_RING_PUMPS_H
#define DOTR_EE_RING_PUMPS_H

/* Adjacent routines 0x242d00-0x242f20 that move bytes between a request and the 0x50000-byte
 * ring through 0x242f20 and report to the stream objects at 0x350620 / 0x350680.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The ring as these routines use it: 0x50000 bytes and the capacity word at
 * +0x50008 (small_misc.h names all three trailing words). */
struct DotrPumpRing {
    char bytes[0x50000];
    int unknown_50000;
    int unknown_50004;
    int size;
};
/* The request: a position inside the ring and a byte count. The two leading
 * words are not read here. */
struct DotrPumpRequest {
    int unknown_00;
    int unknown_04;
    char *position;
    int size;
};
/* The object at 0x350620 (sound_file.h: data_00350620_sound, small_misc.h:
 * audioDec), only passed by address here. */
struct DotrPumpStream;
extern struct DotrPumpStream data_00350620_pump;

/* Callees under this group's argument views. 0x243390 and 0x243450 are
 * defined in sound_file with their own first-parameter types and int
 * starts; audioDecSendToIOP (0x2435a0, also in sound_file) is viewed here
 * with this group's stream type. */
extern int func_002435a0_pump(struct DotrPumpStream *stream);
extern void func_00243390_pump(struct DotrPumpStream *stream, char **start1, int *count1, char **start2, int *count2);
extern void func_00243450_pump(struct DotrPumpStream *stream, int count);
int copy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size);

/* 0x242d00. Tail-jumps (`j`, no frame) to 0x2435a0 with the 0x350620 object.
 * Assumed: no arguments, the callee's result passed on as int. */
int proceedAudio(void);
/* 0x242e40. Source: the request position + 4, wrapped by the ring capacity
 * when it reaches the ring's end, for at most size - 4 bytes; the part up to
 * the ring's end and the remainder from the ring's start go to 0x242f20 with
 * the two destination spans 0x243390 answers. The copied count is reported to
 * 0x243450; 1 when it is positive, else 0.
 * Assumed: the first argument is unused (int). */
int pcmCallback(int unused, struct DotrPumpRequest *request, struct DotrPumpRing *ring);

#endif
