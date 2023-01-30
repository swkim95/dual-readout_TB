#! /bin/bash

# Command :
# Turn on tmux terminal ( for details of tmux : https://pebpung.github.io/engineering/2022/04/24/Tmux.html , tmux already installed in KNU server)
# (in tmux terminal, type) ./run_wave.sh

for i in `cat run_list.txt`
do
echo "Processing wave run ${i}"
mkdir /u/user/swkim/SE_UserHome/2022_DRC_TB_ntuple/waveform/Run_${i} # CHANGE THIS DIRECTORY!! >> change to wave ntuple saving directory
python3 TBntuplize.py -rn ${i} -fe 5000 | tee -a wave_log/wave_log_${i}.log # tee -a {path to wave ntupler log} >> log path는 마음대로 정하기. 단, 해당 path가 미리 만들어져 있어야 함
done