# GB (Z80) Emulator
Creating a GameBoy Emulator (similar CPU to Z80) from scratch in C with the ultimate goal of running Pokemon Red and eventually full emulation. Will flesh out README as progress comes along.

## Dependencies

To build and run the test suite, you will need the following dependencies:

- CMake (version 3.0 or higher)
- CUnit (version 2.1-3)

## Getting Started

To build:

```bash
$cmake ..
$make
```
Running the test suite:
```bash
$cd build
$./z80_tests
```
---
**NOTE**

Replace the rom filepath in test_cartridge.c with the filepath of the pokemon red rom on your local machine, and/or comment out add_cartridge_tests in test_suite.c

---

