# PractRand Test Interfaces and Usage

There are several different interfaces that can be used to access PractRand's statistical tests for pseudo-random number generators. Internally, each higher-level interface is implemented using the next lower-level interface.

---

## 1. High-Level Interface: Command-Line Tool (`RNG_test`)

The highest-level interface (and thus the simplest for general use) is the **`RNG_test`** tool, which can be controlled from the command line.

- See Section 1 of [Tests_overview](Tests_overview.md) and the relevant section of [tools](tools.md) for detailed usage instructions.

---

## 2. Intermediate Interface: Test Batteries and Managers

The intermediate interface is built around **test batteries** and **test managers**:

### Test Batteries
- You can obtain a battery of tests by calling the PractRand library's test battery functions in namespace `PractRand::Tests::Batteries` (see `include/PractRand/test_batteries.h`).

### Test Managers
- The `TestManager` class is not technically part of the core PractRand library at this time, but its source code is available in `tools/TestManager.h` (interface at the top, implementation below).
  > [!NOTE]
  > `tools/TestManager.h` is neither self-contained (requires several prerequisite headers) nor isolated (wrap the `#include` statement if you want it placed inside a namespace).
- A multithreaded subclass that runs the RNG and each test in separate threads is provided in `tools/MultithreadedTestManager.h`.

### Examples and Customization
- The interface to `TestManager` is documented within `tools/TestManager.h`.
- For a complete working example using this interface, see `tools/RNG_test.cpp`.
- To quickly test a custom RNG, you can modify `tools/RNG_test.cpp` to include your generator and recompile. The placeholder `DummyRNG` near the top of `tools/RNG_test.cpp` was designed for this purpose.

---

## 3. Low-Level Interface: Direct Test API

The low-level interface works directly with `Test` objects and arrays of kilobyte-sized blocks of random bits.

### Relevant Source Files
- **Base Class:** The raw testing interface (the base class from which all PractRand tests for raw datastreams are derived) is declared in `include/PractRand/tests.h`.
- **Sample Code:** See tests in action in `tools/RNG_test.cpp` and `tools/TestManager.h` (where most testing orchestration takes place).
- **Batteries Implementation:** See how standard test sets are instantiated in `src/test_batteries.cpp`.

---

### Step-by-Step Testing Process

Testing a stream of data directly using the low-level API proceeds as follows:

1. **Construct the Test:**
   Call the test constructor with the desired parameters. Parameters cannot be changed after construction (unless a new test instance is created).

2. **Initialize (`init`):**
   Call the test's `init()` method, passing a known-good polymorphic RNG as a parameter.
   > [!NOTE]
   > Most tests do not actually use the known-good RNG, but a few rare tests do. Because all tests in PractRand adhere to a uniform API, every test must be supplied with a known-good RNG.

3. **Allocate Memory Blocks:**
   Create a block of memory to hold the data to be tested (data does not need to all fit in memory at once).
   - The memory block should be an array of type `PractRand::Tests::TestBlock`.
   - Each `TestBlock` is 1 kilobyte (1 KB) in size and should be filled with random bits.
   - `TestBlock` provides a method for efficiently filling itself using a polymorphic RNG. If reading from an alternate data source, populate the blocks manually.

4. **Run Test on Data Blocks (`test_blocks`):**
   Pass the memory block to the test using the `test_blocks()` method.
   - Parameters: a pointer to the array of `TestBlock`s and the number of blocks in that array.
   - **Limit:** Generally, pass no more than 0.5 GB to `test_blocks()` in a single call, as some tests malfunction when given 1 GB or more in a single call.

5. **(Optional) Supply Additional Data Blocks:**
   Create more arrays of `TestBlock`s filled with random bits and pass them to `test_blocks()`.
   - **Prefix Requirement:** The tail end of the previous array of blocks must be prefixed to the beginning of each new array.
   - The pointer passed to `test_blocks()` should point to the beginning of the *new* data in the array.
   - Determine the maximum number of blocks required to prefix by calling `get_blocks_to_repeat()` on the test.
   - *Tip:* See `tools/TestManager.h` for an encapsulation of this requirement. As with the known-good RNG parameter, only a few tests require repeated blocks, but all callers must comply to support a unified interface.

6. **Retrieve Results (`get_result`):**
   Call the test's `get_result()` method passing an empty `std::vector`.
   - The vector is populated with `TestResult` objects—one per subtest that generated a usable result.
   - Basic result inspection: refer to the `name` field and call `get_pvalue()`.
   - `TestResult` also provides metadata regarding p-value quality/importance and supports reporting p-values in transformed coordinates for higher resolution near 0 or 1.

7. **(Optional) Continue Testing for Interim Results:**
   Repeat the procedure starting from step 5. The test retains prior data as if `get_result()` had not been called, allowing interim results during long tests without compromising the ongoing evaluation.

8. **Deinitialize (`deinit`):**
   Call `deinit()`.
   - Resets test state.
   - In some (but not all) cases, this frees most memory used by the test.
   - Test parameters remain intact until destruction.

9. **(Optional) Restart Test:**
   Repeat from step 2 (`init()`).

10. **Destruct:**
    Destruct the test object. All allocated memory used by the test is released.
