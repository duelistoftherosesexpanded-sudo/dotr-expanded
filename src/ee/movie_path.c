/* Evidence: docs/modules/game-modes.md.
 * Callee prototypes are assumed from these calls. The movie
 * table's length, and so the safe index range, is not established.
 */
extern char *SzFile_GetEnv(const char *);
extern int func_0019b648(char *, const char *, ...);
extern int SzGame_PlayMovie(const char *, int, int);
extern char data_002be80f[];
extern char data_002be809[];
extern const char *data_0028ff10[];

void SzGame_PlayMovieNo(int index, int arg1, int arg2)
{
    char path[128];
    char *directory = SzFile_GetEnv(data_002be80f);
    func_0019b648(path, data_002be809, directory, data_0028ff10[index]);
    SzGame_PlayMovie(path, arg1, arg2);
}
