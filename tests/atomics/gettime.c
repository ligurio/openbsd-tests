#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

// https://github.com/btorpey/clocks/blob/master/clocks.c
// http://repnop.org/carte/snippets/time.c

int main(int argc, char** argv)
{
   printf("%25s\t%15s\t%15s\t%15s\n", "clock", "res (ns)", "secs", "nsecs");

   struct timeval tv;
   gettimeofday(&tv, NULL);
   printf("%25s\t%15s\t%15ld\t", "gettimeofday","1,000", tv.tv_sec);
   printf("%15ul\n", tv.tv_usec*1000);

#if _POSIX_TIMERS > 0

   struct timespec x;

   #ifdef CLOCK_REALTIME
   clock_getres(CLOCK_REALTIME, &x);
   printf("%25s\t%15ld", "CLOCK_REALTIME", x.tv_nsec);
   clock_gettime(CLOCK_REALTIME, &x);
   printf("\t%15ld\t%15ld\n", x.tv_sec, x.tv_nsec);
   #endif

   #ifdef CLOCK_REALTIME_COARSE
   clock_getres(CLOCK_REALTIME_COARSE, &x);
   printf("%25s\t%15ld", "CLOCK_REALTIME_COARSE", x.tv_nsec);
   clock_gettime(CLOCK_REALTIME_COARSE, &x);
   printf("\t%15ld\t%15ld\n", x.tv_sec, x.tv_nsec);
   #endif

   #ifdef CLOCK_REALTIME_HR
   clock_getres(CLOCK_REALTIME_HR, &x);
   printf("%25s\t%15ld", "CLOCK_REALTIME_HR", x.tv_nsec);
   clock_gettime(CLOCK_REALTIME_HR, &x);
   printf("\t%15ld\t%15ld\n", x.tv_sec, x.tv_nsec);
   #endif

   #ifdef CLOCK_MONOTONIC
   clock_getres(CLOCK_MONOTONIC, &x);
   printf("%25s\t%15ld", "CLOCK_MONOTONIC", x.tv_nsec);
   clock_gettime(CLOCK_MONOTONIC, &x);
   printf("\t%15ld\t%15ld\n", x.tv_sec, x.tv_nsec);
   #endif

   #ifdef CLOCK_MONOTONIC_RAW
   clock_getres(CLOCK_MONOTONIC_RAW, &x);
   printf("%25s\t%15ld", "CLOCK_MONOTONIC_RAW", x.tv_nsec);
   clock_gettime(CLOCK_MONOTONIC_RAW, &x);
   printf("\t%15ld\t%15ld\n", x.tv_sec, x.tv_nsec);
   #endif

   #ifdef CLOCK_MONOTONIC_COARSE
   clock_getres(CLOCK_MONOTONIC_COARSE, &x);
   printf("%25s\t%15ld", "CLOCK_MONOTONIC_COARSE", x.tv_nsec);
   clock_gettime(CLOCK_MONOTONIC_COARSE, &x);
   printf("\t%15ld\t%15ld\n", x.tv_sec, x.tv_nsec);
   #endif

#endif

   return 0;
}
