@echo off

FOR /F "tokens=* USEBACKQ" %%F IN (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath`) DO (
SET var=%%F
)

call "%var%\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64

rmdir /s /q "%~dp0msvc-debug"
rmdir /s /q "%~dp0msvc-release"

mkdir "%~dp0msvc-debug"
pushd "%~dp0msvc-debug"
cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
popd

mkdir "%~dp0msvc-release"
pushd "%~dp0msvc-release"
cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
popd
