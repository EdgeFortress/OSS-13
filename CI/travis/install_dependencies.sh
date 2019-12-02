#!/bin/bash
set -ev

if [ ! -d /home/travis/SFML/.git ]; then
    cd ~
    git clone --depth 1 https://github.com/SFML/SFML.git
    cd SFML
    mkdir install
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j4
    make install
fi

if [ ! -d /home/travis/vcpkg/.git ]; then
    cd ~
    git clone --depth 1 https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install gtest
fi

cd ~
wget http://lemon.cs.elte.hu/pub/sources/lemon-1.3.1.tar.gz
tar zxf lemon-1.3.1.tar.gz
cd lemon-1.3.1
mkdir build
cd build
cmake ..
make
sudo make install
