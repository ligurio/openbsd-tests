BINDIR = bin
CFLAGS = -lbenchmark -lpthread
OPTIONS = --benchmark_repetitions=10
SOURCES = at_fork.cc at_lock.cc

build: $(BIN_FILES)
	@mkdir -p ${BINDIR}
	g++ at_bpf.cc -o ${BINDIR}/at_bpf ${CFLAGS} -lcrypto -lssl
	g++ at_connect.cc -o ${BINDIR}/at_connect ${CFLAGS}
	g++ at_cpuid.cc -o ${BINDIR}/at_cpuid ${CFLAGS}
	g++ at_crypto.cc -o ${BINDIR}/at_crypto ${CFLAGS} -lssl -lcrypto
	g++ at_ctx.cc -o ${BINDIR}/at_ctx ${CFLAGS}
	g++ at_fork.cc -o ${BINDIR}/at_fork ${CFLAGS}
	g++ at_fsync.cc -o ${BINDIR}/at_fsync ${CFLAGS}
	g++ at_ipc.cc -o ${BINDIR}/at_ipc ${CFLAGS}
	g++ at_lock.cc -o ${BINDIR}/at_lock ${CFLAGS}
	g++ at_mmap.cc -o ${BINDIR}/at_mmap ${CFLAGS}
	g++ at_malloc.cc -o ${BINDIR}/at_malloc ${CFLAGS}
	g++ at_sigbench.cc -o ${BINDIR}/at_sigbench ${CFLAGS}

run: ${TEST}
	${BINDIR}/at_bpf ${OPTIONS}
	${BINDIR}/at_connect ${OPTIONS}
	${BINDIR}/at_cpuid ${OPTIONS}
	${BINDIR}/at_crypto ${OPTIONS}
	${BINDIR}/at_ctx ${OPTIONS}
	${BINDIR}/at_fork ${OPTIONS}
	${BINDIR}/at_fsync ${OPTIONS}
	${BINDIR}/at_ipc ${OPTIONS}
	${BINDIR}/at_lock ${OPTIONS}
	${BINDIR}/at_mmap ${OPTIONS}
	${BINDIR}/at_malloc ${OPTIONS}
	${BINDIR}/at_sigbench ${OPTIONS}

clean:
	rm -rf ${BINDIR}
