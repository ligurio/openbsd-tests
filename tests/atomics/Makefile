BINDIR := bin
OPTIONS += --benchmark_repetitions=10
CFLAGS += -Wall
LDFLAGS += -lbenchmark -lpthread
CC := g++

build:
	@mkdir -p ${BINDIR}
	${CC} at_cpuid.cc -o ${BINDIR}/at_cpuid ${LDFLAGS} ${CFLAGS}
	${CC} at_crypto.cc -o ${BINDIR}/at_crypto ${LDFLAGS} ${CFLAGS}
	${CC} at_fsync.cc -o ${BINDIR}/at_fsync ${LDFLAGS} ${CFLAGS}
	${CC} at_getifaddrs.cc -o ${BINDIR}/at_getifaddrs ${LDFLAGS} ${CFLAGS}
	${CC} at_malloc.cc -o ${BINDIR}/at_malloc ${LDFLAGS} ${CFLAGS}
	${CC} at_mmap.cc -o ${BINDIR}/at_mmap ${LDFLAGS} ${CFLAGS}
	${CC} at_sigbench.cc -o ${BINDIR}/at_sigbench ${LDFLAGS} ${CFLAGS}
	${CC} at_vmm.cc -o ${BINDIR}/at_vmm ${LDFLAGS} ${CFLAGS} -Wno-unused-but-set-variable -Wno-div-by-zero

run:
	${BINDIR}/at_cpuid ${OPTIONS}
	${BINDIR}/at_crypto ${OPTIONS}
	${BINDIR}/at_fsync ${OPTIONS}
	${BINDIR}/at_getifaddrs ${OPTIONS}
	${BINDIR}/at_malloc ${OPTIONS}
	${BINDIR}/at_mmap ${OPTIONS}
	${BINDIR}/at_sigbench ${OPTIONS}
	${BINDIR}/at_vmm ${OPTIONS}

clean:
	rm -rf ${BINDIR}

.PHONY: build clean
