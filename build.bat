@echo off
echo Setting up Trade Simulator build environment...

REM Check if vcpkg exists
if not exist "vcpkg" (
    echo Cloning vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    call bootstrap-vcpkg.bat
    cd ..
)

REM Install dependencies with specific versions
echo Installing dependencies...
call vcpkg\vcpkg install qt5-base:x64-windows --recurse
call vcpkg\vcpkg install openssl:x64-windows --recurse
call vcpkg\vcpkg install nlohmann-json:x64-windows --recurse

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure and build with specific generator
echo Configuring project...
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed. Please make sure Visual Studio 2022 is installed.
    echo Download from: https://visualstudio.microsoft.com/vs/community/
    echo Make sure to select "Desktop development with C++" during installation.
    exit /b 1
)

echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Please check the error messages above.
    exit /b 1
)

echo Build complete!
cd .. 