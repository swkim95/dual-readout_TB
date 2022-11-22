#! /bin/bash

source envset_Mac.sh
mkdir build install
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
make -j8 install
cd ..
