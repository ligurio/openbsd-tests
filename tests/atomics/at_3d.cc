#include "benchmark/benchmark.h"

// https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/examples.html

void perf_3d() {

	// render 3D puffy

}

void BM_3d(benchmark::State& state) {
	while (state.KeepRunning()) perf_3d();
}

BENCHMARK(BM_3d);

BENCHMARK_MAIN()
