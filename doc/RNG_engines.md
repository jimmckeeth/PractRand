# Random Number Generator Engines in PractRand

## Table of Contents

0. [Contents of this file](#0-contents-of-this-file)
1. [Quick recommendations for which RNGs to use](#1-quick-recommendations-for-which-rngs-to-use)
2. [Quick comparison charts for recommended RNGs](#2-quick-comparison-charts-for-recommended-rngs)
3. [Details on the criteria used in sections #1 and #2](#3-details-on-the-criteria-used-in-sections-1-and-2)
4. [Descriptions of recommended RNGs](#4-descriptions-of-recommended-rngs)
5. [Non-recommended RNGs ("other" RNGs)](#5-non-recommended-rngs-other-rngs)
6. [Naming conventions for RNGs in PractRand](#6-naming-conventions-for-rngs-in-practrand)

---

## 0. Contents of this file

- **Section 1:** Quick recommendations for which RNGs to use
- **Section 2:** Quick comparison charts for recommended RNGs
- **Section 3:** Details on the criteria used in sections #1 and #2
- **Section 4:** Longer descriptions of recommended RNGs
- **Section 5:** Non-recommended RNGs ("other" RNGs)
- **Section 6:** Naming conventions for RNGs in PractRand

---

## 1. Quick recommendations for which RNGs to use

The following RNGs have the broadest appeal among normal users:

- **`hc256`**  
  HC-256 is the highest quality recommended RNG and the most cryptographically secure. Its biggest drawback is slow seeding time. It is also a little slow and a bit large, but not so much of either to hinder usability for typical purposes.

- **`sfc64` / `sfc32` / `sfc16`**  
  The `sfc*` RNGs are the fastest of the recommended RNGs and among the smallest. The 32- and 64-bit variants have no known drawbacks, though the 16-bit variant is considered inadequate for parallel uses.

- **`chacha`**  
  ChaCha offers cryptographic security, fast random access, fast seeding, and easily adjustable security vs. speed tradeoffs. Unfortunately, while seeding and random access are fast, generating random numbers at any particular setting ends up being slow relative to other RNGs of comparable security and quality—at least with the PractRand implementation (which currently lacks the SIMD optimizations necessary for high-speed ChaCha execution).

- **`efiix64x384` / `efiix32x384` / `efiix16x384` / `efiix8x384`**  
  The fastest recommended RNG that qualified for a "future-proof" five-star quality rating. It is also the fastest RNG to offer any degree of cryptographic security. Unfortunately, its security rating suffers from insufficient third-party analysis. Additionally, it is a bit slow to seed, particularly on non-64-bit platforms.

---

## 2. Quick comparison charts for recommended RNGs

This section contains lists of RNG algorithms and variants included in the PractRand library, rated from 0 to 5 stars on each of several broad criteria. Each list is restricted to RNGs recommended for a particular purpose; some RNGs thus appear on multiple lists. Descriptions of rating categories are in [Section 3](#3-details-on-the-criteria-used-in-sections-1-and-2), and descriptions of individual RNGs are in [Section 4](#4-descriptions-of-recommended-rngs).

### Common Requirements for Recommended RNGs

In addition to rated criteria, every recommended RNG meets the following baseline requirements:
- Deterministic
- Endian-safe (when used correctly)
- Excellent performance on statistical tests (exception: `mt19937`)
- Fast enough for most purposes (exception: `sha2_based_pool`)
- Supports serialization
- Internally uses only integer math (no floating point)
- Natively operates on 8, 16, 32, or 64-bit integers
- Natively outputs an integer of the same size it operates on internally
- Supports seeding from a 64-bit integer

---

### Recommended for Normal Use on 64-bit CPUs

| Engine | Quality | Speed | Theory | Size | Notes |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `sfc64` | 3 ★★★ | 4 ★★★★ | 1 ★ | 32 bytes | Default recommendation |
| `jsf64` | 3 ★★★ | 4 ★★★★ | 0 | 32 bytes | — |
| `mrc64` | 3 ★★★ | 4 ★★★★ | 1 ★ | 24 bytes | — |
| `mrfast64` | 2 ★★ | 5 ★★★★★ | 0 | 16 bytes | — |
| `rarns64` | 3 ★★★ | 3 ★★★ | 1 ★ | 24 bytes | Random access |
| `xsm64` | 3 ★★★ | 3 ★★★ | 1 ★ | 32 bytes | Random access |
| `arbee` | 4 ★★★★ | 4 ★★★★ | 1 ★ | 40 bytes | Entropy pooling |
| `trivium` | 5 ★★★★★ | 1 ★ | 2 ★★ | 48 bytes | Cryptographic |
| `efiix64x48` | 5 ★★★★★ | 3 ★★★ | 1 ★ | 416 bytes | Cryptographic |
| `isaac64x256` | 5 ★★★★★ | 2 ★★ | 2 ★★ | 4128 bytes | Cryptographic |

---

### Recommended for Normal Use on 32-bit CPUs

| Engine | Quality | Speed | Theory | Size | Notes |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `sfc32` | 3 ★★★ | 4 ★★★★ | 1 ★ | 16 bytes | Default recommendation |
| `jsf32` | 3 ★★★ | 4 ★★★★ | 1 ★ | 16 bytes | — |
| `mrc32` | 2 ★★ | 4 ★★★★ | 0 | 12 bytes | — |
| `mrfast32` | 1 ★ | 5 ★★★★★ | 0 | 8 bytes | — |
| `rarns32` | 3 ★★★ | 3 ★★★ | 2 ★★ | 12 bytes | — |
| `xsm32` | 3 ★★★ | 3 ★★★ | 2 ★★ | 16 bytes | Random access |
| `chacha(8)` | 5 ★★★★★ | 1 ★ | 4 ★★★★ | 124 bytes | Crypto + random access |
| `efiix32x48` | 5 ★★★★★ | 3 ★★★ | 1 ★ | 832 bytes | Cryptographic |
| `isaac32x256` | 5 ★★★★★ | 2 ★★ | 2 ★★ | 2064 bytes | Cryptographic |
| `hc256` | 5 ★★★★★ | 2 ★★ | 4 ★★★★ | 8580 bytes | Best quality, best crypto |

> [!NOTE]
> ChaCha/Salsa speed ratings are based on non-SIMD implementations; SIMD implementations will be faster.

---

### Recommended for Normal Use on 16-bit CPUs

| Engine | Quality | Speed | Theory | Size | Notes |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `sfc16` | 2 ★★ | 4 ★★★★ | 1 ★ | 8 bytes | — |
| `mrc16` | 1 ★ | 4 ★★★★ | 0 | 6 bytes | — |
| `efiix16min` | 2 ★★ | 3 ★★★ | 1 ★ | 12 bytes | — |
| `efiix16x48` | 5 ★★★★★ | 3 ★★★ | 1 ★ | 776 bytes | — |

---

### Recommended for Normal Use on 8-bit CPUs

| Engine | Quality | Speed | Theory | Size | Notes |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `efiix8min` | 2 ★★ | 3 ★★★ | 1 ★ | 6 bytes | — |
| `efiix8x48` | 5 ★★★★★ | 3 ★★★ | 1 ★ | 388 bytes | — |

> [!NOTE]
> Using an RNG of one word size on a CPU of a different word size incurs a performance penalty, but is otherwise fine.

---

### Random Access RNGs (Seekable Streams)

Some RNGs allow arbitrary skipping forward or backward efficiently in the output stream:

| Engine | Quality | Speed | Size | Word Size | Other Notes |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `xsm32` | 3 ★★★ | 4 ★★★★ | 16 bytes | 32-bit | — |
| `xsm64` | 4 ★★★★ | 4 ★★★★ | 32 bytes | 64-bit | — |
| `chacha(8)` | 5 ★★★★★ | 1 ★ | 124 bytes | 32-bit | Cryptographic |
| `chacha(12)` | 5 ★★★★★ | 1 ★ | 124 bytes | 32-bit | Cryptographic |
| `chacha(20)` | 5 ★★★★★ | 0 | 124 bytes | 32-bit | Cryptographic |
| `salsa(8)` | 4 ★★★★ | 1 ★ | 140 bytes | 32-bit | Cryptographic |
| `salsa(12)` | 5 ★★★★★ | 0 | 140 bytes | 32-bit | Cryptographic |
| `salsa(20)` | 5 ★★★★★ | 0 | 140 bytes | 32-bit | Cryptographic |

---

### Progressive Seeding / Entropy Pooling RNGs

Some RNGs allow versatile progressive seeding, useful for adapting information in irregular or exotic formats to serve as a seed or key:

| Engine | Quality | Speed | Size | Word Size | Other Notes |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `arbee` | 4 ★★★★ | 4 ★★★★ | 40 bytes | 64-bit* | — |
| `sha2_based_pool` | 5 ★★★★★ | 0 | 302 bytes | 8/64-bit* | Cryptographic |

---

### Cryptographically Secure RNGs

These RNGs make it extraordinarily difficult for an attacker to reconstruct their seed (key) or internal state from examining output:

| Engine | Quality | Speed | Size | Word Size | Crypto Security | Other Notes |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| `trivium` | 5 ★★★★★ | 1 ★ | 48 bytes | 64-bit | 2 ★★ | — |
| `chacha(8)` | 5 ★★★★★ | 1 ★ | 124 bytes | 32-bit | 2 ★★ | Random access |
| `chacha(12)` | 5 ★★★★★ | 1 ★ | 124 bytes | 32-bit | 4 ★★★★ | Random access |
| `chacha(20)` | 5 ★★★★★ | 0 | 124 bytes | 32-bit | 4 ★★★★ | Random access |
| `salsa(8)` | 4 ★★★★ | 1 ★ | 140 bytes | 32-bit | 1 ★ | Random access |
| `salsa(12)` | 5 ★★★★★ | 0 | 140 bytes | 32-bit | 3 ★★★ | Random access |
| `salsa(20)` | 5 ★★★★★ | 0 | 140 bytes | 32-bit | 4 ★★★★ | Random access |
| `sha2_based_pool` | 5 ★★★★★ | 0 | 302 bytes | 8/64-bit* | 3 ★★★ | Entropy pooling |
| `efiix8x48` | 5 ★★★★★ | 3 ★★★ | 52 bytes | 8-bit | 0 | — |
| `efiix16x48` | 5 ★★★★★ | 3 ★★★ | 104 bytes | 16-bit | 1 ★ | — |
| `efiix32x48` | 5 ★★★★★ | 3 ★★★ | 208 bytes | 32-bit | 1 ★ | — |
| `efiix64x48` | 5 ★★★★★ | 3 ★★★ | 416 bytes | 64-bit | 1 ★ | — |
| `isaac32x256` | 5 ★★★★★ | 2 ★★ | 2064 bytes | 32-bit | 2 ★★ | — |
| `isaac64x256` | 5 ★★★★★ | 2 ★★ | 4128 bytes | 64-bit | 2 ★★ | — |
| `hc256` | 5 ★★★★★ | 2 ★★ | 8580 bytes | 32-bit | 5 ★★★★★ | — |

---

### Low-End, Embedded, or Exotic Hardware Constraints

Non-standard limitations may apply on low-end hardware:

1. **Multiplication is very costly:**  
   Most recommended RNGs do not use multiplication.  
   *Exceptions:* `mrc64`, `mrc32`, `mrc16`, `mrfast64`, `mrfast32`.
2. **Cannot use floating-point math:**  
   No problem; none of the recommended RNGs use floating point.
3. **Memory is a major constraint:**  
   Refer to the `Size` column in the charts above. In particular, `mrc`, `mrsf`, `sfc`, `jsf`, `xsm`, `arbee`, `trivium`, and `chacha` are reasonably small for their quality and feature sets.
4. **Integer math must be emulated on floating point:**  
   All PractRand RNGs will tend to be slow on such hardware.
5. **Barrel shifts are inefficient:**  
   Even hardware lacking native barrel shifts can generally emulate them at reasonable speeds. To avoid barrel shifts completely: `isaac32x256`, `isaac64x256`, and `trivium`.
6. **Integer sizes are not 8, 16, 32, or 64 bits:**  
   Some recommended algorithms may still be efficient, but custom implementations will be required.
7. **Need to minimize transistor count in hardware:**  
   `trivium`, `sfc*`, and `jsf*` can all be implemented with minimal hardware. Of these, Trivium offers the highest quality and is the only one providing cryptographic security.

---

### Frozen RNG Algorithms

The following RNG algorithms are frozen—they will produce identical results across all future versions (barring bug fixes):
- `jsf64`
- `jsf32`
- `isaac32x256` (aka "ISAAC")
- `isaac64x256` (aka "64-bit ISAAC")
- `mt19937` (aka "The Mersenne Twister")
- `hc256` (aka "HC-256")
- `trivium`
- `chacha` (aka ChaCha) *(even number of rounds only)*
- `salsa` (aka Salsa20) *(even number of rounds only)*

Other recommended RNGs in PractRand may vary between versions, but will stabilize as PractRand approaches version 1.0.

---

## 3. Details on the criteria used in sections #1 and #2

### Quality (Rated 0 to 5 Stars)

**Broad guidelines:**
- **1 star:** Good enough for typical apps & games
- **2 stars:** Good enough for almost any* non-parallel purpose today
- **3 stars:** Good enough for almost any* purpose today
- **4 stars:** Good enough for any* purpose in the next few years
- **5 stars:** Good enough for any* purpose  
*( \* = excluding cryptographic purposes, which are rated separately)*

An RNG's quality score is determined as the **LOWEST** of several subscores: empirical testing, cycle length, statespace, and trust/analysis.

#### 1. Empirical Testing
- **1 star:** Must pass 1 GB of PractRand standard; must pass TestU01 SmallCrush and gjrand `pmcp --small`.
- **2 stars:** Must pass 16 GB of PractRand standard; must pass TestU01 SmallCrush and gjrand `pmcp --standard`.
- **3 stars:** Must pass 256 GB of PractRand standard; must pass TestU01 Crush and gjrand `pmcp --big`.
- **4 stars:** Must pass 4 TB of PractRand standard; must pass TestU01 Crush & BigCrush and gjrand `pmcp --tera`.
- **5 stars:** Must not have any known failures on general-purpose tests.

#### 2. Cycle Length (Period)
- **1 star:** Minimum cycle length $\ge 2^{40}$, or average cycle length $\ge 2^{45}$.
- **2 stars:** Minimum cycle length $\ge 2^{45}$, or average cycle length $\ge 2^{55}$.
- **3 stars:** Minimum cycle length $\ge 2^{50}$, or average cycle length $\ge 2^{70}$.
- **4 stars:** Minimum cycle length $\ge 2^{55}$, or average cycle length $\ge 2^{95}$.
- **5 stars:** Minimum cycle length $\ge 2^{80}$, or average cycle length $\ge 2^{160}$.

#### 3. Statespace
- **1 star:** Statespace $\ge 2^{40}$
- **2 stars:** Statespace $\ge 2^{60}$
- **3 stars:** Statespace $\ge 2^{90}$
- **4 stars:** Statespace $\ge 2^{150}$
- **5 stars:** Statespace $\ge 2^{240}$  
*(Closely related to cycle length)*

#### 4. Trusted (Author's Analysis & Trustworthiness)
- **1 star:** Not TOO horribly broken
- **2 stars:** Probably adequate
- **3 stars:** Mostly trustworthy
- **4 stars:** Quite solid
- **5 stars:** Completely solid  
*(Takes into account seeding, avalanche testing, interstate correlations, linear patterns, subcycles, etc.)*

---

### Quality Subscores for PractRand RNGs

| Engine | Overall | Empirical | Cycle Length | Statespace | Trusted |
| :--- | :---: | :---: | :---: | :---: | :---: |
| `jsf32` | 3 | 5 | 4 | 3 | 3 |
| `jsf64` | 3 | 5 | 5 | 5 | 3 |
| `sfc16` | 2 | 4 | 2 | 2 | 2 |
| `sfc32` | 3 | 5 | 4 | 3 | 3 |
| `sfc64` | 3 | 5 | 5 | 5 | 3 |
| `xsm32` | 3 | 4 | 4 | 3 | 3 |
| `xsm64` | 4 | 5 | 5 | 4 | 4 |
| `mrfast32` | 1 | — | 2 | 2 | 1 |
| `mrfast64` | 2 | 5 | 4 | 3 | 2 |
| `mrc16` | 1 | 3 | 1 | 1 | 1 |
| `mrc32` | 2 | 5 | 4 | 3 | 2 |
| `mrc64` | 3 | 5 | 5 | 4 | 3 |
| `arbee` | 4 | 5 | 5 | 5 | 4 |
| `isaac32x256` | 5 | 5 | 5 | 5 | 5 |
| `isaac64x256` | 5 | 5 | 5 | 5 | 5 |
| `efiix8x384` | 5 | 5 | 5 | 5 | 5 |
| `efiix16x384` | 5 | 5 | 5 | 5 | 5 |
| `efiix32x384` | 5 | 5 | 5 | 5 | 5 |
| `efiix64x384` | 5 | 5 | 5 | 5 | 5 |
| `hc256` | 5 | 5 | 5 | 5 | 5 |
| `trivium` | 5 | 5 | 5 | 5 | 5 |
| `sha2_based_pool` | 5 | 5? | 5 | 5 | 5 |
| `chacha (8)` | 5 | 5 | 5* | 5 | 5 |
| `chacha (12)` | 5 | 5 | 5* | 5 | 5 |
| `chacha (20)` | 5 | 5 | 5* | 5 | 5 |
| `salsa (8)` | 4.5 | 5 | 5* | 5 | 4.5 |
| `salsa (12)` | 5 | 5 | 5* | 5 | 5 |
| `salsa (20)` | 5 | 5 | 5* | 5 | 5 |
| `mt19937` | 2 | 2 | 5 | 5 | 3 |

*\* Normally cycle length is $2^{68}$ (score 4), but an optional tweak extends it to $2^{100}$ (score 5).*

#### Subscore Explanations for PractRand Engines

- **`jsf32`** (3 / 5 / 4 / 3 / 3):
  - *Empirical (5):* Passes all tests so far (thoroughly tested).
  - *Cycle (4):* Average cycle $\approx 2^{127}$, fine for modern uses but not future-proof.
  - *States (3):* $\approx 2^{128}$; statespace issues are difficult but possible to hit.
  - *Trust (3):* Looks good from all angles, but not perfect.
- **`jsf64`** (3 / 5 / 5 / 5 / 3):
  - *Empirical (5):* Passes all tests so far.
  - *Cycle (5):* Average cycle $\approx 2^{255}$, future-proof.
  - *States (5):* $\approx 2^{256}$, future-proof.
  - *Trust (3):* Adding extra bits beyond `jsf32` did not significantly improve structure.
- **`arbee`** (4 / 5 / 5 / 5 / 4):
  - *Empirical (5):* Passes all tests so far.
  - *Cycle (5):* Average cycle $\approx 2^{319}$ ($\text{min} \ge 2^{64}$), future-proof.
  - *States (5):* $2^{320}$, future-proof.
  - *Trust (4):* Better mixed and slightly larger than `jsf64`; fewer places for flaws to hide.
- **`sfc16 (v4)`** (2 / 4 / 2 / 2 / 2):
  - *Empirical (4):* Flaws detectable on extremely long runs (512 TB on PractRand standard).
  - *Cycle (2):* Average cycle $\approx 2^{63}$ ($\text{min} \ge 2^{16}$), marginal for some uses.
  - *States (2):* $2^{64}$, marginal for some modern uses.
  - *Trust (2):* Apparently decent, but author remains suspicious.
- **`sfc32 (v4)`** (3 / 5 / 4 / 3 / 3):
  - *Empirical (5):* Passed all tests.
  - *Cycle (4):* Average cycle $\approx 2^{127}$ ($\text{min} \ge 2^{32}$), good for today, not future-proof.
  - *States (3):* $2^{128}$, marginal for extreme modern uses.
  - *Trust (3):* Looks solid.
- **`sfc64 (v4)`** (3 / 5 / 5 / 5 / 3):
  - *Empirical (5):* Passed all tests.
  - *Cycle (5):* Average cycle $\approx 2^{255}$ ($\text{min} \ge 2^{64}$), future-proof.
  - *States (5):* $2^{256}$, future-proof.
  - *Trust (3):* Extra bits over `sfc32` did not drastically change structural confidence.
- **`xsm32`** (3 / 4 / 4 / 3 / 3):
  - *Empirical (4):* Passes all output tests, though seeding test results are weaker.
  - *Cycle (4):* Always $2^{64}$, good for current uses, not future-proof.
  - *States (3):* $2^{95}$, marginal for some modern parallel applications.
  - *Trust (3):* State transition is terrible, but output hashing compensates; interseed correlations could emerge in worst-case scenarios.
- **`xsm64`** (4 / 5 / 5 / 4 / 4):
  - *Empirical (5):* Passes all tests so far.
  - *Cycle (5):* Always $2^{128}$, future-proof.
  - *States (4):* $2^{192}$, suitable for anything currently, but not fully future-proof.
  - *Trust (4):* Extra bits provide a substantial structural improvement.
- **`mrfast32`** (1 / — / 2 / 2 / 1) & **`mrfast64`** (2 / 5 / 4 / 3 / 2)
- **`mrc16`** (1 / 3 / 1 / 1 / 1), **`mrc32`** (2 / 5 / 4 / 3 / 2), **`mrc64`** (3 / 5 / 5 / 4 / 3)
- **`mt19937`** (2 / 2 / 5 / 5 / 3):
  - *Empirical (2):* Fails binary rank tests and linear complexity tests (fails TestU01 Crush, PractRand at 256 GB, and gjrand `--huge`).
  - *Cycle (5):* Exactly $2^{19937}-1$, far beyond future-proof.
  - *States (5):* Exactly $2^{19937}-1$.
  - *Trust (3):* Hashing and state size mask underlying flaws for most non-crypto uses, but excessive linearity remains problematic.
- **`hc256`** (5 / 5 / 5 / 5 / 5):
  - Passes all known tests; cycle and statespace are completely future-proof; fully trustworthy.
- **`chacha (8+)`** (5 / 5 / 5* / 5 / 5):
  - Passes all tests; cycle extended to $2^{100}$; fully trustworthy.
- **`salsa (8)`** (4.5 / 5 / 5* / 5 / 4.5) & **`salsa (12+)`** (5 / 5 / 5* / 5 / 5)

---

### Quality Subscores for Non-PractRand RNG Examples

| Engine | Overall | Empirical | Cycle | States | Trusted | Notes |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| Typical libc `rand()` | 0 | 0 | 0 | 0 | 0 | Fails rapidly; $2^{17}$ practical cycle; no rightward mixing |
| `rand48` | 0 | 0 | 1/0 | 1/0 | 0 | Low-bit subcycles ($2^{17}$ in practice) |
| `ICG, m=2**31-1` | 0 | 0 | 0 | 0 | 3.5 | Far too slow to be practical; tiny state & cycle |
| `RC4` | 3.5 | 4 | 5 | 5 | 3.5 | Passes many tests, but fails PractRand after 1 TB |
| `KISS93` | 2 | 2.5 | 5 | 3 | 2 | Fails BigCrush subtest and PractRand at 2 TB |
| `KISS4691` | 3 | 4 | 5 | 5 | 3 | Passes BigCrush; eventually fails low-bit long-range correlation tests |

---

### Other Rating Criteria

- **Speed (0 to 5 Stars):**
  - **0 stars:** Very slow — too slow for normal use
  - **1 star:** Slow — barely fast enough for normal use
  - **2 stars:** Medium-slow
  - **3 stars:** Medium-fast
  - **4 stars:** Fast
  - **5 stars:** Very fast  
> [!NOTE]
> * Speed is relative and machine-dependent. Users should run `RNG_benchmark` on their target system. Consider output size (bits/call) and CPU word size mismatch penalties.
- **Theory (0 to 5 Stars):**
  - Reflects academic study and mathematical tractability.
  - **0 stars:** Known strictly empirically or by instinct.
  - **1 star:** Provable properties exist; $\ge 1$ published paper.
  - **5 stars:** Thoroughly documented academic analysis and flaws.  
> [!NOTE]
> * Most recommended RNGs have low theory ratings because algorithms easy to analyze mathematically (LCGs, LFSRs) tend to make poor PRNGs.
- **Size (in bytes):** Implementation footprint. Larger sizes offer larger state and cycles, but slower initialization, higher cache pressure, and poorer inlining.
- **Word:** Native integer math size (8, 16, 32, or 64-bit). Usually equals the output word size.
- **Statespace Size:** Total count of distinct internal states. Important for parallel stream generation.
- **Minimum Cycle Length (Period):** The shortest possible cycle. For chaotic RNGs this may be unprovable (listed as 1).
- **Operations Used:** Arithmetic and logic operations required (useful when evaluating embedded/exotic hardware compatibility).
- **Full Word Output:** Random bits generated per call match the internal integer word size (true for all recommended PractRand RNGs).
- **Buffering:** Produces a block of words internally and buffers them for subsequent calls.
- **Random Access:** Capable of seeking forward/backward efficiently in the output stream.
- **Entropy Pooling:** Supports progressive seeding with variable data sizes (can act as a hash/pool).
- **Crypto Security:** Estimated difficulty of state recovery:
  - **1 star:** Analyzed by PractRand author only.
  - **2 stars:** Analyzed by a few competent reviewers.
  - **3 stars:** Analyzed by extensive public research.
  - **+1 star:** Established reputation, wide security margin, or $>10$ years of study.
  - **+1 star:** Provably secure or self-evident security.
- **Multi-cyclic:** Statespace exceeds the minimum cycle length.
- **Reversible:** Average cycle length is a substantial fraction of the statespace (irreversible RNG cycles scale around $\sqrt{\text{statespace}}$).

---

## 4. Descriptions of recommended RNGs

### Summary: Pros and Cons

| Engine(s) | Pros | Cons |
| :--- | :--- | :--- |
| `sfc16` / `sfc32` / `sfc64` | Fast, small | None |
| `hc256` | Highest quality, strong cryptographic security | Large, slow to seed, slow generation |
| `chacha` / `salsa` | Random access, cryptographic security, high quality, fast seeding | Slow without SIMD optimizations |
| `efiix8x384` / `efiix16x384` / `efiix32x384` / `efiix64x384` | High quality, fast for its quality class, marginal crypto security | Slow to seed |
| `mt19937` | Well-studied, widely recognized | Large, slow to seed, poor quality vs. speed |
| `jsf32` / `jsf64` | Fast, small | Some bad cycles exist |
| `xsm32` / `xsm64` | Random access, small | Requires fast hardware multiplication |
| `mrsf32` / `mrsf64` | Very fast, very small | Requires fast multiplication, bad cycles exist |
| `mrc16` / `mrc32` / `mrc64` | Fast, small | Requires fast multiplication |
| `trivium` | Small, crypto security, minimal hardware gates, high quality | Slow in software |
| `sha2_based_pool` | High quality, crypto security, entropy pooling | Very slow |
| `arbee` | Small, fast, entropy pooling | None |
| `isaac32x256` / `isaac64x256` | Cryptographic security | Large, slow, somewhat slow to seed |

---

### Small Fast RNGs

Small fast RNGs span 2 to 5 words in state size (16 to 40 bytes on 64-bit systems) and constitute the fastest category.

#### `jsf32` / `jsf64`
- **Name:** Unnamed (termed JSF: *Jenkins' Small Fast PRNG*), written by Bob Jenkins.
- Serves as the author's baseline for small fast RNGs. Well studied with no biases found in 32- or 64-bit outputs.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 4 / 5 | Statespace: 3 / 5 | Trusted: 3 | Overall: 3
  - *Speed:* Fast
  - *Operations:* Addition, bitwise logic, fixed shifts
  - *Features:* Full word output, unbuffered, no random access, no entropy pooling, no crypto security
  - *Cycle / State:* Min cycle: 1 | Statespace: $2^{128}-4$ (32-bit) / $2^{256}-1$ (64-bit)
  - *Footprint:* 16 bytes (32-bit) / 32 bytes (64-bit)
  - *Properties:* Multi-cyclic, reversible

#### `sfc16` / `sfc32` / `sfc64`
- **Name:** *Small Fast Counting RNG, version 4*, written by Chris Doty-Humphrey.
- Combines high speed, compact state, good statistical qualities, and guaranteed minimum periods via a driving counter.
- **Attributes:**
  - *Quality Subscores:* Empirical: 4 / 5 / 5 | Cycle: 2 / 4 / 5 | Statespace: 2 / 3 / 5 | Trusted: 2 / 3 / 3 | Overall: 2 / 3 / 3
  - *Speed:* Fast
  - *Operations:* Addition, bitwise logic, fixed shifts
  - *Features:* Full word output, unbuffered, no random access, no entropy pooling, no crypto security
  - *Minimum Cycle:* $2^{16}$ (16-bit) / $2^{32}$ (32-bit) / $2^{64}$ (64-bit)
  - *Statespace:* $2^{64}$ / $2^{128}$ / $2^{256}$
  - *Footprint:* 8 bytes (16-bit) / 16 bytes (32-bit) / 32 bytes (64-bit)
  - *Properties:* Multi-cyclic, reversible

#### `mrsf32` / `mrsf64`
- **Name:** *Multiply & Rotate, Small & Fast* (or *Mrs. Fast*).
- Designed to fit entirely inside two CPU registers.
- **Attributes:**
  - *Quality Subscores:* Cycle: 3 / 3 | Statespace: 3 / 4 | Trusted: 1 / 2 | Overall: 1 / 2
  - *Speed:* Very fast
  - *Operations:* Addition, bitwise logic, fixed shifts, multiplication
  - *Features:* Full word output, unbuffered, no random access, no crypto security
  - *Minimum Cycle:* 1 | Statespace: $2^{64}$ (32-bit) / $2^{128}$ (64-bit)
  - *Footprint:* 8 bytes (32-bit) / 16 bytes (64-bit)
  - *Properties:* Multi-cyclic, reversible

#### `mrc16` / `mrc32` / `mrc64`
- **Name:** *Multiply, Rotate, Count* (or *Mr. Count*).
- Similar to `mrsf`, but adds a driving counter to prevent short cycles.
- **Attributes:**
  - *Speed:* Very fast
  - *Operations:* Addition, bitwise logic, fixed shifts, multiplication
  - *Footprint:* 6 bytes (16-bit) / 12 bytes (32-bit) / 24 bytes (64-bit)
  - *Properties:* Multi-cyclic, reversible

---

### Cryptographic & High-Quality RNGs

These generators provide superior statistical strength and varying levels of cryptographic security, typically at the cost of slower seeding.

#### `isaac32x256` / `isaac64x256`
- **Name:** ISAAC / ISAAC64 (*Indirection, Shift, Accumulate, Add, Count*), by Bob Jenkins.
- Indirection-based array RNG combining good statistical properties with moderate cryptographic resistance.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 5 | Statespace: 5 | Trusted: 5 | Overall: 5
  - *Speed:* Medium-slow
  - *Operations:* Addition, bitwise logic, fixed shifts, array indexing
  - *Features:* Full word output, buffered, no random access, moderate crypto security
  - *Minimum Cycle:* $2^{40}$ (32-bit) / $2^{72}$ (64-bit)
  - *Statespace:* $2^{8296}$ (32-bit) / $2^{16584}$ (64-bit)
  - *Footprint:* 2064 bytes (32-bit) / 4128 bytes (64-bit)
  - *Properties:* Multi-cyclic, reversible

#### `hc256`
- **Name:** HC-256.
- Large Fibonacci-style table-based cipher with indirection. Designed for provable cryptographic resistance.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 5 | Statespace: 5 | Trusted: 5 | Overall: 5
  - *Speed:* Slow
  - *Operations:* Addition, bitwise logic, fixed shifts, array indexing
  - *Features:* Full word output, buffered, strong cryptographic security
  - *Minimum Cycle:* None known (no bad cycles suspected)
  - *Statespace:* $2^{65547}$ | Footprint: 8580 bytes | Word Size: 32-bit
  - *Properties:* Multi-cyclic, reversible

#### `efiix8x384` / `efiix16x384` / `efiix32x384` / `efiix64x384`
- **Name:** EFIIX (*Entropy From Iteration, Indirection, XOR, and addition*), by Chris Doty-Humphrey.
- Maintains 384 braided strands of internal state with dynamic permutation and intermixing. Fastest CSPRNG in PractRand; unbuffered.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 5 | Statespace: 5 | Trusted: 5 | Overall: 5
  - *Speed:* Medium-slow
  - *Operations:* Addition, bitwise logic, fixed shifts, array indexing
  - *Features:* Full word output, unbuffered, some crypto security
  - *Minimum Cycle:* $2^8$ / $2^{16}$ / $2^{32}$ / $2^{64}$ (no bad cycles suspected)
  - *Statespace:* $2^{3104}$ / $2^{6208}$ / $2^{12416}$ / $2^{24832}$
  - *Footprint:* 388 / 776 / 1552 / 3104 bytes
  - *Properties:* Multi-cyclic, reversible

#### `trivium`
- Lightweight hardware-oriented stream cipher consisting of three coupled shift registers.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 5 | Statespace: 5 | Trusted: 5 | Overall: 5
  - *Speed:* Quite slow in software
  - *Operations:* Bitwise logic, fixed shifts
  - *Features:* Full word output (64-bit), unbuffered, low-to-medium crypto security (medium if initial outputs are discarded)
  - *Statespace:* $2^{288}$ | Footprint: 48 bytes
  - *Properties:* Multi-cyclic, reversible

#### `salsa` / `chacha` (Variable Rounds)
- **Names:** Salsa20/# (Snuffle 2005) and ChaCha# (Snuffle 2008), by Daniel J. Bernstein.
- Operates like a block cipher applied to a counter. ChaCha is simpler, faster, and slightly more secure than Salsa.
- **Rounds vs. Quality and Security:**

  | Rounds | ChaCha Quality | ChaCha Security | Salsa Quality | Salsa Security |
  | :---: | :---: | :---: | :---: | :---: |
  | 20 | 5 ★★★★★ | 4 ★★★★ | 5 ★★★★★ | 4 ★★★★ |
  | 12 | 5 ★★★★★ | 4 ★★★★ | 5 ★★★★★ | 3 ★★★ |
  | 8 | 5 ★★★★★ | 2 ★★ | 4 ★★★★ | 1 ★ |
  | 6 | 4 ★★★★ | 0 | 3 ★★★ | 0 |
  | 4 | 3 ★★★ | 0 | 1 ★ | 0 |

  *(Only even round numbers are recommended.)*

- **Period Extension Tweak:** The standard cycle is $2^{68}$. PractRand implements an optional carry-overflow tweak that extends the period to $2^{100}$ (enabled by default in PractRand autoseeding mode).
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 5* | Statespace: 5 | Trusted: 5 | Overall: 5
  - *Speed:* Quite slow (faster with SIMD implementations)
  - *Operations:* Addition, bitwise logic, fixed shifts
  - *Features:* Full word output, buffered, random access, medium-to-high crypto security
  - *Statespace:* $2^{384}$ | Footprint: 124 bytes (ChaCha), 136–140 bytes (Salsa)
  - *Properties:* Multi-cyclic, reversible

---

### Popular RNGs

#### `mt19937`
- **Name:** Mersenne Twister.
- Widely recognized and mathematically analyzed single-cycle generator based on a large twisted LFSR with linear output tempering.
- **Attributes:**
  - *Quality Subscores:* Empirical: 2 | Cycle: 5 | Statespace: 5 | Trusted: 2–3 | Overall: 2
- > [!NOTE]
  > Empirical Note: Fails binary matrix rank tests; fails PractRand standard after 256 GB.
  - *Speed:* Medium-slow
  - *Operations:* Addition, bitwise logic, fixed shifts, simple arrays (multiplication in seeding)
  - *Features:* Full word output, buffered, no effective random access, no crypto security
  - *Cycle & Statespace:* Exactly $2^{19937}-1$ (single cycle)
  - *Footprint:* 2500 bytes | Word Size: 32-bit
  - *Properties:* Single-cycle, reversible

---

### Entropy Pooling RNGs

An entropy pool accepts arbitrary variable-length input and generates an infinite pseudo-random stream acting as a continuous hash.

#### `arbee`
- Based on Bob Jenkins' 3-rotate 64-bit JSF, augmented with a cycle-guarantee counter and progressive seeding.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 5 | Statespace: 5 | Trusted: 4 | Overall: 4
  - *Speed:* Medium-fast
  - *Operations:* Addition, bitwise logic, fixed shifts
  - *Features:* Full word output (64-bit), unbuffered output, entropy pooling, no crypto security
  - *Minimum Cycle:* $2^{64}$ | Statespace: $2^{320}$ ($2^{256}$ effective)
  - *Footprint:* 40 bytes
  - *Properties:* Multi-cyclic, reversible

#### `sha2_based_pool`
- Cryptographic entropy pool built directly around SHA2-512 and state buffers.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5? | Cycle: 5 | Statespace: 5 | Trusted: 5 | Overall: 5
  - *Speed:* Very slow
  - *Operations:* Addition, bitwise logic, fixed shifts, array indexing
  - *Features:* Buffered, entropy pooling, moderate crypto security
  - *Footprint:* ~302–308 bytes | Word Size: mixed 8/64-bit
  - *Properties:* Multi-cyclic, irreversible

---

### Random Access RNGs

Random access (seekable) RNGs allow direct forward and backward jumping to arbitrary stream offsets without full iteration.

#### `xsm32` / `xsm64`
- Developed specifically for PractRand to replace slow LCG-based seekable engines.
- Combines a fast linear LCG core with a seed-dependent additive constant and a non-linear output permutation function.
- **Attributes:**
  - *Quality Subscores:* Empirical: 5 | Cycle: 3 / 5 | Statespace: 3 / 4 | Trusted: 3 / 4 | Overall: 3 / 4
  - *Speed:* Medium-fast
  - *Operations:* Addition, bitwise logic, fixed shifts, hardware multiplication
  - *Features:* Full word output, unbuffered, random access, very low crypto security
  - *Minimum Cycle:* $2^{64}$ (32-bit) / $2^{128}$ (64-bit)
  - *Statespace:* $2^{96}$ (32-bit) / $2^{192}$ (64-bit)
  - *Footprint:* 16 bytes (32-bit) / 32 bytes (64-bit)
  - *Properties:* Multi-cyclic, reversible

#### `chacha8` / `chacha12` / `chacha20`
*(Refer to description under [Cryptographic & High-Quality RNGs](#salsa--chacha-variable-rounds).)*

---

## 5. Non-recommended RNGs ("other" RNGs)

PractRand includes a separate suite of non-recommended RNG algorithms located in the `PractRand::RNGs::NotRecommended` namespace (source headers in `include/PractRand/RNGs/other/`).

### Purpose
Recommended PRNGs rarely fail statistical tests in practical running times. Non-recommended algorithms with known biases and structural flaws are preserved specifically as benchmarks to evaluate and calibrate new statistical tests.

### Categories and Headers

1. **`simple`** (`include/PractRand/RNGs/other/simple.h`):  
   Small simple RNGs that do not use multiplication.
2. **`mult`** (`include/PractRand/RNGs/other/mult.h`):  
   Small simple RNGs that utilize multiplication.
3. **`fibonacci`** (`include/PractRand/RNGs/other/fibonacci.h`):  
   RNGs that utilize arrays with simple, linear access patterns.
4. **`indirection`** (`include/PractRand/RNGs/other/indirection.h`):  
   RNGs that utilize arrays with complex, data-dependent access patterns.
5. **`transform`** (`include/PractRand/RNGs/other/transform.h`):  
   RNG wrappers parameterized on an inner engine (e.g., Bays-Durham Shuffle).
6. **`special`** (`include/PractRand/RNGs/other/special.h`):  
   RNGs with complex control flow, transcendental math (`sin`, `log`, etc.), or unique characteristics.

---

## 6. Naming conventions for RNGs in PractRand

The canonical naming convention for RNG engines in PractRand follows these rules:

- **Rule A:** All algorithm names are strictly lowercase.
- **Rule B:** Hyphens and special characters from original published names are omitted.
- **Rule C:** If the original name ended with a numeral, that numeral is preserved at the end.  
  *Example:* `HC-256` $\rightarrow$ `hc256`.
- **Rule D:** If Rule C does not apply and the engine discards bits from a wider internal word (as in truncated LCGs), the suffix `A_B` is appended, where `A` is the internal state bit width and `B` is the output bit width.  
  *Example:* A 64-bit LCG discarding to 32-bit output $\rightarrow$ `lcg64_32`.
- **Rule E:** If neither Rule C nor Rule D applies, and the algorithm includes a parameterizable indirection table, `AxB` is appended, where `A` is the word bit width and `B` is the number of table elements.  
  *Example:* ISAAC (32-bit variant, 256-element table) $\rightarrow$ `isaac32x256`.
- **Rule F:** If none of Rules C, D, or E apply, the primary word size in bits is appended to the name.  
  *Example:* MWLAC (32-bit variant) $\rightarrow$ `mwlac32`.
