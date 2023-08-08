@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
set objects=args compare dir files getdent hashing paths main
set CF=--optimize

for %%i in (%objects%) do (
    gcc -c -Wall -I headers %CF% %%i.c -o %%i.o
    if !ERRORLEVEL!==0 (
        echo %%i.c compiled
    ) else (
        echo %%i.c did not compile
        exit /B 1
    )
    echo.
    set output=!output! %%i.o
)

gcc %output% %CF% -o SHA1check.exe
if !ERRORLEVEL! == 0 (
    echo SHA1check.exe built
) else (
    echo.
    echo Something went wrong...
)

del *.o
pause
