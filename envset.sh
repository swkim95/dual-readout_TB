#! /bin/bash

source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh

export INSTALL_DIR_PATH=$PWD/install

export LD_PRELOAD="/usr/lib64/libpdcap.so"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib64:/usr/lib64/dcap
export PYTHONPATH=$PYTHONPATH:$INSTALL_DIR_PATH/lib64
