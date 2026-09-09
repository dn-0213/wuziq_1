@echo off
rem Build script for Gomoku C terminal version (MSVC)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 goto :err
cl /nologo /utf-8 /W4 main.c game.c ui.c /Fe:gomoku.exe
if errorlevel 1 goto :err
echo.
echo Build OK: gomoku.exe
goto :eof
:err
echo Build FAILED.
exit /b 1
