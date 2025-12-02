# Mes solutions pour l'Advent of Code
- [2015](https://adventofcode.com/2015)
- [2016](https://adventofcode.com/2016)
- [2017](https://adventofcode.com/2017)
- [2018](https://adventofcode.com/2018)
- [2019](https://adventofcode.com/2019)
- [2020](https://adventofcode.com/2020)
- [2021](https://adventofcode.com/2021)
- [2022](https://adventofcode.com/2022)
- [2023](https://adventofcode.com/2023)
- [2024](https://adventofcode.com/2024)
- [2025](https://adventofcode.com/2025)

# Comment faire

## Projets C++

Prérequis pour le C++

- [vcpkg](https://vcpkg.io/en/getting-started)
- [CMake](https://cmake.org/)
- [ninja](https://ninja-build.org/)

Compilation

    ./build-cpp

Exécution de tous les projets

    ./build-cpp -t run

Exécution des solutions d'une année (exemple : 2019)

    ./build-cpp -t run-2019

Exécution d’un jour particulier (exemple : 2019 — jour 2)

    ./build-cpp -t run-2019-2

## Projets Zig

*Testé avec la version 0.15.2 de [Zig](https://ziglang.org/)*

Compilation

    ./zig-build

Pour executer tous les projets

    ./zig-build run

Exécution des tests unitaires

    ./zig-build test

Exécution d'une année (exemple : 2024)

    ./zig-build run-2024

Exécution d’un jour particulier (exemple : 2016 — jour 5)

    ./zig-build run-2016-5

## Projets Rust

Compilation

    cargo build

Exécution des tests unitaires

    cargo test

Exécution d’un jour particulier (exemple : 2016 — jour 5)

    cargo run --bin 2016-5
