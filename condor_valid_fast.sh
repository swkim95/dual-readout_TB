#! /bin/bash

source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh

export LD_PRELOAD="/usr/lib64/libpdcap.so"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/lib64/dcap"

export INSTALL_DIR_PATH=$PWD/install

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib64
export PYTHONPATH=$PYTHONPATH:$INSTALL_DIR_PATH/lib64

python3 ./test/TBvalid.py -rn $1 -id "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_Data/" -in "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_ntuple/fastmode/" -f -o "./test/valid_plot/" | tee -a /u/user/swkim/dual-readout_TB/test/valid_tee_log/fast/fast_valid_fullLog_$1.log