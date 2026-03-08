set windows-shell := ["C:\\Program Files\\Git\\bin\\sh.exe", "-uc"]

zig-cache-dir := "build/zig/.cache"
export ZIG_LOCAL_CACHE_DIR := zig-cache-dir
export ZIG_GLOBAL_CACHE_DIR := zig-cache-dir
export NINJA_STATUS := '[%s/%t %p :: %e] '
configure-cpp := "cmake -G 'Ninja Multi-Config' -S . -B build/cpp -DCMAKE_C_COMPILER='zig;cc' -DCMAKE_CXX_COMPILER='zig;c++'"

[private]
defafult:
    @just --list

build: cpp rust zig odin

clean:
    {{ if path_exists('build') == 'true' { 'rm -r build/' } else { '' } }}
    {{ if path_exists('.zig-cache') == 'true' { 'rm -r .zig-cache/' } else { '' } }}
    {{ if path_exists('zig-out') == 'true' { 'rm -r zig-out/' } else { '' } }}

[private]
configure-cpp:
    {{ if path_exists('build/cpp') == 'false' { configure-cpp } else { '' } }}

[working-directory: 'build/cpp']
[arg('release', long, short='r', value='-f build-RelWithDebInfo.ninja')]
cpp target='' release='': configure-cpp
    ninja {{release}} {{target}}

rust:
    cargo build

[arg('release', long, short='r', value='--release=safe')]
[arg('cpp', long, value='-Dlang=cpp')]
zig steps='' release='' cpp='' *options:
    zig build --prefix build/zig {{steps}} {{release}} {{cpp}} {{options}}

odin:
    @{{ if path_exists("build/odin") == 'false' { 'mkdir -p build/odin' } else { '' } }}
    odin build src/2019/day2.odin -file -out:build/odin/2019-2.exe

show-leaderboard:
    python3 inputs/leaderboard.py
