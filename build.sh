#!/bin/bash

WARNNING_FLAGS="-Wall -Wextra -Wshadow -Wdouble-promotion -Wconversion"
COMMON_FLAGS="-O3 -g -fdiagnostics-absolute-paths -D SUMMONED_DEBUG=1 -lm"
SDL_FLAGS="-I../external/include/SDL2 -D _REENTRANT -L../external/libs -lSDL2"
TTF_FLAGS="-I../external/include -D _REENTRANT -L../external/libs -lSDL2_ttf"

mkdir -p build
pushd build

clang ../code/linux_platform.c -o summoned $COMMON_FLAGS -ldl $SDL_FLAGS $TTF_FLAGS -Wl,-rpath,$ORIGIN../external/libs

clang ../code/summoned.c -o summoned.so -shared $COMMON_FLAGS

popd