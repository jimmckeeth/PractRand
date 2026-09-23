// Xoshiro256** (Blackman & Vigna, 2018) generator that streams raw 64-bit
// output to stdout for testing with PractRand's RNG_test.  The 256-bit state
// is initialized from a single 64-bit seed via splitmix64, as recommended by
// the algorithm's authors.
//
// Build:
//   g++ -std=c++14 -O3 -Wall -o xoshiro256starstar examples/xoshiro256starstar.cpp
// Run:
//   ./xoshiro256starstar | ./RNG_test stdin64
// Reproduce a specific run (seed is printed to stderr each run):
//   ./xoshiro256starstar <seed-hex> | ./RNG_test stdin64

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <random>

static uint64_t splitmix64_state;

static uint64_t splitmix64() {
	uint64_t z = (splitmix64_state += 0x9E3779B97F4A7C15ULL);
	z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
	return z ^ (z >> 31);
}

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static uint64_t s[4];

static uint64_t xoshiro256starstar_next() {
	const uint64_t result = rotl(s[1] * 5, 7) * 9;
	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];
	s[2] ^= t;
	s[3] = rotl(s[3], 45);

	return result;
}

int main(int argc, char **argv) {
	freopen(NULL, "wb", stdout);  // Only necessary on Windows, but harmless.

	uint64_t seed;
	if (argc >= 2) {
		seed = strtoull(argv[1], NULL, 16);
	}
	else {
		std::random_device rd;
		seed = (uint64_t(rd()) << 32) | rd();
	}
	std::fprintf(stderr, "xoshiro256starstar(0x%016llx) initialized.\n", (unsigned long long)seed);

	splitmix64_state = seed;
	for (int i = 0; i < 4; ++i)
		s[i] = splitmix64();

	constexpr size_t BUFFER_SIZE = 1024 * 1024 / sizeof(uint64_t);
	static uint64_t buffer[BUFFER_SIZE];

	while (1) {
		for (size_t i = 0; i < BUFFER_SIZE; ++i)
			buffer[i] = xoshiro256starstar_next();
		fwrite((void *)buffer, sizeof(buffer[0]), BUFFER_SIZE, stdout);
	}
}
