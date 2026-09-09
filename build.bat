@echo off
REM 五子棋测试构建脚本（Windows）
REM 用法：build.bat        编译并运行测试
REM       build.bat clean  清理生成的文件

if "%1"=="clean" (
    if exist test_game.exe del test_game.exe
    if exist test_game del test_game
    echo Cleaned.
    goto :eof
)

REM 优先使用 gcc，其次 cl（MSVC）
where gcc >nul 2>nul
if %errorlevel%==0 (
    echo [gcc] Compiling...
    gcc -Wall -std=c89 -pedantic -o test_game.exe test_game.c game.c
    if %errorlevel%==0 (
        echo Running tests...
        test_game.exe
    ) else (
        echo Compile failed.
        exit /b 1
    )
    goto :eof
)

where cl >nul 2>nul
if %errorlevel%==0 (
    echo [cl] Compiling...
    cl /Fe:test_game.exe test_game.c game.c
    if %errorlevel%==0 (
        echo Running tests...
        test_game.exe
    ) else (
        echo Compile failed.
        exit /b 1
    )
    goto :eof
)

echo Error: No C compiler found. Please install gcc (MinGW) or MSVC (cl).
exit /b 1