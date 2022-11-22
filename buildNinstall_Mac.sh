#! /bin/zsh

source envset_Mac.sh
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DPYTHON_LIBRARY=/opt/homebrew/Cellar/python@3.10/3.10.8/Frameworks/Python.framework/Versions/3.10/lib/libpython3.10.dylib -DPYTHON_INCLUDE_DIR=/opt/homebrew/Cellar/python@3.10/3.10.8/Frameworks/Python.framework/Versions/3.10/include/python3.10 ..
make -j8 install
cd ..
