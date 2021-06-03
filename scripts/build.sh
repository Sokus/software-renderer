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
gcc -o summoned\
 ../code/input/*.c\
 ../code/logic/*.c\
 ../code/objects/*.c\
 ../code/output/*_LINUX.c\
 ../code/utility/*.c\
 ../code/*.c\
 -I../code
exec > /dev/null

popd #build
popd #project
popd #parent
