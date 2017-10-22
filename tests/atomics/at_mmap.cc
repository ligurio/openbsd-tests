#include "benchmark/benchmark.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/*
 *  It is important for databases and large web and proxy servers to map files
 *  into memory instead of having a buffer and reading the file contents into
 *  the buffer. If you map the file into memory directly, the operating system
 *  has more memory left for I/O buffering.
 *
 *  The Unix syscall for memory mapping files is called mmap. The performance
 *  aspect we are benchmarking here is the efficiency of the data structures
 *  the kernel uses to manage the page tables. Memory is managed in units of
 *  "pages", with one page typically being 4k. Many architectures (including
 *  x86) can to 4 MB pages as well for special occasions. On some SPARC CPUs
 *  the page size is 8k, on IA-64 it can be 4k to 16k.
 *
 *  The operating system needs to maintain two data structures for memory
 *  mapped files: one system wide "page table" for all the pages, because more
 *  than one process may do a shared mapping of the same page, plus one table
 *  for each process. The process specific table is what fork copies.
 *
 *  This benchmark takes a 200 MB file and mmaps other second 4k page of it
 *  into its address space. To make sure we measure the mmap data structure and
 *  not the hard disk, the benchmark starts by reading every of those pages
 *  once, so they are in the buffer cache. Then this benchmark takes the time
 *  it takes to mmap each page, and the time it takes to read the first byte of
 *  each page.
 *
 *  The point is that the operating system does not actually need to map a page
 *  into the address space when we mmap it. Only when we access that page, the
 *  process local page table needs to get updated. The MMU will signal an
 *  exception to the operating system as soon as the process touches the
 *  mmapped page that the OS did not actually map yet.
 *
 */

#define B_SIZE    2048
#define B_NUM     100
#define MAX_FILES 10000

void mmap_perf() {

    int fdo, i, j;
    char buffer[B_SIZE];
    struct stat s;
    size_t size;

    srand(time(NULL));

    for(i = 0; i < MAX_FILES; i++) {

        for(j = 0; j < B_SIZE; j++) {
            buffer[j] = (char)(rand() & 0xFF);
        }

        char filename[] = "at_mmap-XXXXXX";

        if ((fdo = mkstemp(filename)) == -1) {
            exit(errno);
        }

        for (j = 0; j < B_NUM; j++) {
            if (write(fdo, buffer, sizeof(buffer)) == -1) {
                exit(errno);
            }
        }

	// https://www.lemoda.net/c/mmap-example/
	// https://people.freebsd.org/%7Ejkim/mmap_test.c
	// https://gist.github.com/marcetcheverry/991042
	// https://github.com/linux-test-project/ltp/tree/master/testcases/open_posix_testsuite/conformance/interfaces/mmap

	if (fstat(fdo, &s) !=0 ) {
		perror("fstat error");
	}
	size = s.st_size;

	if ((mmap(0, size, PROT_READ, MAP_SHARED, fdo, 0)) == (caddr_t) -1) {
		perror("mmap error");
	}

        if (close(fdo) == -1) {
            exit(errno);
        }
        unlink(filename);
    }
}

static void BM_mmap(benchmark::State& state) {
    while (state.KeepRunning()) mmap_perf();
}

BENCHMARK_RANGE(BM_mmap, 1, 10 * 1000);

BENCHMARK_MAIN()
