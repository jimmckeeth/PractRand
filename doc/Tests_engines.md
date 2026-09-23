# Specific Tests in PractRand

This document details the individual statistical test engines and test transforms implemented in PractRand.

## Recommended Tests

### BCFN
Checks for long-range linear correlations (bit counting).

- **Description & Target Biases**: In practice, this often detects Fibonacci-style RNGs that rely upon large lags to defeat other statistical tests.
- **Methodology**: Each individual "level" is a frequency test on overlapping sets of Hamming weights.
- **Parameters**: Two integer parameters are used:
  1. **Minimum "Level"**: The minimum level at which it checks for bias (it checks all higher levels for which it has sufficient data). Higher values are faster but can miss shorter-range correlations. The recommended minimum level is `2`, which skips the slowest parts, avoids redundancy with DC6 (which checks for the shortest-range linear correlations), and performs a reasonable amount of work relative to the memory scanned.
  2. **Table Size Logarithm**: The $\log_2$ of the internal table size, determining the amount of memory and cache used. Recommended values are `10` to `15` (larger values for larger caches, lower values for smaller caches).
- **Memory Requirements**: The variant used in the core tests does not require more than a few megabytes of memory.

---

### DC6
Checks for short-range linear correlations (bit counting).

- **Methodology**: A frequency test on overlapping sets of Hamming weights.
- **Parameters**: Takes several parameters that determine:
  - The size of integers it operates on internally.
  - The number of adjacent integers between which it evaluates correlations.
  - Which information it uses for each integer.
  *(For recommended values, see the implementation of `get_core_tests()` and `get_expanded_core_tests()` in `test_batteries.cpp`.)*
- **Memory Requirements**: The variant used in the core tests does not require more than a few megabytes of memory.

---

### Gap16
A variation on the classic "Gap" test.

- **Parameters**: None required.
- **Memory Requirements**: Does not require more than a few megabytes of memory.

---

### FPF
"Floating Point Frequency" test. Contrary to its name, it uses purely integer arithmetic.

- **Target Biases**: Checks for very short-range correlations—even shorter than DC6—especially those correlations involving many zero bits.
- **Methodology**: Roughly speaking, this test applies a frequency test to the binary format of floating-point numbers storing integer values of overlapping windows of the original data stream.
- **Performance**: Can be slow on certain parameter settings.
- **Memory Requirements**: The variant used in the core tests does not require more than a few megabytes of memory.

---

### BRank
A fairly standard binary matrix rank test.

- **Methodology**: The most original component is the control logic determining when data is sampled from the RNG output stream to form a matrix and what size matrix to construct.
- **Parameters**: A log-scale amount of time per gigabyte spent calculating matrix ranks.
- **P-values**: Due to the coarse-grained nature of its results, precise p-values cannot be computed for many of its subtests.
- **Memory Requirements**: Can use several hundred megabytes of memory on 32-bit platforms, and potentially more on 64-bit platforms.

---

### mod3n
Based on the `mod3` test in gjrand, this test searches for short-range patterns in the value modulo 3 of the RNG output.

- **Methodology**: Examines the RNG output as a sequence of 8-bit, 16-bit, 32-bit, 64-bit, 128-bit, 256-bit values, etc. (similar to BCFN). It samples only small chunks of the output stream here and there rather than testing the entire stream (similar to BRank).
- **Target Biases**: Mostly catches similar biases to DC6 and BCFN; however, while DC6 and BCFN operate on Hamming weights, `mod3n` does not. Consequently, patterns that leave Hamming weight distributions untouched will slip past DC6/BCFN but are readily caught by `mod3n`.
- **Memory Requirements**: The variant used in the core tests does not require more than a few megabytes of memory.

---

### TMFn
A simple test designed to detect simple long- to very long-range correlations commonly present in Linear Congruential Generator (LCG) output.

- **Methodology & Performance**: Inspects only a tiny fraction of the data stream, requiring very little CPU time.
- **Calibration Status**: Test results are not yet calibrated.
- **Memory Requirements**: Does not require more than a few megabytes of memory.

---

## Transforms
*(These appear as tests in listings, but function as wrappers for other tests)*

- **`multiplex`**: Routes operations on this test to one or more wrapped tests.
- **`lowbits`**: For each 4, 8, 16, 32, or 64 bits, extracts the lowest 1, 2, 4, 8, 16, or 32 bits and passes them to the wrapped test(s), discarding the remainder.
- **`shrink`**: Wrapper transform.
- **`FirstNofM`**: Wrapper transform.
- **`BWT`**: Wrapper transform (Burrows-Wheeler Transform).

---

## Non-Recommended Tests

- **`CoupGap`**: "Coupon / Gap hybrid" test; still in development. Behavior is similar to most standard gap tests.
- *(Many more still to come)*
