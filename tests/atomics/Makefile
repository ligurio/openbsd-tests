BINDIR = bin

build:
	@mkdir -p ${BINDIR}
	g++ at_bcrypt.cc -o ${BINDIR}/at_bcrypt -lbenchmark -lpthread -lcrypto -lssl
	g++ at_cpuid.cc -o ${BINDIR}/at_cpuid -lbenchmark -lpthread
	g++ at_ctx.cc -o ${BINDIR}/at_ctx -lbenchmark -lpthread
	g++ at_fork.cc -o ${BINDIR}/at_fork -lbenchmark -lpthread
	g++ at_fsync.cc -o ${BINDIR}/at_fsync -lbenchmark -lpthread
	g++ at_mmap.cc -o ${BINDIR}/at_mmap -lbenchmark -lpthread
	g++ at_sigbench.cc -o ${BINDIR}/at_sigbench -lbenchmark -lpthread
	g++ at_sha256.cc -o ${BINDIR}/at_sha256 -lbenchmark -lpthread -lssl -lcrypto

run:
	${BINDIR}/at_bcrypt
	${BINDIR}/at_cpuid
	${BINDIR}/at_ctx
	${BINDIR}/at_fork
	${BINDIR}/at_fsync
	${BINDIR}/at_mmap
	${BINDIR}/at_sigbench
	${BINDIR}/at_sha256

clean:
	rm -rf ${BINDIR}
