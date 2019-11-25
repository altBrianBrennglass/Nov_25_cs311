// header used by users and by time_fns-19.c
#ifdef time_fns
#else
#define time_fns
#endif

#if defined (WIN32) || defined (_WIN32)
#define ISWIN
#define HAVE_STRUCT_TIMESPEC
#include <windows.h>
#endif

#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>

/*  following struct comes from windows.h on a Windows system
typedef union _LARGE_INTEGER {
	struct { DWORD LowPart; LONG HighPart; };
	struct { DWORD LowPart; LONG HighPart; } u;
	__int64 QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
*/
//extern void time_functions_19();
// following interfaces are "exposed" for user access
extern void start_timing();  	// starts cpu and wall clock timers
extern void stop_timing();		// stops cpu and wall clock timers
extern double get_wall_clock_diff(); // uses get_wall_clock
extern void get_wall_time_ints(int *secs, int *msecs); //modifies user's parms

extern void millisleep(int mils);
extern void start_nanotime();
extern void stop_nanotime();
extern int get_nanodiff();

// Do not use the following interface in user code.
extern void get_wall_clock(time_t *secs, unsigned short *mils); // uses time_t units for internal calc.