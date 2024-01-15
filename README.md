# GB (Z80) Emulator
Creating a GameBoy Emulator (similar CPU to Z80) from scratch in C with the ultimate goal of running Pokemon Red and eventually full emulation. Will flesh out README as progress comes along.
Development currently halted!!
## Dependencies

To build and run the test suite, you will need the following dependencies:

- CMake (version 3.0 or higher)
- CUnit (version 2.1-3)

## Getting Started

To install dependencies on Linux:

```bash
$sudo apt-get update
$sudo apt-get install -y cmake libsdl2-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev libcunit1 libcunit1-doc libcunit1-dev
```

To build:

```bash
$cmake
$make
```
Running the test suite:
```bash
$cd build
$./z80_tests
```

