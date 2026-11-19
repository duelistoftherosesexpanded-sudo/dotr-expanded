#ifndef DOTR_EE_MOVIE_START_H
#define DOTR_EE_MOVIE_START_H

/* Compatible packet view, not an established original SDK type. */
typedef unsigned long long PacketWord64;
typedef union PacketQuadword {
    unsigned int words[4];
    PacketWord64 halves[2];
} PacketQuadword;

int SzGame_PlayMovie(const char *, int, int);
void SzGame_ClearGsMem(int, int, int, int, int, int);

#endif
