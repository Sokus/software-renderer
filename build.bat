@echo off

:: Flags
set macros=-D SUMMONED_DEBUG=1
set common=-g -O0
set warnings=-Wall -Wextra -Werror -std=c11 -pedantic
set flags=%macros% %common% %warnings%

set libs=-luser32 -lgdi32 -lwinmm

:: Filenames
set exe=summoned.exe
set dll=summoned.dll

set root=%~dp0%

:: Commands
set win32_compile=gcc %root%code\win32_platform.c -o %exe% %flags% %libs%
set game_compile=gcc %root%code\summoned.c -shared -o %dll% %flags% %libs%

if not exist build mkdir build
pushd build

:: Compile the dynamic library
%game_compile%

:: Check if exe is running
set recompile=1
if exist %exe% (tasklist | find "%exe%">nul: && set recompile=0)

:: Recompile exe if possible
if %recompile%==1 (%win32_compile%) else ( echo %exe% is currently running )


:: Exit build directory
popd 
