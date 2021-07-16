@echo off
setlocal enabledelayedexpansion

pushd %~dp0..
set root=%cd%
popd
set code=%root%\code
set build=%root%\build
set commonFlags=-g -Wall -Wextra -Werror-implicit-function-declaration -Wno-unused-function -Wconversion -Wno-format -O0
set exe=win32_summoned.exe
set dll=summoned.dll

set win32Compile=gcc -o %exe% %commonFlags% %code%\win32_summoned.c
set gameCompile=gcc -shared -o %dll% %commonFlags% %code%\summoned.c

Rem Clean the build directory
((if exist %build% rmdir /s /q %build%) & mkdir %build%) 1>NUL 2>NUL

pushd %build%

set recompile=1
if exist %exe% (
    Rem Check if exe is running
    tasklist /fi "ImageName eq %exe%" /fo csv 2>NUL | find /I "%exe%">NUL
    if "%ERRORLEVEL%"=="0" set recompile=0
)

Rem Recompile exe if possible
if %recompile%==1 (
    set start=!time!
    %win32Compile%
    set end=!time!

    echo | set /p dummyVar="%exe% compiled in "
    CALL %~dp0\timediff.bat !start! !end!
) else ( echo %exe% is currently running )

Rem Compile the dynamic library
set start=%time%
%gameCompile%
set end=%time%

echo | set /p dummyVar="%dll% compiled in "
CALL %~dp0\timediff.bat %start% %end%

popd Rem Exit build directory
