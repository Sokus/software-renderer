#!/bin/bash

exec > /dev/null
pushd "$(dirname "${BASH_SOURCE[0]}")"

pushd ../
if [ -d "build" ]; then
    rm -rf build
fi
mkdir build
pushd ./build

exec > /dev/tty
pwd
gcc -o summoned\
 ../code/console/*.c\
 ../code/logic/*.c\
 ../code/objects/*.c\
 ../code/parser/*.c\
 ../code/utility/*.c\
 ../code/platform/linux/*_LINUX.c\
 ../code/*.c\
 -I../code
exec > /dev/null

popd #build
popd #project
popd #parent
