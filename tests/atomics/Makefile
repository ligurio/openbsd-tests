BINDIR := bin
OPTIONS := --benchmark_repetitions=10
TARGETS := at_fork at_lock
CFLAGS := $(CFLAGS) -Wall
LDFLAGS := $(LDFLAGS) -lbenchmark -lpthread

build: $(BIN_FILES)
	@mkdir -p ${BINDIR}
	g++ at_bind.cc -o ${BINDIR}/at_bind ${LDFLAGS} ${CFLAGS}
	g++ at_bpf.cc -o ${BINDIR}/at_bpf ${LDFLAGS} ${CFLAGS} -lcrypto -lssl
	g++ at_connect.cc -o ${BINDIR}/at_connect ${LDFLAGS} ${CFLAGS}
	g++ at_cpuid.cc -o ${BINDIR}/at_cpuid ${LDFLAGS} ${CFLAGS}
	g++ at_crypto.cc -o ${BINDIR}/at_crypto ${LDFLAGS} ${CFLAGS} -lssl -lcrypto
	g++ at_ctx.cc -o ${BINDIR}/at_ctx ${LDFLAGS} ${CFLAGS}
	g++ at_fork.cc -o ${BINDIR}/at_fork ${LDFLAGS} ${CFLAGS}
	g++ at_fsync.cc -o ${BINDIR}/at_fsync ${LDFLAGS} ${CFLAGS}
	g++ at_ipc.cc -o ${BINDIR}/at_ipc ${LDFLAGS} ${CFLAGS}
	g++ at_lock.cc -o ${BINDIR}/at_lock ${LDFLAGS} ${CFLAGS}
	g++ at_mmap.cc -o ${BINDIR}/at_mmap ${LDFLAGS} ${CFLAGS}
	g++ at_malloc.cc -o ${BINDIR}/at_malloc ${LDFLAGS} ${CFLAGS}
	g++ at_sigbench.cc -o ${BINDIR}/at_sigbench ${LDFLAGS} ${CFLAGS}
	g++ at_socket.cc -o ${BINDIR}/at_socket ${LDFLAGS} ${CFLAGS}

run: ${TEST}
	${BINDIR}/at_bind ${OPTIONS}
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
	${BINDIR}/at_socket ${OPTIONS}

clean:
	rm -rf ${BINDIR}

.PHONY: build clean
