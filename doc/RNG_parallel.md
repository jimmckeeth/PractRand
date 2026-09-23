# Parallel Random Number Generation Issues

Parallel RNG issues arise when multiple RNG instances need to produce uncorrelated output. This is most often encountered in distributed computing applications, but the same basic issue can arise between separate runs of a single program on the same computer.

Typical RNG-using applications do not care much if different RNG instances are slightly correlated with each other. However, some applications care a great deal.

If you need to avoid correlation between different instances of your RNG, you need to use an RNG algorithm with a large state space and relatively little inter-state correlation.

PractRand RNG quality ratings already take such factors into account. You can use the tables below to determine the minimum quality rating required for your parallel random number generation scenario. Recommended RNGs with their quality ratings can be found in [RNG_engines.md](RNG_engines.md).

## Usage Scenarios and Recommendations

| Scenario Name | Total Seedings | Total Numbers Generated | Minimum Quality Rating |
| :--- | :--- | :--- | :--- |
| **Light** | < 2<sup>5</sup> | < 2<sup>45</sup> | 2 star |
| **Moderate** | < 2<sup>20</sup> | < 2<sup>60</sup> | 3 star |
| **Heavy** | < 2<sup>45</sup> | < 2<sup>75</sup> | 4 star |
| **Extreme** | < 2<sup>80</sup> | < 2<sup>110</sup> | 5 star |

> [!NOTE]
> "Total numbers generated" refers to the total number of RNG outputs used across all instances of the RNG combined. For example, a million computers each using a quadrillion random numbers would amount to 2<sup>70</sup> numbers generated. If each of those million computers seeded an RNG on average a thousand times during the course of producing those quadrillion numbers, the total seedings would be 2<sup>30</sup>.

## Instance Seeding

The seed for each RNG instance must be distinct from all other instances' seeds:

- **PractRand Autoseeding:** Normally sufficient to provide that kind of uniqueness, at least on PRNGs where seeding is liable to be the limiting factor.
  - On unrecognized platforms (platforms that are neither Windows nor \*nix) or exceptional failures (such as `fopen` failing on `/dev/urandom`), autoseeding may provide lower quality seeding.
  - You can check whether autoseeding succeeded by inspecting the return value of `PractRand::initialize_PractRand()`. It returns `true` if autoseeding obtained enough entropy for high-quality seeding, or `false` if it failed to find sufficient entropy and will produce low-quality seeds.
- **Manual Seeding:** If not using autoseeding, you must employ a scheme for ensuring unique keys across your domain:
  - **Probabilistic schemes:** Requires large seeds and substantial seeding entropy.
  - **Globally Unique Identifiers (GUIDs):** Also tends to require large seeds, though not quite as large as probabilistic schemes.

## Multithreading

Multithreaded RNG usage is a special case of parallel random number generation. For issues specific to multithreaded programs generating random numbers, see [RNG_multithreading.md](RNG_multithreading.md).
