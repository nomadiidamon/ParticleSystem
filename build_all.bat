@echo off
REM =====================================================
REM Build script for ParticleSystem C++ and MAUI projects
REM =====================================================

REM --- Set variables ---
set ROOT_DIR=%~dp0
set BUILD_DIR=%ROOT_DIR%build
set CONFIG=Release
set DOTNET_PROJECT_DIR=%ROOT_DIR%ParticleSystemApp
set DOTNET_SOLUTION=%DOTNET_PROJECT_DIR%\ParticleSystemApp.sln

REM --- Build C++ Project ---
echo.
echo ===============================
echo Configuring C++ Project: ParticleSystem
echo ===============================
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

REM Configure CMake
cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%"

REM Build C++ Project
echo.
echo ===============================
echo Building C++ Project: ParticleSystem
echo ===============================
cmake --build "%BUILD_DIR%" --config %CONFIG%

REM --- Build .NET MAUI Project ---
echo.
echo ===============================
echo Building .NET MAUI Project: ParticleSystemApp
echo ===============================
cd /d "%DOTNET_PROJECT_DIR%"
dotnet build "%DOTNET_SOLUTION%" -c %CONFIG%

echo.
echo ===============================
echo Build complete!
echo ===============================
pause
