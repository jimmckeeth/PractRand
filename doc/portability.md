# PractRand Portability

## 1. Portability of Programs Using PractRand

In general, PractRand attempts to produce identical results across all platforms. However, there are some exceptions, particularly with regard to endianness:

* **Non-Recommended RNGs:** Some non-recommended RNGs may produce different results on big-endian platforms than on little-endian platforms.
* **Test Suites & TestBlocks:** There are numerous minor endianness issues in the statistical tests and in how `TestBlock`s for the tests are filled. These tend to cause test results to not be completely identical between big-endian and little-endian platforms, though the differences are minor.

The recommended RNGs themselves are intended to return identical output on both big-endian and little-endian platforms. Those that can act as entropy pools are designed to respond identically to the same input across big-endian and little-endian architectures. However, small errors in endian portability may have crept in.

Furthermore, in many scenarios it can be very difficult for a user application to correctly handle endianness portability independent of the behavior of the library. Thus, users who require strict endian independence should think carefully about what that entails and conduct thorough testing.

---

## 2. Portability of PractRand Itself

### Tested Build Environments

PractRand has been built and tested on:

* **Clang:**
  * Clang 18.1.3 / x86-64 / Mint Linux
  * Clang 19.1.7 / x86-64 / Fedora Linux
* **GCC:**
  * GCC 13.3.0 / x86-64 / Mint Linux
  * GCC 14.3.1 / x86-64 / Fedora Linux
  * GCC 4.6.1 / x86-64 / Ubuntu Linux
  * GCC 4.5.4 / MinGW x64
  * GCC 3.4.5 / MinGW x86 *(TLS was not supported)*
* **Visual C++ (MSVC):**
  * VC 2026 x64 / Windows 11
  * VC 2026 x86 / Windows 11
  * VC 2013 x64 / Windows 7, 8, and 10
  * VC 2013 x86 / Windows 7, 8, and 10
  * VC 2010 x86 / Windows XP and 7

### Language Standards & Configuration

* PractRand uses a subset of **C++11** and/or **C99**.
* When compiling on a different platform, the first step is making a few minor configuration adjustments in `include/PractRand/config.h`.

### Basic Library Requirements

> [!NOTE]
> These requirements typically translate to having a reasonably compliant C++ compiler and standard hardware typical of modern PCs and embedded chips.

* **Addressable Unit:** Memory must be addressed in 8-bit units (`sizeof(Uint8) == 1`).
* **Integer Types:** Support for 8, 16, 32, and 64-bit signed and unsigned integers, including:
  * Unsigned right-shifts on all unsigned integer types.
  * Signed right-shifts on signed integer types.
* **C++ Features:**
  * Basic C++ support, including namespaces and templates.
  * Template support is strongly recommended, though not completely required.
  * C++ exceptions are **not** required.
* **Standard Library:** Standard C/C++ library, including the STL.
* **Endianness:** Must be either standard big-endian or little-endian (no "middle-endian" architectures).
* **Auto-seeding & Entropy:** Auto-seeding and automatic entropy work best out of the box on Windows or \*nix systems (see `platform_specifics.cpp`; support for other platforms can be added there).
* **Floating-Point Arithmetic:** IEEE-compliant floating point is required for `rand_float` and `rand_double` methods. Non-compliant formats can be adjusted in `RNG_internals.h`.

### Multithreading Requirements

When using PractRand from a multithreaded application:

* **Thread Local Storage (TLS):** Compiler-level support for TLS is recommended (autoseeding functions better with TLS, though it is not strictly mandatory).
* **Thread Safety:** The standard C/C++ library implementation must be thread-safe (e.g., thread-safe `malloc`).
* **Unrecognized Platforms:** Multithreading on unrecognized platforms may cause PractRand to leak a single byte of heap allocation per thread created.

### Tools Requirements

The standalone tools (as opposed to the core libraries) may require:

* A command-line environment.
* Either **pthreads** or **Win32** threading (recommended, but not strictly mandatory).

### Known Exceptions

* `Test_calibration` is currently **not portable**. It is an internal tool not intended for end users.
