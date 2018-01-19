BINDIR := bin
OPTIONS += --benchmark_repetitions=10
CFLAGS += -Wall
LDFLAGS += -lbenchmark -lpthread
CC := g++

build:
	@mkdir -p ${BINDIR}
	${CC} at_2d.cc -o 2d -L/usr/X11R6/lib/ -lGL -lGLU -lglut
	${CC} at_bind.cc -o ${BINDIR}/at_bind ${LDFLAGS} ${CFLAGS}
	${CC} at_bpf.cc -o ${BINDIR}/at_bpf ${LDFLAGS} ${CFLAGS}
	#${CC} at_busyloop.cc -o ${BINDIR}/at_busyloop ${LDFLAGS} ${CFLAGS}
	${CC} at_connect.cc -o ${BINDIR}/at_connect ${LDFLAGS} ${CFLAGS}
	${CC} at_cpuid.cc -o ${BINDIR}/at_cpuid ${LDFLAGS} ${CFLAGS}
	${CC} at_crypto.cc -o ${BINDIR}/at_crypto ${LDFLAGS} ${CFLAGS}
	${CC} at_ctx.cc -o ${BINDIR}/at_ctx ${LDFLAGS} ${CFLAGS}
	${CC} at_execv.cc -o ${BINDIR}/at_execv ${LDFLAGS} ${CFLAGS}
	${CC} at_fork.cc -o ${BINDIR}/at_fork ${LDFLAGS} ${CFLAGS}
	${CC} at_fsync.cc -o ${BINDIR}/at_fsync ${LDFLAGS} ${CFLAGS}
	${CC} at_getifaddrs.cc -o ${BINDIR}/at_getifaddrs ${LDFLAGS} ${CFLAGS}
	${CC} at_ipc.cc -o ${BINDIR}/at_ipc ${LDFLAGS} ${CFLAGS}
	${CC} at_lock.cc -o ${BINDIR}/at_lock ${LDFLAGS} ${CFLAGS}
	${CC} at_malloc.cc -o ${BINDIR}/at_malloc ${LDFLAGS} ${CFLAGS}
	${CC} at_mmap.cc -o ${BINDIR}/at_mmap ${LDFLAGS} ${CFLAGS}
	${CC} at_sigbench.cc -o ${BINDIR}/at_sigbench ${LDFLAGS} ${CFLAGS}
	${CC} at_socket.cc -o ${BINDIR}/at_socket ${LDFLAGS} ${CFLAGS}
	${CC} at_vmm.cc -o ${BINDIR}/at_vmm ${LDFLAGS} ${CFLAGS} -Wno-unused-but-set-variable -Wno-div-by-zero

run:
	${BINDIR}/at_2d ${OPTIONS}
	${BINDIR}/at_bind ${OPTIONS}
	${BINDIR}/at_bpf ${OPTIONS}
	#${BINDIR}/at_busyloop ${OPTIONS}
	${BINDIR}/at_connect ${OPTIONS}
	${BINDIR}/at_cpuid ${OPTIONS}
	${BINDIR}/at_crypto ${OPTIONS}
	${BINDIR}/at_ctx ${OPTIONS}
	${BINDIR}/at_execv ${OPTIONS}
	${BINDIR}/at_fork ${OPTIONS}
	${BINDIR}/at_fsync ${OPTIONS}
	${BINDIR}/at_getifaddrs ${OPTIONS}
	${BINDIR}/at_ipc ${OPTIONS}
	${BINDIR}/at_lock ${OPTIONS}
	${BINDIR}/at_malloc ${OPTIONS}
	${BINDIR}/at_mmap ${OPTIONS}
	${BINDIR}/at_sigbench ${OPTIONS}
	${BINDIR}/at_socket ${OPTIONS}
	${BINDIR}/at_vmm ${OPTIONS}

clean:
	rm -rf ${BINDIR}

.PHONY: build clean
