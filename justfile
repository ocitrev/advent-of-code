set windows-shell := ["C:\\Program Files\\Git\\bin\\sh.exe", "-uc"]

zig-cache-dir := "build/zig/.cache"
export ZIG_LOCAL_CACHE_DIR := zig-cache-dir
export ZIG_GLOBAL_CACHE_DIR := zig-cache-dir
export NINJA_STATUS := '[%s/%t %p :: %e] '
configure-cpp := "cmake -G 'Ninja Multi-Config' -S . -B build/cpp -DCMAKE_C_COMPILER='zig;cc' -DCMAKE_CXX_COMPILER='zig;c++'"
exe-ext := if os() == 'windows' { '.exe' } else { '' }

[private]
defafult:
    @just --list --unsorted

build: cpp rust zig odin

clean:
    {{ if path_exists('build') == 'true' { 'rm -r build/' } else { '' } }}
    {{ if path_exists('.zig-cache') == 'true' { 'rm -r .zig-cache/' } else { '' } }}
    {{ if path_exists('zig-out') == 'true' { 'rm -r zig-out/' } else { '' } }}
    {{ if path_exists('zig-pkg') == 'true' { 'rm -r zig-pkg/' } else { '' } }}

_configure-cpp:
    {{ if path_exists('build/cpp') == 'false' { configure-cpp } else { '' } }}

[arg('release', long='release', short='r', value='-f build-RelWithDebInfo.ninja')]
[working-directory('build/cpp')]
cpp target='' release='': _configure-cpp
    ninja {{ release }} {{ target }}

[arg('release', long='release', short='r', value='-r')]
rust release='':
    cargo build {{ release }}

[arg('cpp', long='cpp', value='-Dlang=cpp')]
[arg('release', long='release', short='r', value='--release=safe')]
zig steps='' release='' cpp='' *options:
    zig build --prefix build/zig {{ steps }} {{ release }} {{ cpp }} {{ options }}

@_odin-configure:
    {{ if path_exists("build/odin") == 'false' { 'mkdir -p build/odin' } else { '' } }}

_odin-build name file: _odin-configure
    odin build {{ file }} -file -out:build/odin/{{ name }}{{ exe-ext }}

odin-2019-2: (_odin-build '2019-2' 'src/2019/day2.odin')

odin: odin-2019-2

pull:
    git pull --recurse-submodules

show-leaderboard:
    python3 inputs/leaderboard.py

subl:
    subl --project advent-of-code.sublime-project
