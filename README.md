
# Project Makefile

This project includes a Makefile that helps automate the compilation, testing, and debugging of multiple C++ projects.

## Table of Contents
1. [Targets](#targets)
2. [Compilation Options](#compilation-options)
3. [Valgrind Memory Analysis](#valgrind-memory-analysis)
4. [Code Coverage](#code-coverage)
5. [Cleaning Up](#cleaning-up)
6. [Usage Example](#usage-example)

---

## Targets

This Makefile defines several targets:

- **all**: Builds all the executables: `LFServer`, `PServer`, and `tests`.
```bash
make all
```

- **`LFServer`**: Compiles the leader-follower server executable, using the source files from `Network/src`, `Algorithms/src`, `DataStructures/src`, and `DesignPatterns/src`, along with `LfserverDemo.cpp`.
  ```bash
  make LFServer
  ```

- **`PServer`**: Compiles the pipeline server executable, using similar source files but with `PipelineServerDemo.cpp`.
  ```bash
  make PServer
  ```

- **`tests`**: Compiles the test executable, using `TestsMain.cpp` and `Tests/src/Tests.cpp`.
  ```bash
  make tests
  ```

---

## Compilation Options

The Makefile uses the following compilation options:

- **CXX**: The compiler used is `g++`.
  
- **CXXFLAGS**: Common compiler flags include:
  - `-Wall`: Enables all compiler warnings.
  - `-g`: Includes debugging information.
  - `-std=c++20`: Uses the C++20 standard.
  - `-pthread`: Enables multithreading with pthreads.
  - `-fprofile-arcs -ftest-coverage`: Enables code coverage generation.

Run the commands to build the executables:
```bash
make LFServer
make PServer
make tests
```

---

## Valgrind Memory Analysis

Valgrind is used to analyze memory usage, detect race conditions, and perform profiling:

- **Memory Leak Check**:
  Run Valgrind’s `memcheck` tool to detect memory leaks and invalid memory accesses:
  ```bash
  make valgrind-memcheck_LF
  make valgrind-memcheck_P
  ```

- **Race Condition Check**:
  Use Valgrind’s `helgrind` tool to detect race conditions:
  ```bash
  make valgrind-helgrind_LF
  make valgrind-helgrind_P
  ```

- **Performance Profiling**:
  Run Valgrind’s `callgrind` tool to collect profiling data for performance analysis:
  ```bash
  make valgrind-callgrind_LF
  make valgrind-callgrind_P
  ```

Use `kcachegrind` to visualize the callgrind output for performance analysis.

---

## Code Coverage

The Makefile supports code coverage analysis using `gcov` and `lcov`. To generate a coverage report:

1. Compile the project with coverage flags and run tests.
2. Generate a coverage report:
   ```bash
   make coverage
   ```

The code coverage report will be saved as `gcov_report.txt`, and an HTML version will be generated in the `coverage_html/` directory.

---

## Cleaning Up

The `clean` target removes compiled object files, executables, and coverage data:
```bash
make clean
```

This also removes `.gcda` and `.gcno` files from the source directories, ensuring a fresh build.

---

## Usage Example

To compile the `LFServer`, run a memory check, and generate a coverage report:

```bash
make LFServer
make valgrind-memcheck_LF
make coverage
```

To visualize performance with `kcachegrind` after running `callgrind`:

```bash
make valgrind-callgrind_LF
kcachegrind callgrind.out.<pid>
```

This Makefile provides an efficient development workflow with debugging, testing, and performance analysis tools.
