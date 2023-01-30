#! /bin/bash

# Command :
# Turn on tmux terminal ( for details of tmux : https://pebpung.github.io/engineering/2022/04/24/Tmux.html , tmux already installed in KNU server)
# (in tmux terminal, type) ./run_fast.sh

for i in `cat run_list.txt`
do
echo "Processing fast run ${i}"
mkdir /u/user/swkim/SE_UserHome/2022_DRC_TB_ntuple/fastmode/Run_${i} # CHANGE THIS DIRECTORY!! >> change to fast ntuple saving directory
python3 TBntuplize.py -rn ${i} -f -fe 100000 | tee -a fast_log/fast_log_${i}.log # tee -a {path to fast ntupler log} >> log path는 마음대로 정하기. 단, 해당 path가 미리 만들어져 있어야 함
done