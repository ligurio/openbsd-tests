#include "benchmark/benchmark.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

void getifaddrs_perf()
{
        struct ifaddrs *ifaddr, *ifa;
        int family, s, n;
        char host[NI_MAXHOST];

        if (getifaddrs(&ifaddr) == -1) {
                perror("getifaddrs");
                exit(EXIT_FAILURE);
        }

        /* touch the data */
        for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
                if (ifa->ifa_addr == NULL)
                        continue;
                family = ifa->ifa_addr->sa_family;
        }

        freeifaddrs(ifaddr);
}

void BM_getifaddrs(benchmark::State& state) {
	  while (state.KeepRunning()) getifaddrs_perf();
}

BENCHMARK(BM_getifaddrs);

BENCHMARK_MAIN()
