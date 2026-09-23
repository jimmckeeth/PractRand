# PractRand Version History

## Version 0.96

### General
- Ran all code through Valgrind and other diagnostic tools, fixing potential issues.
- Tweaked test battery structures to slightly improve multithreading performance.
  - *Note*: Multithreading still needs a rewrite to eliminate continuous creation and destruction of worker threads.
- Began development of automated testing infrastructure:
  - Developing scripts to query PractRand for comprehensive lists of flawed PRNGs.
  - Automated cross-testing across PractRand, TestU01, gjrand, and Dieharder.
  - Queryable results to look up PRNGs matching specific failure/pass profiles.
  - Automated determination of optimal test batteries to detect PRNG bias as rapidly as possible.
  - Automated evaluation of new tests to determine outdated tests, competitor tests, and vulnerability profiles.
- Added command-line support for complex PRNG compositions, such as:
  ```bash
  ./RNG_test "xor(jsf32,AddToSeed(jsf32,1))"
  ```
  *(Creates two instances of `jsf32`, seeds one randomly and the other with seed + 1, XORs their outputs, and evaluates the result.)*

### Distributions
- **Gaussian Distribution**: Rewritten.
  - Currently uses popcounts to index distribution regions, followed by quadratic fits within each region (may update to cubic fits or a new algorithm later).
  - Code for alternative distribution algorithms retained in `non_uniform.cpp`.
- **Uniform Integer Distributions**: Renamed; underlying implementations unchanged.
- **Uniform Floating-Point Distributions**: Renamed; underlying implementations largely unchanged.

### RNGs
- **mrc64 / mrc32 / mrc16**: Very fast multiplication-based generators.
- **mrsf64 / mrsf32**: Faster multiplication-based generators with state fitting into two registers; more prone to short cycles than `mrc`.
- **rarns\*** and **xsm\***: Removed again following analysis (output hashing insufficiently masked underlying flaws).
- Incorporated general XOR-based generator fast-forwarding code into PractRand.

### Tests
- **DC6**: Calibration improved; resolved excessively paranoid p-values at specific test lengths.
- **FPF**: Test logic unchanged, but output formatting for test names and parameters adjusted for brevity.
- **BDayS2**: New core test battery member. Detects bias in linear PRNGs that bypassed other PractRand tests with low CPU overhead.
- **LPerm16**: Added to expanded test set. Automatically suppresses reports if test length exceeds ~512 TB to avoid false positives.
- **APerm16**: Added to expanded test set; subject to the same long-run limitations as `LPerm16`.
- Various other unlisted minor improvements.

---

## Version 0.95

### General
- Adjusted code to eliminate build warnings under GCC and Clang.

### Tests
- **NearS2**: Added to expanded test set. Detects bias across various PRNGs (medium-short ranged correlations; 384-bit footprint, 64-bit stride). Limited effectiveness on states > 256 bits; reasonably fast.
- **NearS3**: Added to expanded test set. Slightly lower detection rate than `NearS2`, but cleaner and detects distinct biases.
- **BRank**: Revised to evaluate a broader variety of matrix dimensions.
- **FPMulti**: Rewritten; currently only `FPG` is active and functional in the expanded test set.
- **DC7**: Added to expanded test set on trial.
- **BirthdaySystematic64**: Added.

### RNGs
- **xsm64 / xsm32 / xsm16**: Revised.
- **rarns64 / rarns32 / rarns16**: 3-word XOR-based generators with output hashing to resist `BRank` (16-bit variant eventually fails; 32-bit and 64-bit pass).

### Tools
- Added `--version` command-line option.
- **RNG_to_TestU01**: Added Unix utility to test PractRand RNG implementations against TestU01 batteries (`SmallCrush`, `Crush`, `BigCrush`).

---

## Version 0.94

### RNGs
- **xsm64**: Revised after the previous implementation fell short of target quality on specific seeds.
- **xsm32**: Revised.
- **efiix8x48 / efiix16x48 / efiix32x48 / efiix64x48**: Reduced iteration table size from 128 to 32 and indirection table size from 256 to 16 to minimize memory and cache footprints (renamed from `efiix*x384`).

### Tools
- **RNG_test**:
  - Adjusted raw output formatting to preserve column alignment during extreme test failures.
  - Updated seed display for RNGs lacking meaningful seeds.
  - Fixed an off-by-one bug causing slight evaluation threshold inaccuracies.
  - Added the `normalish` evaluation threshold between `normal` and `unusual`.
  - Added support for `SeedingTester` and `EntropyPoolingTester` meta-RNGs directly via RNG naming syntax (previously only `-ttseed64` and `-ttep`).
- **RNG_output**:
  - Increased buffer size and fixed endianness handling.
  - Added support for `SeedingTester` and `EntropyPoolingTester` meta-RNGs.
  - Improved column alignment formatting for large raw values.

### Test Batteries
- **Core Tests**:
  - Added **mod3n**: Highly effective on specific PRNGs (e.g., `flea32x1`, `ara32`, `ibaa32`) with low overhead by testing fractional streams; inspired by gjrand's `mod3`.
  - Added **TMFn**: Targets LCGs with low resource requirements.
  - **FPF**: Removed the `all2` subtest due to unreliable p-values under extreme results.
- **Expanded Tests**:
  - Added `mod3n`, `TMFn`, and `BirthdaySystematic`.

### Tests
- **DistC6**: Fixed a parameterization bug in the expanded test set that caused crashes on some platforms.
- **BRank**: Minor bug fixes.
- **Transforms**: Fixed bugs in `FirstNofM`.
- **mod3n**: Targets 8, 16, 32, 64, and 128-bit blocks simultaneously (similar to `BCFN`).
- **TMFn**: Specialized test targeting linear congruential generators.
- Added pre-configured RNG test sets to assist test automation.

---

## Version 0.93

### RNGs
- **All Recommended RNGs**:
  - Added customized seeding-from-another-RNG implementations.
  - State spaces $\le 2^{128}$: Allows seeding across all possible states.
  - State spaces $\ge 2^{256}$: Restricts seeding to approximately $\sqrt{\text{statespace}}$ initial states.
  - Enables probabilistic proofs of zero bad cycles where practical.
- **sfc16 / sfc32 / sfc64**: Improved shift constants (modifies output sequence) and adjusted seeding.
- **arbee**: Tweaked feed-in algorithms.
- **efiix\***: Updated seeding algorithms.
- **Non-Recommended RNGs**:
  - Added `xoroshiro128plus`.
  - Fixed algorithm implementation in `efiix` non-recommended variants.

### Tools
- **RNG_test**:
  - Refined evaluation thresholds.
  - Fixed multithreading issues (notably on Unix).
  - Fixed bug in `--ttep` mode.
  - Increased standard input buffer size.
- **RNG_output**:
  - Increased buffer size.
  - Added support for `inf` byte counts.
  - Refined warnings, usage documentation, and error messages.

### Tests
- **BRank**: Minor fixes and output adjustments; 32-bit binaries enforce smaller maximum matrix dimensions to prevent address space exhaustion.
- **BCFN_FF**: Adopted more conservative p-values for `:freq` subtests to reduce false suspicious results.
- **FPF**: Fixed `:all` result handling on short test runs.

---

## Version 0.92

### RNGs
- **Non-Recommended RNGs**: Added two weakened variants of Trivium for test evaluation.

### Tools
- **RNG_test**:
  - Added buffered stdin reading, boosting throughput during multithreaded testing of external PRNGs.
  - Switched from CPU time to C++11 system time measurement (can be toggled via `#define CPP_2011_SUPPORTED`).

### Tests
- Added **Pat5** (short-range linear test).
- **BRank**: Fixed minor abstraction layer violation.
- **BCFN / BCFN_FF / BCFN_MT**: Improved calculation precision on long test runs.

### Documentation
- Added summary of NIST STS results to [Tests_results](Tests_results.md).

---

## Version 0.91

### RNGs
- **efiix**: Minor seeding revisions.
- **xsm32 / xsm64**: Updated algorithm and seeding; improved quality for parallel generation at the cost of execution speed.
- **Non-Recommended RNGs**:
  - Fixed bugs in `dual_cbuf`, `dual_cbuf_accum`, and `isaac32_small`.
  - Added `isaac16_small` and `efiix4_small`.

### Test Batteries
- **Core Tests**: Added an `FPF` parameterization and incorporated `BRank`.
- **Expanded Tests**: Updated test configurations.

### Tests
- **New Tests**:
  - **BRank**: Binary matrix rank test.
  - **BCFN_MT**: Multithreaded BCFN (initial addition).
  - **BCFN_FF**: Full-frequency variant.
  - **FPMulti**: Multi-stream floating point test (initial addition).
- **FPF**: Significant optimizations, scoring improvements, subtest restructuring, and cleanup.
- **BCFN**: Major optimizations (made default in tools) and removed the `!` subtest.
- **Gap-16**: Updated calibration data for accurate extreme p-values.
- **DC6**: Performance improvements.

### Tools
- **RNG_test**: Defaults to `-tlfail` (terminate testing immediately upon test failure); fixed negative elapsed time bug.

### Documentation
- Substantial improvements across documentation files.

---

## Version 0.90

### General & Core
- Fixed an accidental debug flag from versions 0.88/0.89 that broke autoseeding.
- Fixed header file case-sensitivity issues and improved test suite checks.
- Added reinterpret RNG transforms (allows piping and reinterpreting outputs at different word widths).

### RNGs
- **efiix**: Minor seeding adjustment.
- Added additional seeding options across various RNG implementations.

### Tools
- Fixed bugs in `RNG_output` and non-recommended RNG implementations.

---

## Version 0.89

### RNGs
- **chacha**: Disabled SIMD path due to dynamic allocation alignment issues.
- **salsa**: Added Salsa20 implementation.

### Tools
- **RNG_test**:
  - Added `-tlfail` (stop testing when a failure occurs).
  - Added `-tlmaxonly` (continue testing after failures; default behavior).

### General
- Fixed header portability regression introduced in version 0.88.
- Documentation updates.

---

## Version 0.88

### Important Bugfix
- Fixed a critical regression in `sha2_based_pool` introduced in 0.87, which affected internal autoseeding.

---

## Version 0.87

### Highlights
- Improved p-value accuracy across test batteries.
- Redesigned RNG test reporting format.
- Overhauled autoseeding mechanism.
- Enhanced tool suite capabilities.

### RNG Algorithms
- **chacha**: Added single 32-bit cryptographic RNG featuring variable quality settings, random access, and fast seeding.
- **jsf16 / jsf32 / jsf64**:
  - Tweaked `jsf32` initialization.
  - Removed `jsf16` (superseded by `sfc16`).
  - Added fast-seeding option.
- **sfc16 / sfc32 / sfc64**:
  - Fixed seeding-from-RNG bug in `sfc16`.
  - Upgraded to SFC version 4 (legacy version moved to non-recommended).
  - Added fast-seeding option.
- **xsm32 / xsm64**:
  - Fixed integer seeding and random-access bugs in `xsm64`.
  - Adjusted engine ratings down half a star (3.0 $\to$ 2.5 on `xsm32`; 4.0 $\to$ 3.5 on `xsm64`).
- **mt19937**: Added support for seeding from arbitrary-length sequences of 32-bit integers; updated integer seeding for values $\ge 2^{32}$.
- **arbee**: Seeding adjustments, optimized `add_entropy_N`, fixed metadata, and refined interfaces.
- **Non-Recommended RNGs**: Bug fixes and algorithm additions.

### Testing & Calibration
- **Reporting**: Significant changes to test result reporting; resolved edge cases causing rare false positives.
- **P-Value Quality**: Dramatically improved calibration and p-value precision for `BCFN`, `Gap16`, `DC6`, `FPF`, and `CoupGap`.
  - `BCFN` and `Gap16` now report separate subtest p-values instead of single merged values.
- **Self-Tests**:
  - Added test-vector validation for `mt19937`, `chacha`, `hc256`, `trivium`, and `isaac32x256`.
  - Validates fast-forward and rewind capabilities for `xsm32`, `xsm64`, and `chacha`.

### Infrastructure & Autoseeding
- **Autoseeding**:
  - Fixed race conditions during rapid thread creation/destruction.
  - Implemented tiered strategy:
    1. TLS + atomic increments on recognized platforms.
    2. TLS + thread-local allocation on unrecognized platforms.
    3. Fallback to `std::clock()` where TLS is unsupported.
- **Error Handling**: Uses a configurable user callback if provided; defaults to stderr output with `exit(1)`.
- **Initialization**: `initialize_PractRand()` now returns status indicating whether sufficient entropy was collected.

### Tools & Documentation
- Renamed "test programs" directory/concept to **tools**.
- **RNG_test**: Overhauled with command-line flag parser (`--help`); added testing modes for seeding quality and entropy pooling quality.
- Merged standalone multithreaded test program into `RNG_test`.
- **RNG_output**: Fixed unseeded RNG bug; updated nested RNG syntax (e.g., `SShrink(jsf32)`).
- **Test_calibration**: Added internal calibration tool.
- Added documentation for parallel random number generation.

---

## Version 0.86

### RNG Algorithms
- **jsf16**: Corrected shift constants to restore intended avalanche properties.
- **efiix8x384 / efiix16x384 / efiix32x384 / efiix64x384**: Optimized seeding algorithm for speed.
- **trivium**: Bug fixes and improved validation routines.

### Tools
- Added initial multithreaded RNG testing program utilizing worker threads across core tests and folds.

---

## Version 0.85

### Highlights
- Major enhancements to RNG interfaces, portability, and seekable PRNG algorithms.

### Portability & Build
- Resolved compilation issues on modern GCC releases (tested on GCC 4.6.1).
- Removed requirement for manual `config.h` edits across standard compilers.

### RNG Interfaces
- **Lightweight RNGs**:
  - Namespace moved to `PractRand::RNGs::LightWeight`.
  - Now available across all RNGs via default template wrappers.
  - Optional disabling via `#define PRACTRAND_NO_LIGHT_WEIGHT_RNGS`.
- **Polymorphic RNGs**: Added missing virtual destructor; added serialization and deserialization methods.
- **Entropy Pools**: Reorganized into `PractRand::RNGs::Polymorphic` and `PractRand::RNGs::Raw`.
- **Random Access (Seeking)**: Added support for 128-bit seek offsets (specified as two 64-bit integers).
- **Boost / C++0x TR1 Compatibility**: Guarded behind `#define PRACTRAND_BOOST_COMPATIBILITY`; enabled on base `vRNG`.

### RNG Algorithms
- **Trivium**: Added non-indirection cryptographic generator (initial implementation, disabled pending validation).
- **xsm64**: Added 64-bit seekable generator to replace `clcg96_32` (full 64-bit output, parallel-safe).
- **xsm32**: Added seekable generator to replace `lcg64_32` (superior quality and state space).
- **sha2_based_pool**: Fixed potential crash bug; namespace updated.
- **arbee**: Improved input mixing algorithm (`flush_buffers()` now functional); namespace updated.
- **sfc32 / sfc64**: Seeding adjusted to prevent nearby cycle overlaps.
- **isaac_with_sha2_pool**: Removed (recommended pattern: use `sha2_based_pool` to seed faster PRNGs).
- **efiix\***: Renamed from `efiix*x256` to `efiix*x384`; enhanced security characteristics.
- **isaac32x256 / isaac64x256**: Optimized implementation, conformed seeding to reference standard, and added test vector validation.
- **hc256**: Performance optimizations and validation test vectors.
- **mt19937**: Validation test vectors added.
- **Removed RNGs**: `mwlac64`, `mwlac32`, `mwlac16`, `lcg64_32`, and `clcg96_32`.

### Tools & Tests
- Split monolithic example into:
  1. `RNG_benchmark`: Performance and throughput benchmarking.
  2. `RNG_output`: Raw binary output streaming to stdout.
  3. Statistical testing harness.
- Added multithreaded RNG testing utility (initially Win32).
- Extended `lowbits` transform to support 4-bit blocks.
- Added documentation covering entropy pools.

---

## Version 0.84

### RNGs
- **jsf32**: Aligned closely with Bob Jenkins' reference implementation.
- **jsf16**: Adjusted shift constants for improved avalanche behavior.
- **hc256**: Fixed two bugs in `hc256::walk_state` that caused autoseeding crashes.
- Added **sfc16 / sfc32 / sfc64**: Fast, small-state (3-word) generators based on ARX operations.
- Removed `mwlac32` and `mwlac64` (redundant with JSF family).
- Added `NotRecommended` category for flawed generators intended specifically to benchmark statistical test sensitivity.

### Tests & Tools
- Added `switching` transform.
- Added performance profiling and execution timing to example programs.
- Moved standard test lists from examples into library core.
- Added RNG test results documentation.

---

## Version 0.83

### Fixes & Licensing
- Fixed missing symbol and return value bugs in `randi_fast`.
- Fixed HC-256 bugs (uninitialized XOR operand and reverse buffer traversal).
- Updated licensing: Replaced SHA-2 and MT19937 implementations with public domain equivalents (offering smaller size and higher performance).
- Relocated standard test batteries into library header `PractRand/test_batteries.h`.

---

## Version 0.82

### General
- Resolved GCC and MinGW compilation issues.
- Added new tests:
  - **FPF** (Floating Point Frequency).
  - **CoupGap** (Coupon Collector / Gap hybrid).
- Added `get_more_tests` battery helper to example programs.
- Tweaked `arbee` entropy accumulation and interfaces.

---

## Version 0.81

### Fixes & Refinements
- Fixed missing symbol `randi_fast_implementation`.
- Resolved binary bloat in `PractRand.lib` and `PractRand_full.lib`.
- Fixed edge-case issues in uniform floating-point generation.
- Added partial C++0x TR1 distribution compatibility on `vRNG8/16/32/64`.
- Generator refinements:
  - `jsf`: Matched Jenkins' reference implementation and blocked known bad states.
  - `arbee`: Improved mixing quality and eliminated short cycles.
  - `efiix`: Reverted to earlier, higher-speed variant.
- Verified licensing across bundled algorithms (`jsf`, `ibaa`).

---

## Version 0.80 (Initial Release)

### Generator Infrastructure
- **RNG Categories**:
  - Raw generators.
  - Lightweight generators.
  - Polymorphic wrappers (`vRNG`).
  - Entropy pools.
  - Inline implementations.
  - Template adaptors (Raw $\to$ Lightweight).
- **RNG Interface**:
  - Output types: uniform integers, fast semi-uniform integers, and uniform floating point.
  - Seeding modes: 64-bit integer, arbitrary polymorphic RNG, autoseeding, and entropy accumulation (`add_entropy*`, `flush_buffers`).
  - State walking for serialization, deserialization, checksumming, and avalanche inspection.
  - Optional random-access seeking (`seek_forward`, `seek_backward`).

### Bundled RNG Algorithms
- **Recommended RNGs**:
  - `jsf16`, `jsf32`, `jsf64` (Bob Jenkins' Small Fast PRNG).
  - `mwlac32`, `mwlac64`.
  - `clcg96_32`, `lcg64_32`.
  - `hc256`.
  - `isaac32x256`, `isaac64x256`.
  - `efiix8x256`, `efiix16x256`, `efiix32x256`, `efiix64x256`.
  - `mt19937` (Mersenne Twister).
- **Entropy Pools**:
  - `arbee`.
  - `sha2_based_pool`.
  - `isaac_with_sha2_pool`.

### Statistical Tests & Transforms
- **Core Tests**:
  - `DC6`
  - `BCFN`
  - `Gap-16`
- **Transforms**:
  - `multiplex`
  - `shrink`
  - `lowbits`
  - `FirstNofM`
