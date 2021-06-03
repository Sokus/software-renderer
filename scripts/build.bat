@echo off

set buildDir=%~dp0..\build
if exist %buildDir% rmdir %buildDir% /s /q
mkdir %buildDir%
pushd %buildDir%

set files=..\code\*.c ^
..\code\input\*.c ^
..\code\logic\*.c ^
..\code\objects\*.c ^
..\code\output\*_WIN32.c ^
..\code\utility\*.c
gcc %files% -o summoned -I..\code

popd