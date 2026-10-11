# Project C - Grand EII
[![CI](https://github.com/giangvtr/GrandEII/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/giangvtr/GrandEII/actions/workflows/ci.yml)
*This repository is forked from the original Gitlab repository*

Developed by **BM, DM, EA, HB, and VG**.   

---

## About the Project
This project aims to create a **light management software using infrared (IR) commands**.  
Users can add IR-controllable lights by capturing codes from their original remote controls and mapping actions to them.  

The motivation came from:
- The inconvenience of using separate remotes for each light (loss, batteries, ergonomics, lack of synchronization).  
- Inspiration from **GrandMA**, a professional lighting control software for stage and live performances.  

To support a wide variety of lights, the software provides **color and intensity control**, ensuring users can achieve their desired effects.  

**Prototype hardware**: Arduino MEGA 2560 with IR emitters/receivers.  

---

## For End Users
### Requirements
- Linux system  
- Install `gtk3` with `GLib 2.80`  

### Run the Program
```bash
cd GRAND_EII/src/build
./GRANDEII_PC
```
### Arduino Setup
- Install Arduino IDE
- Upload GRANDEII_ARDUINO.ino to your Arduino board

---

## For developpers
### Dependencies
- Linux system
- gtk3 with GLib 2.80
- gcov (test coverage)
- valgrind (memory check)
- lcov (HTML coverage reports, optional)

### Branch Structure
- communication → Arduino ↔ PC communication
- view → GUI (GTK-based)
- model → Data structures & business logic
- unit-tests → Unit tests (src/unit_tests/)

### Clone a Branch
`git clone --single-branch --branch <branch> <repo-url>`

### Build & Run
```
cd GRAND_EII/src
mkdir build && cd build
cmake ..
make

./UNIT_TESTS   # run unit tests
./GRANDEII_PC  # run main program

```

### Run with Coverage
```
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory out
xdg-open out/index.html
```

### Continuous Integration
GitHub Actions (`.github/workflows/ci.yml`) builds the project and runs the unit tests,
a valgrind memory check and a coverage report on every push and pull request to `main`.
The coverage report can be downloaded from the run page (artifact `coverage-html`).
It covers the model (the testable logic) only: `arduino/` (needs hardware), `view/` and
`controller/` (GTK UI code) are excluded.

### Commit Workflow
```
git status
git add <file>
git commit -m "descriptive message"
git push origin <branch>
```

💡 Keep commits small and focused.
