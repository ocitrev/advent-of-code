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

# Prerequisites

Ce projet a besoin de [vcpkg](https://vcpkg.io/en/getting-started) pour trouver ses dépendences. La variable d'environment `VCPKG_ROOT` doit est bien définie.

# Comment faire

## Projets C++

Il faut configurer cmake avec la commande

    ./configure-cpp

Ensuite pour compiler avec la commande

    ./build-cpp

Pour executer tous les projets

    ./build-cpp -t run

Pour executer tous les projets de 2019

    ./build-cpp -t run-2019

Pour executer un projet en particulier, exemple avec jour 2, 2019

    ./build-cpp -t run-2019-2

## Projets Zig

Pour compiler les projets Zig:

    ./zig-build

Pour executer tous les projets

    ./zig-build run

Pour executer tous les tests unitaires

    ./zig-build test

Pour executer tous les projets de 2024

    ./zig-build run-2024

Pour executer un projet en particulier, exemple avec jour 5, 2016

    ./zig-build run-2016-5

## Projets Rust

Pour compiler les projets Rust:

    cargo build

Pour executer un projet en particulier, exemple avec jour 5, 2016

    cargo run --bin 2016-5
