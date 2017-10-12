#include "benchmark/benchmark.h"
#include <openssl/sha.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

bool sha256(void* input, unsigned long length)
{
    unsigned char *md = 0;

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return false;

    if(!SHA256_Final(md, &context))
        return false;

    return true;
}

void BM_SHA256(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(sha256);
	  }
}

BENCHMARK(BM_SHA256);

void rand_string(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) random() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

#ifdef __OpenBSD__
void bcrypt_perf(int length) {
	char buffer[50];
	const char *pref;
	char string[1024];

	//rand_string(string, length);
	if (crypt_newhash("sergeyb", pref, buffer, sizeof(buffer)) != 0)
		err(1, "can't generate hash");
}
#endif

void BM_bcrypt(benchmark::State& state) {
#ifdef __OpenBSD__
    while (state.KeepRunning()) bcrypt_perf(state.range(0));
#else
    state.SkipWithError("Unsupported");
#endif
}

BENCHMARK_RANGE(BM_bcrypt, 1, 10 * 10);

BENCHMARK_MAIN()
