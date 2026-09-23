# Practically Random (PractRand)

Statistical tests & pseudo-random number generators (RNGs, PRNGs)

Version **0.96** *(beta)* | Official [Homepage](https://pracrand.sourceforge.net/)

> **PractRand** is a random number generation & testing library. It's the most
> thorough suite of statistical tests for fast PRNGs anywhere: multithreaded
> for speed, command-line tools for automation, and no upper limit on how much
> data it can chew through. It also ships a variety of C++ pseudo-random
> number generators with well-designed interfaces aimed at practical use, not
> just research.
>
> > *From [SourceForge description](https://sourceforge.net/projects/pracrand/)*

### Features

- A convenient & powerful interface to RNG algorithms
- A variety of fast, high-quality built-in RNG algorithms
- Fast & effective statistical tests for RNGs, from a quick 128 MB smoke
  test up to tens of terabytes
- A command-line tool (`RNG_test`) that can test any external RNG as long
  as it can pipe raw bytes to stdout

## Fork Notes

This is a copy of PractRand 0.96, a statistical-testing engine for RNG (hardware TRNGs, PCG/xoshiro/ChaCha-family
PRNGs, etc.). A few things to know if you're expecting an upstream checkout:

- **`src/`, `include/`, and `tools/` are unmodified** from the upstream 0.96
  release. No PractRand source was patched to produce the examples below
- **`examples/` is new** — it isn't part of upstream PractRand. It contains
  small, self-contained C++ programs for three modern PRNGs that PractRand
  does *not* ship built in (PCG32, xoshiro256\*\*), plus a standalone
  ChaCha20 implementation for comparison against PractRand's own bundled
  ChaCha. See [Testing external generators](#testing-external-generators)
  below.
- **`Makefile` and `tests/` are also new.** Upstream PractRand ships no
  build system at all (see `doc/installation.md`) — you're expected to
  invoke `g++`/`ar` by hand. This fork adds a `Makefile` that wraps those
  same commands, plus `tests/run_tests.sh`, a basic smoke-test script. See
  [Building with `make`](#building-with-make) and
  [Running the tests](#running-the-tests) below.
- **License is unchanged**: everything under `src/`, `include/`, `tools/`,
  and `doc/` is CC0 / public domain per `doc/license.md`. The new files
  under `examples/`, `tests/`, and the `Makefile` are released under the
  same terms.

## Usage Guide

This section is inspired by an older
[guide](https://www.pcg-random.org/posts/how-to-test-with-practrand.html)
written against PractRand 0.93, but has been rewritten and verified
against **v0.96**.

### Prerequisites

You need a C++14-capable compiler (`g++` 5+ or `clang++` 3.4+), `ar`
(from binutils, usually already present), and `pthread` support. Nothing
else — PractRand has no external library dependencies.

**On Fedora 44:**

```bash
sudo dnf install gcc-c++ make git
```

**On Ubuntu 26.04:**

```bash
sudo apt install build-essential git
```

### Building

Build the PractRand static library, then link the command-line tools
against it. These commands were verified against this checkout with
GCC 16.2.1 — they produce warnings from PractRand's own code (integer
overflow/shift edge cases in rarely-exercised generators) but no errors.

```bash
g++ -std=c++14 -c src/*.cpp src/RNGs/*.cpp src/RNGs/other/*.cpp -O3 -Iinclude -pthread
ar rcs libPractRand.a *.o
rm *.o
g++ -std=c++14 -o RNG_test tools/RNG_test.cpp libPractRand.a -O3 -Iinclude -pthread
g++ -std=c++14 -o RNG_output tools/RNG_output.cpp libPractRand.a -O3 -Iinclude -pthread
g++ -std=c++14 -o RNG_benchmark tools/RNG_benchmark.cpp libPractRand.a -O3 -Iinclude -pthread
```

Confirm it built correctly and check the version:

```
$ ./RNG_test --version
RNG_test using PractRand version 0.96
```

`RNG_test` is the tool used throughout this guide. `RNG_output` dumps raw
bytes from a built-in RNG to stdout (handy for piping to other tools).
`RNG_benchmark` measures the speed of all built-in RNGs on your machine.

### Building with `make`

The commands above are also wrapped in a `Makefile` (new in this fork —
see [Fork Notes](#fork-notes)), which additionally builds the three example
generators in `examples/`:

```
$ make
g++ -std=c++14 -O3 -Wall -Iinclude -MMD -MP -c src/math.cpp -o build/obj/math.o
g++ -std=c++14 -O3 -Wall -Iinclude -MMD -MP -c src/non_uniform.cpp -o build/obj/non_uniform.o
...
g++ -std=c++14 -O3 -Wall -Iinclude -MMD -MP -c src/RNGs/other/transform.cpp -o build/obj/RNGs/other/transform.o
ar rcs libPractRand.a build/obj/math.o build/obj/non_uniform.o ...
g++ -std=c++14 -O3 -Wall -Iinclude -o RNG_test tools/RNG_test.cpp libPractRand.a -pthread
g++ -std=c++14 -O3 -Wall -Iinclude -o RNG_output tools/RNG_output.cpp libPractRand.a -pthread
g++ -std=c++14 -O3 -Wall -Iinclude -o RNG_benchmark tools/RNG_benchmark.cpp libPractRand.a -pthread
g++ -std=c++14 -O3 -Wall -o examples/chacha20 examples/chacha20.cpp
g++ -std=c++14 -O3 -Wall -o examples/pcg32 examples/pcg32.cpp
g++ -std=c++14 -O3 -Wall -o examples/xoshiro256starstar examples/xoshiro256starstar.cpp
```

(the `...`s above stand in for one more `-c` line per remaining PractRand
source file, and the rest of the `ar` command line — trimmed here for length)

Object files go in `build/obj/` (with header dependency tracking, so a
second `make` with nothing changed does nothing); the library and tools
land at the top level, matching the manual commands above and the paths
used throughout this guide. Other targets:

| Target | Effect |
| --- | --- |
| `make lib` | build `libPractRand.a` only |
| `make tools` | build `RNG_test`, `RNG_output`, `RNG_benchmark` |
| `make examples` | build the three example generators |
| `make test` | build everything, then run `tests/run_tests.sh` (see below) |
| `make clean` | remove all build output |
| `make help` | print this list |

`make CXX=clang++` or `make CXXFLAGS="-O2 -g"` override the compiler or
flags for a one-off build.

### Testing a built-in RNG directly

Before testing external generators, confirm the tool works against a
generator PractRand ships itself, e.g. `sfc64`:

```
$ ./RNG_test sfc64 -tlmax 512M
RNG_test using PractRand version 0.96
RNG = sfc64, seed = 0x...
test set = core, folding = standard (64 bit)

rng=sfc64, seed=0x...
length= 512 megabytes (2^29 bytes), time= ... seconds
  no anomalies in ... test result(s)
```

`-tlmax 512M` caps the test length at 512 megabytes so it doesn't run
forever; without it, `RNG_test` defaults to a maximum of 32 terabytes and
keeps going until it finds a failure or you stop it. See `--list_recommended_rngs`
and `--list_reference_rngs` for the full list of names built in to this copy
of PractRand.

### Testing external generators

Most interesting PRNGs — like PCG and xoshiro256\*\* — are not built in to
PractRand. The standard way to test them is the same trick this guide's
predecessor used for the Mersenne Twister: write a small program that spews
raw random bytes to stdout, and pipe it into `RNG_test`.

The essential pattern, buffered for speed (writing one value at a time to
stdout is easy but slow):

```c++
#include <cstdio>
#include <cstdint>

int main() {
    freopen(NULL, "wb", stdout);  // only necessary on Windows, but harmless

    /* ... construct and seed your RNG here ... */

    constexpr size_t BUFFER_SIZE = 1024 * 1024 / sizeof(uint32_t);
    static uint32_t buffer[BUFFER_SIZE];
    while (1) {
        for (size_t i = 0; i < BUFFER_SIZE; ++i)
            buffer[i] = /* next output from your RNG */;
        fwrite((void*) buffer, sizeof(buffer[0]), BUFFER_SIZE, stdout);
    }
}
```

Two things worth doing every time you wire up a new generator this way:

1. **Sanity-check the raw bytes with a hex dump before trusting any test
   result.** It's the fastest way to catch a face-palm bug, like writing a
   64-bit value where a 32-bit one was declared (you'll see zero-runs every
   other word) or forgetting to seed the generator at all (you'll see an
   obviously repeating pattern).
2. **Print the seed to stderr** so a run can be reproduced later if
   something interesting (or wrong) turns up.

Full, working examples for the three generators below are in
[`examples/`](examples/): [`pcg32.cpp`](examples/pcg32.cpp),
[`xoshiro256starstar.cpp`](examples/xoshiro256starstar.cpp), and
[`chacha20.cpp`](examples/chacha20.cpp).

#### PCG32

[PCG32](https://www.pcg-random.org/) is O'Neill's "XSH RR" 64-state,
32-output variant — a 64-bit LCG whose output is permuted with a
xorshift and a state-dependent rotation.

```bash
g++ -std=c++14 -O3 -Wall -o pcg32 examples/pcg32.cpp
```

A hex dump check first:

```
$ ./pcg32 12345 6789 2>/dev/null | head -c 64 | hexdump -C
00000000  12 62 cc c3 4d ca 90 75  2b ae 2d f3 be fe c6 fc  |.b..M..u+.-.....|
00000010  25 18 55 77 48 d1 dc 59  7b 00 ab b1 af ae 73 a9  |%.UwH..Y{.....s.|
00000020  a7 c6 c1 7e bb 3d 73 ba  0a f6 e3 51 af 7a 94 be  |...~.=s....Q.z..|
```

No obvious patterns, no zero-runs. Now test it — since PCG32 emits full
32-bit words, use `stdin32`:

```
$ ./pcg32 12345 6789 | ./RNG_test stdin32 -tlmax 512M
pcg32(0x0000000000012345, 0x0000000000006789) initialized.
RNG_test using PractRand version 0.96
RNG = RNG_stdin32, seed = unknown
test set = core, folding = standard (32 bit)

rng=RNG_stdin32, seed=unknown
length= 512 megabytes (2^29 bytes), time= 3.9 seconds
  Test Name                         Raw       Processed     Evaluation
  [Low1/32]Gap-16:A                 R=  -4.3  p =1-3.9e-4   unusual
  ...and 166 test result(s) without anomalies
```

That single "unusual" is well within the expected false-positive rate (see
[Interpreting results](#interpreting-results) below) — not a red flag on
its own. Publicly reported PractRand runs against PCG32 into the
multi-terabyte range have found nothing worse than occasional "unusual"
results of this kind, consistent with what you'd expect from a
statistically strong 64-bit-state generator.

#### xoshiro256\*\*

[xoshiro256\*\*](https://prng.di.unimi.it/) (Blackman & Vigna, 2018) is a
256-bit-state generator recommended by its authors as a good all-purpose,
non-cryptographic default. Its 256-bit state is seeded from a single 64-bit
value via splitmix64, as the authors recommend.

```bash
g++ -std=c++14 -O3 -Wall -o xoshiro256starstar examples/xoshiro256starstar.cpp
./xoshiro256starstar abcdef 2>/dev/null | head -c 64 | hexdump -C
```

It emits full 64-bit words, so use `stdin64`:

```
$ ./xoshiro256starstar abcdef | ./RNG_test stdin64 -tlmax 512M
xoshiro256starstar(0x0000000000abcdef) initialized.
RNG_test using PractRand version 0.96
RNG = RNG_stdin64, seed = unknown
test set = core, folding = standard (64 bit)

rng=RNG_stdin64, seed=unknown
length= 512 megabytes (2^29 bytes), time= 3.8 seconds
  no anomalies in 213 test result(s)
```

Clean, as expected — xoshiro256\*\* has likewise been reported clean by its
authors and others in PractRand runs well into the terabyte range.

#### ChaCha20

ChaCha20 is a cryptographic stream cipher (Bernstein, 2008; standardized in
[RFC 8439](https://www.rfc-editor.org/rfc/rfc8439)). Because it's a CSPRNG,
it isn't expected to show statistical bias within any test length that's
feasible to run — that's the whole point of using it as a keystream
generator. `examples/chacha20.cpp` implements the unmodified 20-round
ChaCha core, but deviates from RFC 8439's state layout in one deliberate
way: instead of a 96-bit nonce plus a 32-bit block counter, it treats all
four of the state's non-key words (128 bits total) as one counter. RFC
8439's 32-bit counter wraps — and the keystream starts repeating — after
only 2^32 blocks (256 GB), which is comfortably inside PractRand's default
32 TB test ceiling. PractRand's own bundled ChaCha offers the same
"extend_cycle" option for the same reason (see `src/RNGs/chacha.cpp`).

```bash
g++ -std=c++14 -O3 -Wall -o chacha20 examples/chacha20.cpp
./chacha20 deadbeef 2>/dev/null | head -c 64 | hexdump -C
```

```
$ ./chacha20 deadbeef | ./RNG_test stdin32 -tlmax 512M
chacha20(0x4adfb90f68c9eb9bde586a3141a10922021fbc2f8e1cfc1d7466ce737be16790) initialized.
RNG_test using PractRand version 0.96
RNG = RNG_stdin32, seed = unknown
test set = core, folding = standard (32 bit)

rng=RNG_stdin32, seed=unknown
length= 256 megabytes (2^28 bytes), time= 2.3 seconds
  no anomalies in 156 test result(s)

rng=RNG_stdin32, seed=unknown
length= 512 megabytes (2^29 bytes), time= 4.8 seconds
  no anomalies in 167 test result(s)
```

For comparison, PractRand also ships ChaCha built in, so you can test an
equivalent 20-round ChaCha without writing or compiling anything:

```
$ ./RNG_test "chacha(20)" -tlmax 256M
RNG_test using PractRand version 0.96
RNG = chacha(20), seed = 0x...
test set = core, folding = standard (32 bit)

rng=chacha(20), seed=0x...
length= 256 megabytes (2^28 bytes), time= 2.2 seconds
  no anomalies in 156 test result(s)
```

The built-in version defaults to 12 rounds if you omit the `(20)` — pass a
round count explicitly (from 1 to 255) to match the standard. `chacha(8)`
and `chacha(12)` are also common reduced-round settings used to evaluate
how much security margin the cipher has to spare.

### Running the tests

`tests/run_tests.sh` (new in this fork) is a basic smoke test, **not** a
statistical certification of any generator — it exists to catch build
breakage, a mis-wired generator (wrong output width, forgotten seed, a
typo'd `stdin` mode), or a broken pipe, in a few seconds. Real statistical
testing means running `RNG_test` at realistic lengths yourself, as above.

It checks, in order: `RNG_test`'s own `--self_test` (validates PractRand's
bundled RNGs against known test vectors), two built-in RNGs (`sfc64` and
`chacha(20)`), and each of the three `examples/` generators — first a
hexdump-style sanity check (catches an all-zero or constant-byte stream),
then a real `RNG_test` run over the matching `stdin*` width. It fails loudly
if the word `FAIL` turns up anywhere.

```
$ make test
...
== RNG_test self-test ==
  PASS: RNG_test --self_test
== built-in RNGs ==
  PASS: sfc64 (built in)
  PASS: chacha(20) (built in)
== example generators (examples/) ==
  PASS: pcg32 (via stdin32)
  PASS: xoshiro256starstar (via stdin64)
  PASS: chacha20 (via stdin32)

All checks passed.
```

`make test` builds everything first, then runs the script. You can also run
it directly against an already-built tree, optionally passing a test length
(anything `-tlmin`/`-tlmax` understand — default `32M`):

```bash
./tests/run_tests.sh          # default: 32M per check
./tests/run_tests.sh 256M     # slower, marginally more thorough
```

### Interpreting results

Look for the word `FAIL` — it's flagged clearly on the right of the output
table. `RNG_test` only prints anomalous rows by default; a line like
`...and 166 test result(s) without anomalies` means everything else passed
quietly. The full evaluation scale, from least to most suspicious, is:
`normal`, `normalish`, `unusual`, `mildly suspicious`, `suspicious`,
`very suspicious`, `VERY SUSPICIOUS`, `FAIL`.

These thresholds are calibrated so that, per results summary, `FAIL` has a
false-positive rate around 3-in-a-billion and `unusual` has a
false-positive rate around 1-in-10. That means seeing an occasional
`unusual` — especially in isolation, not clustered with other anomalies —
is expected and not meaningful on its own. See `doc/Tests_overview.md`
(section 1.B) for the full statistical reasoning.

### Useful `RNG_test` options

A few flags worth knowing beyond what's used above (`./RNG_test --help`
has the complete list):

| Flag | Purpose |
| --- | --- |
| `-tlmin LENGTH` / `-tlmax LENGTH` | Set the minimum/maximum test length. `LENGTH` accepts a plain number (log2 of bytes), `14KB`/`40M`/`1GB`/etc., or a time like `1.4s`/`2m`. Default max is 32 TB. |
| `-tlfail` / `-tlmaxonly` | Stop as soon as a failure is found (default), or keep going regardless. |
| `-a` | Show every test result, not just anomalous ones. |
| `-e EXPECTED` | Tune how many results get shown per summary (default: about 0.1 "interesting" results per summary). |
| `-seed SEED` | Reproduce a specific run with a 128-bit hex seed (for RNGs PractRand seeds itself — external RNGs via `stdin*` seed themselves). |
| `-multithreaded` | Run the RNG and each test concurrently; useful on multi-core machines for large runs. |
| `stdin`, `stdin8`, `stdin16`, `stdin32`, `stdin64` | Read raw bytes from stdin, interpreted in chunks of the given width. Use whichever matches your generator's natural output width, or plain `stdin` if unsure. |

Version 0.96 also added composite RNG names, so you can combine generators
directly on the command line without writing any code, e.g.:

```
./RNG_test "xor(jsf32,AddToSeed(jsf32,1))"
```

That constructs two `jsf32` instances (one seeded randomly, the other with
a seed one higher), XORs their output together, and feeds the result to the
tests.

### Conclusion

Using a random-number test suite isn't hard. Pick a generator, get its raw
bytes into `RNG_test` (built in, or piped through `stdin*`), sanity-check
with a hex dump, and read the evaluation column. You can do it, too!
