# Boothbay Scale Balancer

A C++ solution for the Boothbay “balance scales” problem. Given a hierarchy of scales and weights, the program computes the minimal extra weight to add on each pan so that every scale in the structure is exactly balanced.

---

## Repository Structure

```
.
├── input/
│   ├── input1.txt
│   ├── input2.txt
│   └── …  
├── Makefile
├── README.md
└── solution.cpp
```

* **input/**
  Contains sample test files (`*.txt`), each with comma-separated lines of the form:

  ```
  ScaleName, LeftChild, RightChild
  ```

  where each child is either a positive integer mass or another scale name.

* **solution.cpp**
  The C++11/17 implementation that:

  1. Parses and validates input lines.
  2. Builds an in-memory tree of scales and masses.
  3. Performs a bottom-up (post-order) DFS to compute and record the extra weight needed on each pan.
  4. Prints each scale (alphabetically) as:

     ```
     ScaleName, extraOnLeft, extraOnRight
     ```

* **Makefile**
  Build and clean rules for both Unix-style shells and Windows (`cmd`).

---

## Prerequisites

* A modern C++ compiler supporting C++17 (e.g. `g++`, `clang++`).
* `make` (GNU Make).
* On Windows, tests can be run via `cmd.exe` redirection or by piping through PowerShell.

---

## Build

```bash
make
```

This compiles `solution.cpp` with `-O3 -std=c++17` and produces the executable `solution` (or `solution.exe` on Windows).

---

## Clean

```bash
make clean
```

Removes the compiled executable (and object files on Windows if present).

---

## Run

### Single Input File

#### Unix / WSL / Git Bash

```bash
./solution < input/input3.txt
```

#### Windows cmd.exe

```bat
solution.exe < input\input3.txt
```

#### Windows PowerShell

```powershell
Get-Content input\input3.txt | .\solution.exe
```

---

## Input Format

* Lines starting with `#` or blank lines are ignored.
* Each non-comment line must have **exactly** two commas, splitting into:

  1. **ScaleName** (must start with a letter, alphanumeric).
  2. **LeftChild** (either another ScaleName or a non-negative integer).
  3. **RightChild** (same rules as LeftChild).
* Whitespace around tokens is trimmed.
* On invalid input (malformed lines, duplicate roots, cycles), the program writes an error to `stderr` and exits with code `1`.

---

## Output Format

For each scale in **alphabetical** order, one line:

```
ScaleName,extraOnLeft,extraOnRight
```

* `extraOnLeft`  = minimal integer weight to add to the left pan.
* `extraOnRight` = minimal integer weight to add to the right pan.
* Exactly one of `extraOnLeft` or `extraOnRight` will be nonzero (or both zero if already balanced).

---

## Examples

### Input

```
# Example input
B1 , 10, B2 
B2, B3, 4
B3, 7, 8
```

### Output

```
B1,25,0
B2,0,13
B3,1,0
---