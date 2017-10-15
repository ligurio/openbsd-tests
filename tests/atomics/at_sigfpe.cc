#include "benchmark/benchmark.h"
#include <sys/socket.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/*
 * The main idea of this test is to generate hardware "division by zero"
 * exception, and measure how long it will be proceeded.
 */

jmp_buf context;

void sig_fpe_handler(int signo) {
        longjmp(context, 1);
}

void sig_fpe(void) {

	int a = 0, b = 0;
        struct sigaction sa;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sig_fpe_handler;
        sa.sa_flags = SA_NODEFER;
        sigaction(SIGFPE, &sa, NULL);

        try {
#ifdef __UNIX__
                if (!setjmp(context))
#endif
                        a = a / b;
        } catch(...) {
                /* */
        }
}

void BM_sig_fpe(benchmark::State& state) {
	while (state.KeepRunning()) sig_fpe();
}

BENCHMARK(BM_sig_fpe);

BENCHMARK_MAIN()

/*
#include <iostream>
using namespace std;

double division(int a, int b)
{
   if( b == 0 )
   {
      throw "Division by zero condition!";
   }
   return (a/b);
}

int main ()
{
   int x = 50;
   int y = 0;
   double z = 0;

   try {
     z = division(x, y);
     cout << z << endl;
   }catch (const char* msg) {
     cerr << msg << endl;
   }

   return 0;
}
*/
