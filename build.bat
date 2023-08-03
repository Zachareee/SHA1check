@echo off
for %%i in (*.c) do (
	echo Compiling %%i
	gcc -c -I headers %%i -o %%~ni.o
	echo.
)
gcc *.o -o main.exe
del *.o
pause
