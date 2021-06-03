pushd ../
if [ -d "build" ]; then
    rmdir --ignore-fail-on-non-empty build
fi
mkdir build
pushd ./build

gcc ../code/input/*.c ../code/logic/*.c ../code/objects/*.c ../code/output/*_LINUX.c ../code/utility/*.c ../code/*.c -o summoned -I../code

popd
popd