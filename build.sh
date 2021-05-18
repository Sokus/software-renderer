pushd ./build
g++ ../code/input/*.cpp ../code/logic/*.cpp ../code/objects/*.cpp ../code/output/*_LINUX.cpp ../code/utility/*.cpp -o summoned.output
popd