# PractRand Tools

PractRand includes several programs that use the library to perform useful tasks:

## 1. RNG_output

A command-line driven program that prints raw random bytes to standard output (`stdout`).

* **Purpose**: Designed for command-line and Unix-like environments where output can be piped to another process or redirected to a file.
* **Arguments**: Takes an RNG name and the number of bytes to generate.
* **Seed Parameter**: Accepts an optional third parameter specifying a 64-bit seed in hexadecimal. If omitted, a seed is selected at random.
* **Continuous Streaming**: The byte count can be specified as `inf` to emit random bytes indefinitely, which is ideal when piping to a consumer of undetermined length.

### Example
Generate approximately 1.1 terabytes using the RNG named `jsf32` with a randomly generated seed:
```bash
./RNG_output jsf32 1234567890000
```

---

## 2. RNG_test

A command-line driven program that applies PractRand statistical tests to an RNG and reports anomalous results.

* **Configuration**: The definition of "anomalous", the choice of RNG, and the specific test battery can be configured from the command line.
* **Standard Input**: External RNGs and raw data files can be tested by piping data into standard input (`stdin` / `stdin32`).
* **Seeding & Entropy Testing**: Features alternate modes to evaluate seeding or entropy pooling quality rather than simple output. It generates a random sequence of seeds with minimal Hamming distance between them, seeds the target RNG, generates 64 bits of output per seed, and concatenates the output into a continuous stream for testing.

### Sample Command Lines

#### Linux / Unix
* **Testing a PractRand RNG**:
  ```bash
  ./RNG_test jsf32
  ```
* **Testing a file**:
  ```bash
  cat random_data_file | ./RNG_test stdin
  ```
* **Testing data piped in from `RNG_output`**:
  ```bash
  ./RNG_output jsf32 inf | ./RNG_test stdin32
  ```

#### Windows
* **Testing a PractRand RNG**:
  ```cmd
  RNG_test jsf32
  ```
* **Testing a file**:
  ```cmd
  RNG_test stdin < random_data_file
  ```
* **Testing data piped in from `RNG_output`**:
  ```cmd
  RNG_output jsf32 inf | RNG_test stdin32
  ```

---

## 3. RNG_benchmark

Measures the generation speed of all recommended RNGs and candidate RNGs.

* Candidate RNGs may receive an unfair speed advantage depending on linker optimization, as they share the same compilation unit.
* Non-recommended RNGs are unoptimized and excluded from benchmarks.

---

## 4. Test_calibration

Contains various routines and code snippets used during the calibration of the statistical tests.

* Not coherently organized and not intended for end users.
* Provided as reference and partial documentation of the statistical calibration methodology.
