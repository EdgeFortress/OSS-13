#!/bin/bash
set -ev

mkdir build
cd build

cmake -DSFML_DIR="~/SFML/install/lib/cmake/SFML" -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4
