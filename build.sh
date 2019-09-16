#!/bin/sh
ls build &>/dev/null && rm -rf build
mkdir build && cd build && conan install .. "$@" && cmake .. && cmake --build .
