@echo off
rmdir /s /q "%~dp0clang-debug "
rmdir /s /q "%~dp0clang-release"

mkdir "%~dp0clang-debug"
pushd "%~dp0clang-debug"
cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
popd

mkdir "%~dp0clang-release"
pushd "%~dp0clang-release"
cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
popd