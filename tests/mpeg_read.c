/* Synthetic host tests for the mpeg_read group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/mpeg_read.h"

int isWithAudio;
int isWithInter;
unsigned int controller_val;
struct DotrMpegDecoder videoDec;
static struct DotrMpegDecoder dec;
static struct DotrMpegReadBuf ring;
static struct DotrMpegFile file;
/* Only the addresses of these two are used; the test completes the types to define them. */
struct DotrMpegAudio {
    char unknown;
};
struct DotrMpegVideoOut {
    char unknown;
};
struct DotrMpegAudio audioDec;
struct DotrMpegVideoOut voBuf;

/* Every stub appends one letter to `log`; scripted results come from sequences whose last
 * value repeats. */
struct seq {
    int n, i;
    int v[6];
};
static char log[160];
static int log_n;
static struct seq state_seq, flush_seq, flushed_seq, pad_seq, count_seq, put_seq, get_seq;
static struct seq read_seq, demux_seq, full_seq, ok_seq;
static int read_pos[4], read_n, end_put[4], end_put_n, end_get[4], end_get_n;
static int demux_size[4], demux_n;
static char put_area[4], get_area[4];

static int next(struct seq *s)
{
    int v = s->v[s->i];

    if (s->i < s->n - 1) {
        s->i++;
    }
    return v;
}

static void note(char c) { log[log_n++] = c; }

int SzPad_Status(void) { note('S'); dec.unknown_08 = next(&count_seq); return 0; }
int SzPad_VSync(void) { note('V'); return 0; }
int SzPad_Act(void) { note('A'); return 0; }
int SzPad_Check(int unknown, int zero, int mask)
{
    assert(unknown == 0 && zero == 1 && mask == 0xffff);
    note('C');
    return next(&pad_seq);
}
void endDisplay(void) { note('e'); }
void startDisplay(int a) { assert(a == 1); note('s'); }
void switchThread(void) { note('t'); }
int isAudioOK(void) { note('k'); return next(&ok_seq); }
int proceedAudio(void) { note('a'); return 0; }
void audioDecPause(struct DotrMpegAudio *p) { assert(p == &audioDec); note('p'); }
void audioDecStart(struct DotrMpegAudio *p) { assert(p == &audioDec); note('o'); }
void audioDecReset(struct DotrMpegAudio *p) { assert(p == &audioDec); note('R'); }
void videoDecAbort(struct DotrMpegDecoder *p) { assert(p == &videoDec); note('x'); }
int videoDecGetState(struct DotrMpegDecoder *p) { assert(p == &dec); note('T'); return next(&state_seq); }
int videoDecFlush(struct DotrMpegDecoder *p) { assert(p == &dec); note('F'); return next(&flush_seq); }
int videoDecIsFlushed(struct DotrMpegDecoder *p) { assert(p == &dec); note('I'); return next(&flushed_seq); }
int voBufIsFull(struct DotrMpegVideoOut *p) { assert(p == &voBuf); note('f'); return next(&full_seq); }
int readBufBeginPut(struct DotrMpegReadBuf *p, char **out)
{
    assert(p == &ring);
    note('P');
    *out = put_area;
    return next(&put_seq);
}
int readBufEndPut(struct DotrMpegReadBuf *p, int count)
{
    assert(p == &ring);
    note('E');
    end_put[end_put_n++] = count;
    return 0;
}
int readBufBeginGet(struct DotrMpegReadBuf *p, char **out)
{
    assert(p == &ring);
    note('G');
    *out = get_area;
    return next(&get_seq);
}
int readBufEndGet(struct DotrMpegReadBuf *p, int count)
{
    assert(p == &ring);
    note('g');
    end_get[end_get_n++] = count;
    return 0;
}
int strFileRead(struct DotrMpegFile *f, char *buffer, int byte_count, int position)
{
    assert(f == &file && buffer == put_area && byte_count == 0x10000);
    note('r');
    read_pos[read_n++] = position;
    return next(&read_seq);
}
int func_0018dee8(struct DotrMpegDecoder *vd, char *data, int size, struct DotrMpegReadBuf *r, int ring_size)
{
    assert(vd == &dec && data == get_area && r == &ring && ring_size == 0x50000);
    note('d');
    demux_size[demux_n++] = size;
    return next(&demux_seq);
}

static void seq1(struct seq *s, int a) { s->n = 1; s->i = 0; s->v[0] = a; }
static void seq4(struct seq *s, int a, int b, int c, int d)
{
    s->n = 4;
    s->i = 0;
    s->v[0] = a;
    s->v[1] = b;
    s->v[2] = c;
    s->v[3] = d;
}

static void reset(int size, int audio, int inter)
{
    memset(log, 0, sizeof log);
    log_n = read_n = end_put_n = end_get_n = demux_n = 0;
    file.size = size;
    file.start = 700;
    ring.size = 0x50000;
    isWithAudio = audio;
    isWithInter = inter;
    controller_val = 0x5555;
    seq1(&state_seq, 0);
    seq1(&flush_seq, 1);
    seq1(&flushed_seq, 1);
    seq1(&pad_seq, 0);
    seq1(&count_seq, 0);
    seq1(&put_seq, 0);
    seq1(&get_seq, 0);
    seq1(&read_seq, 0);
    seq1(&demux_seq, 0);
    seq1(&full_seq, 0);
    seq1(&ok_seq, 0);
}

static int run(void)
{
    return readMpeg(&dec, &ring, &file);
}

/* initAll: every callee records its arguments in order in `ia`; fixtures are host values. */
volatile unsigned dotr_mpeg_d_ctrl_1000e000;
volatile unsigned dotr_mpeg_d_stat_1000e010;
struct DotrMpegFile infile;
struct DotrMpegReadBuf *readBuf;
int mpegWork, viBufData, viBufTag, timeStamp, audioBuff, voBufData, voBufTag;
void *defStack, *videoDecStack;
int defaultTh, videoDecTh;
int data_00350730[4];
char data_00317ef0[1];
char data_0030f39a[1];
static char def_stack[1], dec_stack[1], movie_name[] = "movie";
static char ia[96];
static int ia_n, open_fails, print_n, thread_n;
static struct DotrMpegThreadParam seen_param[2];
static void *seen_arg[2];
static void ev(char c) { ia[ia_n++] = c; }
void readBufCreate(struct DotrMpegReadBuf *p) { assert(p == &ring); ev('r'); }
void func_0018ea68(void) { ev('m'); }
int videoDecCreate(struct DotrMpegDecoder *p, int a, int b, int c, int d, int e, int f, int g)
{
    assert(p == &videoDec && a == 11 && b == 0x1c9168 && c == 12 && d == 13 && e == 256 && f == 14 && g == 512);
    ev('v');
    return 0;
}
void func_001906d0(void) { ev('i'); }
void func_00190810(int a, int b, int c) { assert(a == 1 && b == 0x8000 && c == 0); ev('s'); }
int audioDecCreate(struct DotrMpegAudio *p, int a, int b, int size)
{
    assert(p == &audioDec && a == 15 && b == 0xc000 && size == 24576);
    ev('a');
    return 0;
}
int videoCallback(struct DotrMpegDecoder *mp, struct DotrMpegCbData *str, struct DotrMpegReadBuf *data) { (void)mp; (void)str; (void)data; return 0; }
int pcmCallback(struct DotrMpegDecoder *mp, struct DotrMpegCbData *str, struct DotrMpegReadBuf *data) { (void)mp; (void)str; (void)data; return 0; }
void defMain(void *arg) { (void)arg; }
void videoDecMain(void *arg) { (void)arg; }
int vblankHandler(int cause) { return cause; }
int handler_endimage(int cause) { return cause; }
int videoDecSetStream(struct DotrMpegDecoder *p, int kind, int channel, DotrMpegCallback cb, struct DotrMpegReadBuf *data)
{
    assert(p == &videoDec && channel == 0 && data == &ring);
    if (kind == 0) {
        assert(cb == (DotrMpegCallback)videoCallback);
        ev('0');
    } else {
        assert(kind == 2 && cb == (DotrMpegCallback)pcmCallback);
        ev('2');
    }
    return 0;
}
void voBufCreate(struct DotrMpegVideoOut *p, int data, int tag, int capacity)
{
    assert(p == &voBuf && data == 0x2abcdef0 && tag == 17 && capacity == 2);
    ev('o');
}
int func_0017b6e0(struct DotrMpegThreadParam *p)
{
    seen_param[thread_n] = *p;
    ev('c');
    return 70 + thread_n++;
}
int func_0017b700(int id, void *arg)
{
    assert(id == 70 + thread_n - 1);
    seen_arg[thread_n - 1] = arg;
    ev('t');
    return 0;
}
int strFileOpen(struct DotrMpegFile *f, char *name)
{
    assert(f == &infile && name == movie_name);
    ev('f');
    if (open_fails > 0) {
        open_fails--;
        return 0;
    }
    return 1;
}
int func_00199b50(const char *format, ...) { assert(format == data_0030f39a); print_n++; ev('p'); return 0; }
int func_0017b5c0(int cause, int (*handler)(int), int next) { assert(cause == 2 && handler == vblankHandler && next == 0); ev('I'); return 81; }
int func_0017c428(int cause) { assert(cause == 2); ev('E'); return 0; }
int func_0017b5f0(int cause, int (*handler)(int), int next) { assert(cause == 2 && handler == handler_endimage && next == 0); ev('D'); return 82; }
int func_0017c4f8(int cause) { assert(cause == 2); ev('G'); return 0; }

static void run_init(int audio, int fails)
{
    memset(ia, 0, sizeof(ia));
    memset(seen_param, 0, sizeof(seen_param));
    ia_n = print_n = thread_n = 0;
    open_fails = fails;
    isWithAudio = audio;
    readBuf = &ring;
    mpegWork = 11; viBufData = 12; viBufTag = 13; timeStamp = 14; audioBuff = 15;
    voBufData = (int)0x8abcdef0u; voBufTag = 17;
    defStack = def_stack; videoDecStack = dec_stack;
    defaultTh = videoDecTh = 0;
    data_00350730[0] = data_00350730[1] = 0;
    dotr_mpeg_d_ctrl_1000e000 = 0x100;
    dotr_mpeg_d_stat_1000e010 = 0;
    initAll(movie_name);
    assert(dotr_mpeg_d_ctrl_1000e000 == 0x103 && dotr_mpeg_d_stat_1000e010 == 4);
    assert(defaultTh == 70 && videoDecTh == 71 && data_00350730[1] == 81 && data_00350730[0] == 82);
    assert(seen_param[0].entry == defMain && seen_param[0].stack == def_stack && seen_param[0].stackSize == 2048);
    assert(seen_param[0].initPriority == 32 && seen_param[0].gpReg == data_00317ef0 && seen_param[0].option == 0);
    assert(seen_param[1].entry == videoDecMain && seen_param[1].stack == dec_stack && seen_param[1].stackSize == 16384);
    assert(seen_param[1].initPriority == 32 && seen_param[1].gpReg == data_00317ef0 && seen_param[1].option == 0);
    assert(seen_arg[0] == NULL && seen_arg[1] == &videoDec);
    assert(print_n == fails);
}

static void test_init_all(void)
{
    /* No audio, the file opens at once. */
    run_init(0, 0);
    assert(strcmp(ia, "rmvisa0octctfIEDG") == 0);
    /* Audio stream registered too; the open fails twice, with one message each time. */
    run_init(1, 2);
    assert(strcmp(ia, "rmvisa02octctfpfpfIEDG") == 0);
}

int main(void)
{
    /* Four bytes left: the loop is skipped without asking the decoder. Flush refused once,
     * then the wait: not flushed and not ended once. Audio on: reset. */
    reset(4, 1, 1);
    seq4(&flush_seq, 0, 1, 1, 1);
    seq4(&flushed_seq, 0, 1, 1, 1);
    assert(run() == 1 && strcmp(log, "FtFITtIeR") == 0);
    /* Five bytes but the decoder has ended (3): skipped; the wait also ends on state 3.
     * Audio off: no reset. */
    reset(5, 0, 1);
    seq1(&state_seq, 3);
    seq1(&flushed_seq, 0);
    assert(run() == 1 && strcmp(log, "TFITe") == 0);
    /* Interrupt: new button 0x20 (the high bits of the pad word are dropped), counter 11. */
    reset(0x30000, 1, 1);
    seq1(&pad_seq, 0x10020);
    seq1(&count_seq, 11);
    assert(run() == -1 && strcmp(log, "TSVACepx") == 0 && controller_val == 0x20);
    reset(0x30000, 0, 1);
    seq1(&pad_seq, 0x800);
    seq1(&count_seq, 11);
    assert(run() == -1 && strcmp(log, "TSVACex") == 0 && controller_val == 0x800);
    /* Interruption not allowed: the same press does nothing; one pass, then state 3. */
    reset(5, 0, 0);
    seq4(&state_seq, 0, 3, 3, 3);
    seq1(&pad_seq, 0x20);
    seq1(&count_seq, 11);
    assert(run() == 1 && strcmp(log, "TSVACPtGafTFIe") == 0);
    /* Four passes. Pad: 0x20 with counter 10 (bound: not above 10), 0x20 held with counter
     * 11 (not newly pressed), then 0x10 (outside the mask). Put room 0xffff (no read), then
     * 0x10000 twice (reads of 0x1800 bytes: sector offsets 0 and 3), then nothing left to
     * read. Get sizes 0 (no demultiplex), then three passes that consume 0x1000, 0x1000 and
     * 0xffc, leaving exactly 4. Output buffer not full; full but audio not ready; both, which
     * starts display and audio once. */
    reset(0x3000, 1, 1);
    seq4(&pad_seq, 0x20, 0x20, 0x10, 0x10);
    seq4(&count_seq, 10, 11, 11, 11);
    seq4(&put_seq, 0xffff, 0x10000, 0x10000, 0x10000);
    seq1(&read_seq, 0x1800);
    seq4(&get_seq, 0, 0x100, 0x200, 0x300);
    seq4(&demux_seq, 0x1000, 0x1000, 0xffc, 0);
    seq4(&full_seq, 0, 1, 1, 1);
    seq4(&ok_seq, 0, 1, 1, 1);
    assert(run() == 1);
    assert(strcmp(log, "TSVACPtGaf" "TSVACPrEtGdgafk" "TSVACPrEtGdgafkso" "TSVACPtGdga" "FIeR") == 0);
    assert(read_n == 2 && read_pos[0] == 700 && read_pos[1] == 703);
    assert(end_put_n == 2 && end_put[0] == 0x1800 && end_put[1] == 0x1800);
    assert(demux_n == 3 && demux_size[0] == 0x100 && demux_size[1] == 0x200 && demux_size[2] == 0x300);
    assert(end_get_n == 3 && end_get[0] == 0x1000 && end_get[1] == 0x1000 && end_get[2] == 0xffc);
    assert(controller_val == 0x10);
    test_init_all();
    return 0;
}
