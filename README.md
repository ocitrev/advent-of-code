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

Ce projet a besoin de [vcpkg](https://vcpkg.io/en/getting-started) pour trouver ses dependences. La variable d'environment `VCPKG_ROOT` doit est bien définie.

Comment compiler avec clang:

    mkdir build
    cd build
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
    ninja

Comment compiler GCC:

    mkdir build
    cd build
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
    ninja
