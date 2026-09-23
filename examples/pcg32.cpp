// Minimal PCG32 (pcg32_random_r, O'Neill's "XSH RR" variant) generator that
// streams raw 32-bit output to stdout for testing with PractRand's RNG_test.
//
// Build:
//   g++ -std=c++14 -O3 -Wall -o pcg32 examples/pcg32.cpp
// Run:
//   ./pcg32 | ./RNG_test stdin32
// Reproduce a specific run (seed and sequence are printed to stderr each run):
//   ./pcg32 <seed-hex> <sequence-hex> | ./RNG_test stdin32

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <random>

struct pcg32_random_t {
	uint64_t state;
	uint64_t inc;
};

static uint32_t pcg32_random_r(pcg32_random_t *rng) {
	uint64_t oldstate = rng->state;
	rng->state = oldstate * 6364136223846793005ULL + rng->inc;
	uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
	uint32_t rot = (uint32_t)(oldstate >> 59u);
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static void pcg32_srandom_r(pcg32_random_t *rng, uint64_t initstate, uint64_t initseq) {
	rng->state = 0U;
	rng->inc = (initseq << 1u) | 1u;
	pcg32_random_r(rng);
	rng->state += initstate;
	pcg32_random_r(rng);
}

int main(int argc, char **argv) {
	freopen(NULL, "wb", stdout);  // Only necessary on Windows, but harmless.

	uint64_t seed, seq;
	if (argc >= 3) {
		seed = strtoull(argv[1], NULL, 16);
		seq  = strtoull(argv[2], NULL, 16);
	}
	else {
		std::random_device rd;
		seed = (uint64_t(rd()) << 32) | rd();
		seq  = (uint64_t(rd()) << 32) | rd();
	}
	std::fprintf(stderr, "pcg32(0x%016llx, 0x%016llx) initialized.\n",
		(unsigned long long)seed, (unsigned long long)seq);

	pcg32_random_t rng;
	pcg32_srandom_r(&rng, seed, seq);

	constexpr size_t BUFFER_SIZE = 1024 * 1024 / sizeof(uint32_t);
	static uint32_t buffer[BUFFER_SIZE];

	while (1) {
		for (size_t i = 0; i < BUFFER_SIZE; ++i)
			buffer[i] = pcg32_random_r(&rng);
		fwrite((void *)buffer, sizeof(buffer[0]), BUFFER_SIZE, stdout);
	}
}
