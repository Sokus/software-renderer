@echo off

:: Flags
set macros=-D SUMMONED_DEBUG=1
set common=-g -O0
set warnings_on=-Wall -Wextra -Werror -pedantic -std=c11 -Werror-implicit-function-declaration -Wconversion
set warnings_off=-Wno-format -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable
set warnings_off=%warnings_off% -Wno-cast-function-type -Wno-unused-but-set-variable
set flags=%macros% %common% %warnings_on% %warnings_off%

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

:: Check if exe is running
set recompile=1
if exist %exe% (tasklist | find "%exe%">nul: && set recompile=0)

:: Recompile exe if possible
if %recompile%==1 (%win32_compile%) else ( echo %exe% is currently running )

:: Compile the dynamic library
%game_compile%

:: Exit build directory
popd 
