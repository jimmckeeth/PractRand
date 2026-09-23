# PractRand Test Suite Performance

> See [Tests_results.md](Tests_results.md) for information on how effective the PractRand standard battery of tests is at finding bias in PRNGs.

## Test Battery Performance

Single-threaded testing speed measured on a 3.3 GHz Core i5, using a very fast 32-bit PRNG and compiled with MSVC 2013 Express:

| Test Set | Folding Level | Speed (Seconds / GB) | Notes |
| :--- | :--- | :--- | :--- |
| Normal | No folding | 9.6 s/GB | |
| Normal | Standard folding | 13.5 s/GB | Recommended |
| Normal | Extra folding | 23 s/GB | |
| Expanded | No folding | 25 s/GB | |
| Expanded | Standard folding | 32 s/GB | |
| Expanded | Extra folding | 54 s/GB | |

> [!NOTE]
> These measurements do not count the time required to evaluate test results. Result evaluation time has very little relationship to the amount of data tested. It is usually around 1 to 2 seconds, though under some circumstances (typically involving extra folding) it can be significantly slower.

---

## Individual Test Performance

Single-threaded test execution times measured on the same machine with **no folding**:

- **Baseline (no tests at all / generation only):** 1.5 s/GB

### DC6 Tests

| Test | Speed | Notes |
| :--- | :--- | :--- |
| `DC6-9x1B-1` | 2.7 s/GB | 3.2 s/GB in v0.90 |
| `DC6-6x2B-1` | 2.5 s/GB | |
| `DC6-5x4B-1` | 2.1 s/GB | |
| `DC6-5x8B-101` | 2.0 s/GB | |
| `DC6-4x8B-100` | 2.0 s/GB | |

### FPF Tests

| Test | Speed | Notes |
| :--- | :--- | :--- |
| `FPF-14+6/32` | 2.2 s/GB | |
| `FPF-14+6/16` | 3.6 s/GB | ~5 s/GB in v0.90 |
| `FPF-14+6/8` | 5.8 s/GB | ~8 s/GB in v0.90 |
| `FPF-14+6/4` | 10.5 s/GB | ~14 s/GB in v0.90 |

### Gap Tests

| Test | Speed | Notes |
| :--- | :--- | :--- |
| `Gap16` | 3.1 s/GB | 3.4 s/GB in v0.90 |

### BCFN Tests

| Parameters | Original | Unbalanced | FullFreqs | UFF | MultiThreshold |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `(0,13)` | 9.9 s/GB | 6.8 s/GB | 12.0 s/GB | 9.5 s/GB | *N/A* |
| `(1,13)` | 5.8 s/GB | 4.1 s/GB | 6.9 s/GB | 5.7 s/GB | *N/A* |
| `(2,13)` | 3.4 s/GB | 2.8 s/GB | 4.0 s/GB | 3.5 s/GB | *N/A* |
| `(3,13)` | *N/A* | *N/A* | *N/A* | *N/A* | *N/A* |
