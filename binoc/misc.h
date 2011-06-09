/* MISC.H contains funtion declarations for
*  funtions in the misc libary
*	B.G.C Oct 1989
*/
FILE *my_r_open();
FILE *my_Zopen();
FILE *uncompress();
char *useenv();
char *fgetl();
char *strup();
char *strflush();
char *findfile();
char *myscopy(char *s1, char *s2);
char *mysncopy(char *s1,char *s2, int n);
char *myfree();
char *nonewline(char *s);
char *revbyte();
float get_order(float x);
float getf();
void shcall();
void rmfile();
void mvfile();
void float_set();
void int_set();
void byte_clear();
int fstrcpy();
int checkfil();
float ElapsedSec();
int fplaces(float x, int n);

#define beep(n) { fprintf(stderr,"%c\n",(char)0x07); fflush(stderr);}
#define fbeep(fd) { fprintf(fd,"%c",(char)0x07); fflush(fd);}

int myfiletype();
char *getfilename();
char *getcomment();
char *tilde(),*CompleteFilename();
#define ASCII 15
