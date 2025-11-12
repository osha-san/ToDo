# build_web.ps1
$ErrorActionPreference = "Stop"

Write-Host "=======================================" -ForegroundColor Cyan
Write-Host "  To-Do List Web Server Builder" -ForegroundColor Cyan
Write-Host "=======================================" -ForegroundColor Cyan
Write-Host ""

# Get script directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

Write-Host "Working directory: $PWD" -ForegroundColor Yellow
Write-Host ""

# Check source files
$requiredFiles = @(
    "src\web_main.cpp",
    "src\Task.cpp",
    "src\TaskManager.cpp",
    "include\asio.hpp",
    "include\crow_all.h"
)

foreach ($file in $requiredFiles) {
    if (-not (Test-Path $file)) {
        Write-Host "[ERROR] $file not found!" -ForegroundColor Red
        Write-Host ""
        pause
        exit 1
    }
}

Write-Host "[1/3] All source files found ✓" -ForegroundColor Green
Write-Host ""

# Check compiler
try {
    $null = Get-Command g++ -ErrorAction Stop
    Write-Host "[2/3] Compiler found ✓" -ForegroundColor Green
} catch {
    Write-Host "[ERROR] g++ not found! Install MinGW first." -ForegroundColor Red
    pause
    exit 1
}
Write-Host ""

# Compile
Write-Host "[3/3] Compiling..." -ForegroundColor Yellow
Write-Host ""

$compileCmd = "g++ -std=c++17 -I./include -DASIO_STANDALONE -D_WIN32_WINNT=0x0601 -o web_main.exe src/web_main.cpp src/Task.cpp src/TaskManager.cpp -pthread -lws2_32 -lmswsock -lwsock32"

Invoke-Expression $compileCmd

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "=======================================" -ForegroundColor Green
    Write-Host "  Compilation Successful!" -ForegroundColor Green
    Write-Host "=======================================" -ForegroundColor Green
    Write-Host ""
    
    # Create data folder
    if (-not (Test-Path "data")) {
        New-Item -ItemType Directory -Path "data" | Out-Null
    }
    
    Write-Host "Starting server..." -ForegroundColor Cyan
    Write-Host ""
    
    .\web_main.exe
} else {
    Write-Host ""
    Write-Host "=======================================" -ForegroundColor Red
    Write-Host "  Compilation Failed!" -ForegroundColor Red
    Write-Host "=======================================" -ForegroundColor Red
    pause
    exit 1
}