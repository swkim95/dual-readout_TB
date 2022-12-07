#! /bin/bash

for i in `cat run_list.txt`
do
echo "Processing wave run ${i}"
mkdir /u/user/swkim/SE_UserHome/2022_DRC_TB_ntuple/waveform/Run_${i}
python3 TBntuplize.py -rn ${i} -fe 5000 | tee -a wave_log/wave_log_${i}.log
done
