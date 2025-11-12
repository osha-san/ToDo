@echo off
cls

REM Change to the directory where this batch file is located
cd /d "%~dp0"

echo =======================================
echo   To-Do List Web Server Builder
echo =======================================
echo.
echo Working directory: %CD%
echo.

REM Check if source files exist
if not exist "src\web_main.cpp" (
    echo [ERROR] src\web_main.cpp not found!
    echo Current directory: %CD%
    echo.
    pause
    exit /b 1
)

if not exist "src\Task.cpp" (
    echo [ERROR] src\Task.cpp not found!
    pause
    exit /b 1
)

if not exist "src\TaskManager.cpp" (
    echo [ERROR] src\TaskManager.cpp not found!
    pause
    exit /b 1
)

REM Check if ASIO is installed
if not exist "include\asio.hpp" (
    echo [ERROR] ASIO not found in include\ folder!
    echo.
    echo Please download ASIO from:
    echo https://think-async.com/Asio/
    echo.
    echo Then copy:
    echo   - asio.hpp to include\asio.hpp
    echo   - asio\ folder to include\asio\
    echo.
    pause
    exit /b 1
)

echo [1/3] Checking compiler...
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] g++ not found! Please install MinGW.
    echo.
    echo Download from: https://www.msys2.org/
    echo Then run: pacman -S mingw-w64-x86_64-gcc
    echo.
    pause
    exit /b 1
)
echo    Compiler found: ✓
echo.

echo [2/3] Compiling source files...
echo.

g++ -std=c++17 ^
    -I./include ^
    -DASIO_STANDALONE ^
    -D_WIN32_WINNT=0x0601 ^
    -o web_main.exe ^
    src/web_main.cpp ^
    src/Task.cpp ^
    src/TaskManager.cpp ^
    -pthread ^
    -lws2_32 ^
    -lmswsock ^
    -lwsock32

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo =======================================
    echo [X] Compilation FAILED!
    echo =======================================
    echo.
    echo Common fixes:
    echo 1. Make sure MinGW is installed correctly
    echo 2. Verify ASIO is in include\ folder
    echo 3. Check all source files exist
    echo 4. Try running without administrator privileges
    echo.
    pause
    exit /b 1
)

echo.
echo [3/3] Compilation successful! 
echo.
echo =======================================
echo   Starting Web Server...
echo =======================================
echo.

REM Create data directory if it doesn't exist
if not exist "data" mkdir data

REM Start the server
web_main.exe

echo.
echo =======================================
echo   Server stopped
echo =======================================
pause