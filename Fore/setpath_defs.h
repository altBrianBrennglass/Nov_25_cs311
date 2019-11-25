// In Linux The following block of 6 lines of code must be at the top of your file, 
// after your regular #includes, such as stdio.h.
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif
extern void pause311();
extern void setpath311(char);

#ifndef setset
#define setset
#include <stdio.h>
#include <string.h> // vs stdlib.h, because of some builtin defs in gcc
// Following variables MUST NOT be initialized here

extern char in_path[200];
extern char out_path[200];
extern char infilename[10];
extern char outfilename[11];

extern       FILE* infile;
extern       FILE* outfile;
#endif
 
