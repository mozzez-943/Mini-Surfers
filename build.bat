@echo off
set INCLUDE_PATH=include;SDL2.dll;
set LIB_PATH=lib
g++ -I%INCLUDE_PATH% src\main.cpp -o main.exe -L%LIB_PATH% -lSDL2 -lcuda
