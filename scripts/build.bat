@echo off

set buildDir=%~dp0..\build
if exist %buildDir% rmdir %buildDir% /s /q
mkdir %buildDir%
pushd %buildDir%

set files=..\code\*.c ^
..\code\console\*.c ^
..\code\logic\*.c ^
..\code\objects\*.c ^
..\code\parser\*.c ^
..\code\utility*.c ^
..\code\platform\windows\*_WIN32.c
gcc %files% -o summoned -I..\code

popd