## Build instructions
git submodule update --init --recursive
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
