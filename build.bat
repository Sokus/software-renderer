@echo off

set warnings=-Wall -Wextra -Wshadow -Wconversion -fdiagnostics-absolute-paths -Wno-unused-function
set common_compiler_flags=-O0 -g -Wl,/incremental:no -D SUMMONED_DEBUG=1

set libs=-luser32 -lgdi32 -lwinmm

set win32_compile=clang ..\code\win32_platform.c -o summoned.exe %warnings% %common_compiler_flags% %libs%
set game_compile=clang ..\code\summoned.c -o summoned.dll -shared %warnings% %common_compiler_flags% -Wl,/PDB:summoned_%random%.pdb,/EXPORT:GameUpdateAndRender

if not exist build mkdir build
pushd build

del *.pdb 1>NUL 2>NUL
echo WAITING FOR PDB > lock.tmp
%game_compile%
del lock.tmp

:: Check if exe is running
set recompile=1
if exist summoned.exe ( tasklist | find "summoned.exe">nul: && set recompile=0 )

:: Recompile exe if possible
if %recompile%==1 ( %win32_compile% ) else ( echo summoned.exe is currently running )

:: Exit build directory
popd 
