# Use of PractRand Random Number Generators in Multithreaded Programs

The RNGs in PractRand are thread-safe as long as the user does not try to share RNG instances between threads. While a lock could be added to an RNG to allow it to be shared, that is highly undesirable from a performance perspective. The recommended solution is to have a separate RNG object for each thread.

The easiest way to do that is to tell the compiler that you want the RNG instance located in thread-local storage:
- **MSVC**: Prefix the type name with `__declspec(thread)`.
- **GCC**: Prefix the type name with `__thread`.
- *(Modern C++11 and later)*: Use the standard `thread_local` keyword.

---

## RNG Selection for Multithreaded Applications

When picking an RNG for a multithreaded application, it may be wise to go slightly higher on statistical quality and state-space size than you would for an equivalent single-threaded application:
- See [RNG_engines.md](RNG_engines.md) for lists of recommended RNGs and their quality ratings.
- See [RNG_parallel.md](RNG_parallel.md) for more details on when high-quality RNGs are needed to avoid inter-RNG correlation issues.

---

## Seeding Strategies

### 1. Simple Seeding (Auto-Seeding)

PractRand's auto-seeding mechanism is thread-safe, so you can initialize each RNG with `PractRand::SEED_AUTO`.

However, in order for PractRand auto-seeding to be fully thread-safe, some guidelines must be followed:
1. Call `PractRand::initialize_PractRand()` at the start of `main()`.
2. Do **not** permit multithreading to occur prior to the start of `main()`.

> [!NOTE]
> It is fine to use RNGs prior to `main()`, even auto-seeded RNGs, as long as only one thread exists at that time. Once `initialize_PractRand()` has run, everything else is fully thread-safe.

#### Code Examples (Simple)

**MSVC:**
```cpp
// In the .h file:
extern __declspec(thread) PractRand::RNGs::Polymorphic::hc256 rng;

// In the .cpp file:
__declspec(thread) PractRand::RNGs::Polymorphic::hc256 rng(PractRand::SEED_AUTO);
```

**GCC:**
```cpp
// In the .h file:
extern __thread PractRand::RNGs::Polymorphic::hc256 rng;

// In the .cpp file:
__thread PractRand::RNGs::Polymorphic::hc256 rng(PractRand::SEED_AUTO);
```

---

### 2. Advanced Seeding (Controlled Entropy Pool)

For programs that need more control over the seeding process:

1. **Pick an RNG and declare a thread-local instance**: Declare a global instance in thread-local storage as before (initialized with `SEED_NONE` or `SEED_AUTO`).
2. **Declare a global entropy pool and mutex**: Declare a polymorphic entropy pool and a mutex (e.g. `CRITICAL_SECTION` on Win32, `pthread_mutex_t` on POSIX). These should **not** be in thread-local storage; they must be ordinary global variables.
3. **Initialize entropy before multithreading**: Before the RNG is used and before any other threads are launched, initialize the entropy pool with one or more calls to `add_entropy` methods. The standard method is `add_entropy_automatically()` (reliable on Windows and *nix). Flush buffers and seed the main thread's RNG from the entropy pool.
4. **Seed worker threads under lock**: Each time another thread is created, lock the mutex, use the entropy pool to seed the thread-local RNG instance, and unlock the mutex.

#### Code Examples (Advanced)

**MSVC (Win32):**
```cpp
// In the .h file:
extern __declspec(thread) PractRand::RNGs::Polymorphic::hc256 rng;
extern CRITICAL_SECTION thread_startup_lock;
extern PractRand::RNGs::Polymorphic::sha2_based_pool entropy_pool;

// In the .cpp file:
__declspec(thread) PractRand::RNGs::Polymorphic::hc256 rng(PractRand::SEED_AUTO);
CRITICAL_SECTION thread_startup_lock;
PractRand::RNGs::Polymorphic::sha2_based_pool entropy_pool;

// Inside main(): (or elsewhere, as long as it runs prior to multithreading or RNG use)
InitializeCriticalSection(&thread_startup_lock);
entropy_pool.add_entropy_automatically();
// Additional entropy_pool.add_entropy* calls go here if needed
entropy_pool.flush_buffers();
rng.seed(entropy_pool);

// Inside per-thread initialization function:
EnterCriticalSection(&thread_startup_lock);
rng.seed(entropy_pool);
LeaveCriticalSection(&thread_startup_lock);
```

**GCC (Linux / POSIX):**
```cpp
// In the .h file:
extern __thread PractRand::RNGs::Polymorphic::hc256 rng;
extern pthread_mutex_t thread_startup_lock;
extern PractRand::RNGs::Polymorphic::sha2_based_pool entropy_pool;

// In the .cpp file:
__thread PractRand::RNGs::Polymorphic::hc256 rng(PractRand::SEED_AUTO);
pthread_mutex_t thread_startup_lock;
PractRand::RNGs::Polymorphic::sha2_based_pool entropy_pool;

// Inside main(): (or elsewhere, as long as it runs prior to multithreading or RNG use)
pthread_mutex_init(&thread_startup_lock, NULL);
entropy_pool.add_entropy_automatically();
// Additional entropy_pool.add_entropy* calls go here if needed
entropy_pool.flush_buffers();
rng.seed(entropy_pool);

// Inside per-thread initialization function:
pthread_mutex_lock(&thread_startup_lock);
rng.seed(entropy_pool);
pthread_mutex_unlock(&thread_startup_lock);
```
