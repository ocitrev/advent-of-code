@echo off
rmdir /s /q "%~dp0gcc-debug"
rmdir /s /q "%~dp0gcc-release"

mkdir "%~dp0gcc-debug"
pushd "%~dp0gcc-debug"
cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
popd

mkdir "%~dp0gcc-release"
pushd "%~dp0gcc-release"
cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
popd