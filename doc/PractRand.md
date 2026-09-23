# PractRand (Practically Random)

**Version 0.95**

PractRand (Practically Random) is a C++ library of pseudo-random number generators (PRNGs, or just RNGs) and statistical tests for RNGs.

---

## 1. Random Number Generation with PractRand

Most development platforms come with random number generators that have major statistical flaws, are a bit slow, and/or have inconvenient interfaces. PractRand can help.

### Features

#### A. Very convenient interface for practical use, not just research
* **A1:** Abstraction layer hides details of the RNG algorithm's native output format, as users have better things to worry about.
* **A2:** Natively supports uniform integer and floating-point distributions, compatible with the more complex distributions offered by Boost / C++0x TR1.
* **A3:** Common interface available for a variety of RNG algorithms, making switching underlying algorithms trivial.
* **A4:** Automatic seeding, or seeding from any type(s) you want, with a simple seeding interface for the common cases and powerful seeding helpers for exotic cases.

#### B. A variety of very high quality RNG algorithms
* **B1:** Lists of recommended RNG algorithms and their strengths, to make it easy to find one appropriate for specific circumstances quickly.
* **B2:** RNGs suitable for a wide variety of needs—cryptographically secure RNGs, random access RNGs, RNGs that can be implemented with less than 1000 gates, etc.
* **B3:** Many of the included RNGs are appropriate for use in parallel computing and multithreaded applications, accompanied by documentation to help.
* **B4:** All recommended RNGs are unencumbered. See [license](license.md) for details.

### Other libraries including multiple RNG algorithms with a common interface
* [Boost / C++0x TR1](http://www.boost.org/)
* [TestU01](http://www.iro.umontreal.ca/~simardr/testu01/tu01.html)
* [hasard](http://bytebucket.org/haypo/hasard/wiki/Home)
* [GSL](http://www.gnu.org/software/gsl/)

---

## 2. Statistical Testing of RNGs with PractRand

### Features

* **A. Standard Battery of Tests:** Includes a standard battery of tests, in the tradition of Diehard. Many competitors include only raw tests with no obvious means for someone unfamiliar with them to pick parameterizations, or a default test set that is simply one parameterization of each test regardless of the relative merits of the tests involved.
* **B. Fast and Broad Bias Detection:** Standard battery of tests can detect bias in a wide variety of RNGs quickly. In testing, it can find bias in a much wider variety of RNGs than any other suite of statistical tests, and on average needs less time than other suites.
* **C. Flexible Integration and Speed:** Test users concerned primarily with ease of integrating their RNGs can pipe data into command-line testing tools with minimal effort. Users interested in speed and versatility can use the tests as legally unencumbered source code and static libraries so that data can be passed directly to them—bypassing the speed limitations of piping data and allowing custom test invocations. Some competitors use similar schemes, but many do only one or the other, or choose worse options: taking input only from files (worse speed and versatility, plus size limitations) or only supporting testing on a few predefined RNGs.
* **D. No Realistic Maximum Sequence Length:** Competing software packages generally have interface issues, bugs, or severe scalability issues that make testing very long sequences impossible or meaningless. PractRand has been tested on sequence lengths over 500 terabytes and is expected to work on sequence lengths up to a few exabytes.
* **E. More Original Tests:** Competing software packages tend to mainly use tests identical or nearly identical to ones that appeared in the literature decades ago, with only a few unusual tests. While there is nothing wrong with that, testing with at least two test suites yields less redundancy (and better breadth) if one of the suites is PractRand.
* **F. Preliminary Results on Demand:** PractRand's testing interface allows preliminary results to be requested at any time without having to restart testing from the beginning. Most competitors require that results be evaluated just once at the end of a test. This is an important feature when testing an RNG using dynamic test sequence lengths (progressively longer until the RNG fails a test).
* **G. Multithreaded CLI Tools:** The command-line testing tools are multithreaded to allow higher performance on multicore CPUs. Very few competitors support multithreading like that.
* **H. Exotic Testing Modes:** Exotic testing modes to check things like inter-seed correlation.

### Limitations and Trade-offs
* **Sample Size Requirement:** The main con is that PractRand requires more random bits than most comparable test suites, making it inappropriate for very slow PRNGs.
* **Orthogonality vs. Diagnostic Redundancy:** The more orthogonal test set used by PractRand, while good for speed, means that information about *which* tests were failed may be less informative than similar information produced by a competitor using a more redundant set of tests.

---

### Other packages for testing PRNGs

> **Evaluation Criteria:**
> - **Quality:** Ability to distinguish good RNGs from bad RNGs.
> - **Presentation:** Showing test results in ways that are easy to understand and useful.

#### gjrand
- **URL:** <http://gjrand.sourceforge.net/>
- **Quality:** Very good
- **Presentation:** Decent
- **Open-source:** Viral (GPL)
- **Multithreaded:** Yes
- **Notes:** Pretty obscure, but works really well. Linux build process is straightforward; never tested on non-Unix environments. Documentation seems a bit lacking.

#### TestU01
- **URL:** <http://www.iro.umontreal.ca/~simardr/testu01/tu01.html>
- **Quality:** Good
- **Presentation:** Decent
- **Open Source:** Non-commercial only
- **Multithreaded:** No
- **Notes:** Popular and quite decent. Not quite as good as PractRand or gjrand IMHO, but still pretty good. Rather difficult to build on Windows.

#### RaBiGeTe
- **URL:** <http://cristianopi.altervista.org/RaBiGeTe_MT/>
- **Quality:** Marginal
- **Presentation:** Overly complex
- **Open Source:** Recent versions closed source, older versions viral (GPL)
- **Multithreaded:** Yes
- **Notes:** The fourth best testing package after PractRand, gjrand, and TestU01. The range of biases it can detect is a bit limited—it generally works best on LCGs. It is more efficient on a per-bit basis and comes with a GUI that can help visualize the distribution of results from multiple samples if desired.

#### Dieharder
- **URL:** <http://www.phy.duke.edu/~rgb/General/dieharder.php>
- **Quality:** Bad
- **Presentation:** Decent
- **Open Source:** Viral (GPL)
- **Multithreaded:** No? (don't remember)
- **Notes:** Pretty bad at the moment, but some parts show potential to be very good with a little more work. The command-line interface can be very convenient for scripting. Unable to build on Win32. The default test set is particularly awful, and several tests have false-positive issues.

#### NIST STS
- **URL:** <http://csrc.nist.gov/groups/ST/toolkit/rng/index.html>
- **Quality:** Bad
- **Presentation:** Bad
- **Open Source:** Public domain
- **Multithreaded:** No
- **Notes:** Widely used, but not very good.

#### Diehard
- **URL:** <http://www.stat.fsu.edu/pub/diehard/>
- **Quality:** Bad
- **Presentation:** Bad
- **Open Source:** Yes (not sure what the license is)
- **Multithreaded:** No
- **Notes:** Avoid this; it is only of historical interest. Dieharder (see above) was named after this.

#### ENT
- **URL:** <http://www.fourmilab.ch/random/>
- **Quality:** Bad
- **Presentation:** Mediocre
- **Open Source:** Yes (not sure what the license is)
- **Multithreaded:** No?
- **Notes:** A very weak test.

#### BSI test suite
- **URL:** *(Unknown / no working download link found)*
- **Quality:** ???
- **Presentation:** ???
- **Open Source:** ???
- **Notes:** Could not find a working download link.
