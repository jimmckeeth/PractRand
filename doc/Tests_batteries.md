# PractRand Test Batteries

PractRand defines several batteries of tests for RNGs. These are represented as `PractRand::Tests::ListOfTests` and instantiated via factory functions in the `PractRand::Tests::Batteries` namespace that return `ListOfTests` objects.

> [!IMPORTANT]
> When finished with a `ListOfTests` returned by one of these functions, call `PractRand::Tests::Batteries::destruct_tests` on it to deallocate the resources used by the tests.

---

## Battery Organization

The predefined batteries of tests in PractRand are organized in a 2×3 grid across two axes:
1. **Algorithm Set**: Normal set vs. Expanded set of algorithms.
2. **Folding Degree**: Level of "folding" applied, where PractRand concentrates extra testing effort on the lowest bits of the PRNG output.

### Battery Selection Grid

All functions reside in the `PractRand::Tests::Batteries` namespace:

| Folding Option | Normal Test Set *(recommended)* | Expanded Test Set |
| :--- | :--- | :--- |
| **No Foldings** | `get_core_tests()` | `get_expanded_core_tests()` |
| **Standard Foldings** *(recommended)* | `get_standard_tests()` | `get_expanded_standard_tests()` |
| **Extra Foldings** | `get_folded_tests()` | `get_expanded_folded_tests()` |

---

## Test Sets

- **Normal Test Set** *(recommended)*:  
  Optimized for maximum effectiveness per CPU cycle.

- **Expanded Test Set**:  
  Reduces per-cycle effectiveness in order to improve per-bit effectiveness.

---

## Folding Options

- **No Folding**:  
  Runs the tests directly without foldings. Suitable when confident that no low-bit issues could occur (e.g., algorithms provably symmetric across bit positions, though folding can still occasionally help).

- **Standard Foldings** *(recommended)*:  
  Concentrates extra testing on the lowest bit positions by running duplicate test sets on additional datastreams formed by keeping only the lowest bit(s) of each integer.
  - **Performance Impact**: Slows down test execution by ~30% and evaluation by ~150%.
  - **Metadata Dependency**: Implemented as closely related folding sets tailored to specific PRNG output word sizes (8-bit, 16-bit, 32-bit, 64-bit, and unidentified). The active folding set is chosen based on PRNG metadata; missing or incorrect metadata may lead to suboptimal targeting.

- **Extra Foldings**:  
  Similar to Standard Foldings, but does not rely on PRNG metadata to infer word size. Instead, it creates many extra datastreams to cover all common output formats simultaneously.
  - **Memory Usage**: May consume large amounts of memory on longer data streams (often exceeding 32-bit address spaces; a 64-bit build is recommended).
