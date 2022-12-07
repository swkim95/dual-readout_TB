#! /bin/bash

for i in `cat run_list.txt`
do
echo "Processing fast run ${i}"
mkdir /u/user/swkim/SE_UserHome/2022_DRC_TB_ntuple/fastmode/Run_${i}
python3 TBntuplize.py -rn ${i} -f -fe 100000 | tee -a fast_log/fast_log_${i}.log
done
