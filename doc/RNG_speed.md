# RNG Speed Guidelines and Benchmarks

For a typical application, any recommended RNG in PractRand (except `sha2_based_pool`) is more than fast enough. If your program needs more speed, consider the following guidelines:

1. **Match word size to architecture:** If you are on a 64-bit CPU and compiler, use a 64-bit RNG; on a 32-bit CPU and compiler, use a 32-bit RNG, etc. In most cases, the RNG's name indicates its word size. Among recommended RNGs, the exceptions are:
   - `clcg96_32`: uses 64-bit words internally but outputs 32 bits at a time.
   - `hc256`: uses 32-bit words.
   - Entropy pooling RNGs: `arbee` (64-bit), `sha2_based_pool` (64-bit words), and `isaac_with_sha2` (64-bit words).
2. **Use light-weight / raw implementations:** If you need extra speed and do not require polymorphism, consider using the raw or light-weight versions of the RNGs. They are available for almost all RNGs and are usually faster than their polymorphic equivalents.
3. **Fastest recommended algorithms:** The fastest recommended general-purpose RNGs are `jsf` and `sfc`. On 64-bit systems, `arbee` is also fast while providing entropy pooling and guaranteed cycle lengths without short cycles.

---

## Notes on Entropy Pools

Entropy pools appear twice in the benchmark results:
- **Generation Speed:** Listed under *Entropy Pooling RNGs* in the main generation tables. Shows how fast numbers can be produced (in **GB/s**), covering polymorphic and light-weight interfaces where available.
- **Input Speed:** Listed separately under *Entropy Pool Input Speeds*. Measures how quickly entropy can be absorbed (in **MB/s**, unlike generation speeds which are in GB/s), across different input block sizes (`add_entropy8`, `add_entropy16`, `add_entropy32`, `add_entropy64`, and `add_entropy_N`). For buffered pools, throughput is consistent across block sizes; for unbuffered pools, larger input chunks yield substantially higher throughput.

---

## Benchmark: 64-bit GCC

### Environment & Notes
- **CPU:** Intel Core i5 2500
- **Compiler:** g++ (GCC) 4.5.4 20111030 (prerelease) `[svn/rev.180676 - mingw-w64/oz]`
- **OS:** Windows 7 (64-bit)
- **Build Command:**
  ```sh
  g++ -o bin/RNG_benchmark-gcc-x64.exe src/*.cpp src/RNGs/*.cpp test/RNG_benchmark.cpp -Iinclude -O3
  ```
- **Comments:** The candidate RNGs (`VeryFast*` and `RanrotVariant*`) were in the same compilation unit as the benchmark. GCC optimizations did not cross compilation unit boundaries, giving candidate RNGs an unfair advantage.

### Random Number Generation Speeds

#### Small Fast RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `jsf32` | 1.189 GB/s | 1.181 GB/s | 14,471 KHz |
| `jsf64` | 2.403 GB/s | 2.334 GB/s | 22,248 KHz |
| `sfc32` | 1.295 GB/s | 1.271 GB/s | 36,327 KHz |
| `sfc64` | 2.532 GB/s | 2.500 GB/s | 36,326 KHz |

#### Random Access RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `xsm32` | 1.002 GB/s | 0.985 GB/s | 43,456 KHz |
| `xsm64` | 2.011 GB/s | 1.962 GB/s | 43,034 KHz |

#### Entropy Pooling RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `arbee` | 2.065 GB/s | 2.020 GB/s | 27,917 KHz |
| `sha2_based_pool` | — | 0.062 GB/s | 1,300 KHz |

#### Cryptographic / High Quality RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `hc256` | 0.845 GB/s | 0.578 GB/s | 75 KHz |
| `trivium` | 0.740 GB/s | 0.708 GB/s | 4,465 KHz |
| `isaac32x256` | 0.722 GB/s | 0.519 GB/s | 453 KHz |
| `isaac64x256` | 1.449 GB/s | 1.024 GB/s | 374 KHz |
| `efiix32x384` | 0.985 GB/s | 0.736 GB/s | 370 KHz |
| `efiix64x384` | 1.994 GB/s | 1.620 GB/s | 372 KHz |
| `chacha` | 0.296 GB/s | 0.275 GB/s | 4,875 KHz |
| `salsa` | 0.277 GB/s | 0.263 GB/s | 4,639 KHz |

#### Popular RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `mt19937` | 0.758 GB/s | 0.674 GB/s | 142 KHz |

#### 16-bit Variants
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `sfc16` | 0.551 GB/s | 0.750 GB/s | 27,427 KHz |
| `efiix16x384` | 0.341 GB/s | 0.411 GB/s | 363 KHz |

#### 8-bit Variants
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `efiix8x384` | 0.176 GB/s | 0.302 GB/s | 362 KHz |

#### Candidate RNGs (Not recommended, but close)
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `VeryFast32` | 2.892 GB/s | 1.147 GB/s | — |
| `VeryFast64` | 5.799 GB/s | 2.327 GB/s | — |
| `sfc_alternative32` | 2.906 GB/s | 1.263 GB/s | — |
| `sfc_alternative64` | 5.822 GB/s | 2.532 GB/s | — |
| `ranrot_variant32` | 1.981 GB/s | 0.792 GB/s | — |
| `ranrot_variant64` | 3.861 GB/s | 1.597 GB/s | — |

#### 16 & 8-bit Candidate RNGs (Not recommended, but close)
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `VeryFast16` | 0.963 GB/s | 0.431 GB/s | — |
| `ranrot_variant16` | 0.680 GB/s | 0.304 GB/s | — |
| `ranrot_variant8` | 0.390 GB/s | 0.199 GB/s | — |
| `sfc_alternative16` | 1.097 GB/s | 0.448 GB/s | — |
| `xsm16` | 0.524 GB/s | 0.363 GB/s | — |

### Entropy Pool Input Speeds
*(Speeds in **MB/s**)*

| Method | `arbee` | `sha2_based_pool` |
| :--- | :--- | :--- |
| `add_entropy8` | 239.7 MB/s | 67.5 MB/s |
| `add_entropy16` | 487.3 MB/s | 64.0 MB/s |
| `add_entropy32` | 623.8 MB/s | 66.1 MB/s |
| `add_entropy64` | 755.0 MB/s | 66.2 MB/s |
| `add_entropy_N` | 745.2 MB/s | 68.4 MB/s |

---

## Benchmark: 32-bit MSVC

### Environment & Notes
- **CPU:** Intel Core i5 2500
- **Compiler:** Microsoft Visual C++ 2010 Express (10.0.30319.1)
- **OS:** Windows 7 (64-bit)
- **Comments:** Compiled as a 32-bit executable on a 64-bit CPU and OS, which penalizes 64-bit RNG performance. ChaCha used SSE2 instructions in this build, which was not enabled on most other platforms at the time.

### Random Number Generation Speeds

#### Small Fast RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `jsf32` | 2.404 GB/s | 1.115 GB/s | 16,814 KHz |
| `jsf64` | 1.360 GB/s | 1.171 GB/s | 7,533 KHz |
| `sfc32` | 2.771 GB/s | 1.198 GB/s | 23,730 KHz |
| `sfc64` | 1.371 GB/s | 0.987 GB/s | 10,376 KHz |

#### Random Access RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `xsm32` | 1.529 GB/s | 0.923 GB/s | 31,226 KHz |
| `xsm64` | 0.651 GB/s | 0.619 GB/s | 18,373 KHz |

#### Entropy Pooling RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `arbee` | 1.114 GB/s | 0.900 GB/s | 9,494 KHz |
| `sha2_based_pool` | — | 0.030 GB/s | 549 KHz |

#### Cryptographic / High Quality RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `hc256` | 0.651 GB/s | 0.602 GB/s | 68 KHz |
| `trivium` | 0.289 GB/s | 0.204 GB/s | 1,373 KHz |
| `isaac32x256` | 0.594 GB/s | 0.578 GB/s | 396 KHz |
| `isaac64x256` | 0.938 GB/s | 0.844 GB/s | 225 KHz |
| `efiix32x384` | 1.108 GB/s | 0.852 GB/s | 184 KHz |
| `efiix64x384` | 0.854 GB/s | 0.762 GB/s | 161 KHz |
| `chacha` | 0.378 GB/s | 0.367 GB/s | 5,955 KHz |
| `salsa` | 0.166 GB/s | 0.165 GB/s | 2,763 KHz |

#### Popular RNGs
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `mt19937` | 0.989 GB/s | 0.822 GB/s | 127 KHz |

#### 16-bit Variants
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `sfc16` | 0.658 GB/s | 0.632 GB/s | 19,906 KHz |
| `efiix16x384` | 0.394 GB/s | 0.348 GB/s | 184 KHz |

#### 8-bit Variants
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `efiix8x384` | 0.133 GB/s | 0.128 GB/s | 175 KHz |

#### Candidate RNGs (Not recommended, but close)
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `VeryFast32` | 2.537 GB/s | 1.152 GB/s | — |
| `VeryFast64` | 1.463 GB/s | 1.229 GB/s | — |
| `sfc_alternative32` | 2.818 GB/s | 1.257 GB/s | — |
| `sfc_alternative64` | 1.475 GB/s | 0.896 GB/s | — |
| `ranrot_variant32` | 1.368 GB/s | 1.283 GB/s | — |
| `ranrot_variant64` | 1.409 GB/s | 1.251 GB/s | — |

#### 16 & 8-bit Candidate RNGs (Not recommended, but close)
| RNG | Light-Weight | Polymorphic | Seeding Speed |
| :--- | :--- | :--- | :--- |
| `VeryFast16` | 0.837 GB/s | 0.519 GB/s | — |
| `ranrot_variant16` | 0.630 GB/s | 0.585 GB/s | — |
| `ranrot_variant8` | 0.264 GB/s | 0.253 GB/s | — |
| `sfc_alternative16` | 0.658 GB/s | 0.495 GB/s | — |
| `xsm16` | 0.507 GB/s | 0.210 GB/s | — |

### Entropy Pool Input Speeds
*(Speeds in **MB/s**)*

| Method | `arbee` | `sha2_based_pool` |
| :--- | :--- | :--- |
| `add_entropy8` | 79.8 MB/s | 34.3 MB/s |
| `add_entropy16` | 158.2 MB/s | 34.0 MB/s |
| `add_entropy32` | 190.8 MB/s | 34.1 MB/s |
| `add_entropy64` | 228.2 MB/s | 33.4 MB/s |
| `add_entropy_N` | 207.3 MB/s | 34.3 MB/s |
