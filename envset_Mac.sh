#! /bin/zsh

source /Users/swkim/root-6.26.08/install/bin/thisroot.sh

export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/opt/homebrew/include" 

export INSTALL_DIR_PATH=$PWD/install

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib
export PYTHONPATH=$PYTHONPATH:$INSTALL_DIR_PATH/lib