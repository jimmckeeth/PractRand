// Standalone ChaCha20 (RFC 8439 core, 20 rounds) keystream generator that
// streams raw bytes to stdout for testing with PractRand's RNG_test.
//
// Layout differs from RFC 8439 in one deliberate way: instead of a 96-bit
// nonce plus a 32-bit block counter, all four of the state's non-key,
// non-constant words (128 bits total) are treated as one little-endian
// counter that increments once per 64-byte block. That avoids the 32-bit
// counter wrapping (and the keystream repeating) after only 2^32 blocks
// (256 GB) -- easily reachable at PractRand's default 32 TB test ceiling.
// PractRand's own bundled chacha RNG offers the same "extend_cycle" option
// for the same reason (see src/RNGs/chacha.cpp).  The cipher core itself
// (20 rounds = 10 double-rounds) is unmodified ChaCha20.
//
// Build:
//   g++ -std=c++14 -O3 -Wall -o chacha20 examples/chacha20.cpp
// Run:
//   ./chacha20 | ./RNG_test stdin32
// Reproduce a specific run (key is printed to stderr each run):
//   ./chacha20 <seed-hex> | ./RNG_test stdin32
//
// For comparison, PractRand also has ChaCha built in, so
// "./RNG_test chacha(20)" tests an equivalent 20-round ChaCha without
// writing any code at all -- see the readme section "Testing a built-in RNG
// directly".

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <random>

static inline uint32_t rotl32(uint32_t x, int n) {
	return (x << n) | (x >> (32 - n));
}

static inline void quarter_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d) {
	a += b; d ^= a; d = rotl32(d, 16);
	c += d; b ^= c; b = rotl32(b, 12);
	a += b; d ^= a; d = rotl32(d, 8);
	c += d; b ^= c; b = rotl32(b, 7);
}

// state[0..3]   = constants ("expand 32-byte k")
// state[4..11]  = 256-bit key
// state[12..15] = 128-bit little-endian block counter (see note above)
static void chacha20_block(const uint32_t in[16], uint32_t out[16]) {
	uint32_t x[16];
	memcpy(x, in, sizeof(x));
	for (int i = 0; i < 10; ++i) {
		quarter_round(x[0], x[4], x[8],  x[12]);
		quarter_round(x[1], x[5], x[9],  x[13]);
		quarter_round(x[2], x[6], x[10], x[14]);
		quarter_round(x[3], x[7], x[11], x[15]);
		quarter_round(x[0], x[5], x[10], x[15]);
		quarter_round(x[1], x[6], x[11], x[12]);
		quarter_round(x[2], x[7], x[8],  x[13]);
		quarter_round(x[3], x[4], x[9],  x[14]);
	}
	for (int i = 0; i < 16; ++i)
		out[i] = x[i] + in[i];
}

static inline void counter_increment(uint32_t ctr[4]) {
	for (int i = 0; i < 4; ++i)
		if (++ctr[i] != 0) break;
}

int main(int argc, char **argv) {
	freopen(NULL, "wb", stdout);  // Only necessary on Windows, but harmless.

	uint32_t state[16] = {
		0x61707865, 0x3320646e, 0x79622d32, 0x6b206574, // "expand 32-byte k"
		0, 0, 0, 0,  0, 0, 0, 0,                         // key, filled in below
		0, 0, 0, 0                                       // 128-bit counter, starts at 0
	};

	uint64_t key_words[4];
	if (argc >= 2) {
		// deterministic run: expand a single hex seed into the 256-bit key with splitmix64
		uint64_t sm = strtoull(argv[1], NULL, 16);
		for (int i = 0; i < 4; ++i) {
			sm += 0x9E3779B97F4A7C15ULL;
			uint64_t z = sm;
			z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
			z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
			key_words[i] = z ^ (z >> 31);
		}
	}
	else {
		std::random_device rd;
		for (int i = 0; i < 4; ++i)
			key_words[i] = (uint64_t(rd()) << 32) | rd();
	}
	memcpy(&state[4], key_words, sizeof(key_words));
	std::fprintf(stderr, "chacha20(0x%016llx%016llx%016llx%016llx) initialized.\n",
		(unsigned long long)key_words[0], (unsigned long long)key_words[1],
		(unsigned long long)key_words[2], (unsigned long long)key_words[3]);

	constexpr size_t BLOCKS_PER_BUFFER = 1024 * 1024 / 64;
	static uint32_t buffer[BLOCKS_PER_BUFFER * 16];

	while (1) {
		for (size_t b = 0; b < BLOCKS_PER_BUFFER; ++b) {
			chacha20_block(state, &buffer[b * 16]);
			counter_increment(&state[12]);
		}
		fwrite((void *)buffer, sizeof(buffer[0]), BLOCKS_PER_BUFFER * 16, stdout);
	}
}
