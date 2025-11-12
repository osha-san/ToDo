@echo off
echo Compiling To-Do List App...
g++ -std=c++17 -o TodoApp.exe src/main.cpp src/Task.cpp src/TaskManager.cpp
if %ERRORLEVEL% EQU 0 (
    echo ✓ Compilation successful!
    echo Running program...
    TodoApp.exe
) else (
    echo ✗ Compilation failed!
)
pause