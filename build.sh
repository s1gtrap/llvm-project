#!/bin/bash

pwd
ls -al

# build native llvm
cmake -G Ninja \
	-S llvm/ \
	-B build/llvm-native/ \
	-DCMAKE_BUILD_TYPE=Release \
	-DLLVM_TARGETS_TO_BUILD=WebAssembly \
	-DLLVM_ENABLE_PROJECTS="clang"
cmake --build build/llvm-native/ -- llvm-tblgen clang-tblgen

# build nlohmann/json with emscripten
git clone --depth 1 --branch v3.11.3 https://github.com/nlohmann/json.git build/nlohmann-json
pushd build/nlohmann-json
emcmake cmake -B build -G Ninja
pushd build
emmake ninja
emmake ninja install
popd
popd

# build llvm with emscripten
emcmake cmake -DCMAKE_BUILD_TYPE=Debug -S llvm -B build
cd build
emmake make llvm-json
