FILES = at_hello at_hello1
CFLAGS = -static -O3

build:
	g++ at_fork.cc -o at_fork -lbenchmark -lpthread
	g++ at_cpuid.cc -o at_cpuid -lbenchmark -lpthread
	g++ at_syscall_overhead.cc -o at_syscall_overhead -lbenchmark -lpthread
	g++ at_sigbench.cc -o at_sigbench -lbenchmark -lpthread
	#g++ at_fsync.cc -o at_fsync -lbenchmark -lpthread
	#g++ at_sha256.cc -o at_sha256 -lbenchmark -lpthread -lssl

clean:
	rm bench-[cs]
