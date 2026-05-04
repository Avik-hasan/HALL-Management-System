@echo off
echo Building Hall Management System...
g++ -std=c++17 -o hall_system.exe main.cpp
if %errorlevel%==0 (
    echo Build Successful!
    echo.
    hall_system.exe
) else (
    echo Build Failed!
)
pause
