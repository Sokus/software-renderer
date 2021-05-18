if [ -d "build" ] then
    rmdir build
fi
mkdir build
pushd ./build
g++ ../code/input/*.cpp ../code/logic/*.cpp ../code/objects/*.cpp ../code/output/*_LINUX.cpp ../code/utility/*.cpp ../code/*.cpp -o summoned.output
popd