#!/bin/bash

build_type="$1"

mkdir -p build/
if [[ $build_type == "debug" || $build_type == "" ]]; then
    cd build/
    mkdir -p debug/
    cd debug/
    cmake ../../. -DCMAKE_BUILD_TYPE="Debug"
    cmake --build .
elif [[ $build_type == "debug_asan" ]]; then
    cd build/
    mkdir -p debug/
    cd debug/
    cmake ../../. -DCMAKE_BUILD_TYPE="Debug" -DUSE_ASAN=true
    cmake --build .
elif [[ $build_type == "release" ]]; then
    cd build/
    mkdir -p release/
    cd release/
    cmake ../../. -DCMAKE_BUILD_TYPE="Release"
    cmake --build .
else
    echo "Unrecognized build type: $build_type!"
fi
