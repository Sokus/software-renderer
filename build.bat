@echo off

set common_compiler_flags=-Wall -Wextra -Werror -std=c11 -pedantic -O0 -g -Wl,/incremental:no -fdiagnostics-absolute-paths  -D SUMMONED_DEBUG=1

set libs=-luser32 -lgdi32 -lwinmm

set win32_compile=clang ..\code\win32_platform.c -o summoned.exe %common_compiler_flags% %libs%
set game_compile=clang ..\code\summoned.c -o summoned.dll -shared %common_compiler_flags% -Wl,/PDB:summoned_%random%.pdb,/EXPORT:GameUpdateAndRender

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
