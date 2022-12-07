#! /bin/zsh

g++ -I$INSTALL_DIR_PATH/include  -L$INSTALL_DIR_PATH/lib $INSTALL_DIR_PATH/lib/libdrcTB.dylib `python3.10-config --includes --libs` `root-config --cflags --libs` $1.cc -o $1
