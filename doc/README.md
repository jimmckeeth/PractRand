# PractRand Documentation

This directory contains the documentation for the PractRand (Practically Random) C++ library. The documentation covers installation, random number generation (RNG) usage and algorithms, and the statistical testing suites.

## General Information

* **[PractRand.md](PractRand.md)**: Main overview of the C++ library, providing high-quality PRNGs alongside a fast, multithreaded statistical testing battery.
* **[installation.md](installation.md)**: Explains how to configure, build, and link PractRand as a static C++ library and compile its command-line testing tools across platforms.
* **[license.md](license.md)**: Outlines the CC0 1.0 Universal public domain dedication for PractRand and core tools, and the intellectual property status for included RNGs.
* **[portability.md](portability.md)**: Details the cross-platform portability of PractRand, specifying compiler requirements (C++11/C99 subsets, 8-bit byte addressing) and endianness considerations.
* **[to_do.md](to_do.md)**: Outlines the development roadmap, pending tasks, and planned improvements across PractRand.
* **[versions.md](versions.md)**: Records the release notes and changelog from the initial release through the latest versions.

## Random Number Generation (RNG)

* **[RNG_engines.md](RNG_engines.md)**: Provides recommendations, comparison charts, evaluation criteria, and in-depth descriptions for the PRNG algorithms included in PractRand.
* **[RNG_entropy_pools.md](RNG_entropy_pools.md)**: Describes entropy pools as a hybrid between PRNGs and hash functions, outlining interface methods and buffer flushing semantics.
* **[RNG_interface_variations.md](RNG_interface_variations.md)**: Details PractRand's distinct RNG interface variations (Polymorphic, Light-weight, Raw, Entropy Pools, and testing RNGs).
* **[RNG_multithreading.md](RNG_multithreading.md)**: Details best practices for utilizing PractRand random number generators in multithreaded environments.
* **[RNG_parallel.md](RNG_parallel.md)**: Discusses considerations for parallel and distributed random number generation, providing recommended RNG quality ratings based on scale.
* **[RNG_speed.md](RNG_speed.md)**: Provides practical performance guidelines and benchmark results comparing generation throughput and seeding speeds.
* **[RNG_usage.md](RNG_usage.md)**: A practical guide on integrating PractRand PRNGs, covering algorithm selection, threading, and various seeding strategies.

## Statistical Testing

* **[Tests_overview.md](Tests_overview.md)**: A comprehensive guide to empirical random number testing with PractRand, explaining how to interpret evaluations and calibrated p-values.
* **[Tests_batteries.md](Tests_batteries.md)**: Explains how PractRand organizes its RNG test batteries in a 2x3 grid spanning algorithm sets and folding levels.
* **[Tests_engines.md](Tests_engines.md)**: Details individual statistical test engines (BCFN, DC6, Gap16, FPF, BRank) and test transforms.
* **[Tests_performance.md](Tests_performance.md)**: Provides benchmark throughput figures for standard and expanded test batteries across various folding levels.
* **[Tests_results.md](Tests_results.md)**: Presents an empirical comparison and benchmark of major PRNG statistical test batteries (PractRand, gjrand, TestU01, etc.) by measuring their sensitivity.
* **[Tests_usage.md](Tests_usage.md)**: Details the three programming interfaces available for PractRand's statistical tests (`RNG_test`, test batteries, and raw APIs).

## Tools

* **[tools.md](tools.md)**: Documents the command-line utility tools bundled with PractRand, including `RNG_output`, `RNG_test`, and `RNG_benchmark`.
