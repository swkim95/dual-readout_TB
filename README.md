# dual-readout_TB

FWcore for TB data analysis and pre-exercise scripts for 2023 Feb DRC workshop @GWNU

---
## 1) Dependencies
* ROOT
* python3
* boost::python
>__Note__ All the dependencies can be sourced on KNU / idream server using CVMFS

---
## 2) Installation manual
#### How to compile framework
```sh
# On your working directory, clone preEx_students branch
git clone -b preEx_students

# In dual-readout_TB directory, run installation script
cd dual-readout_TB
source buildNinstall.sh
```
#### How to setup environment
```sh
# Remember to set environment variable everytime you access to server
# In dual-readout_TB directory, run command below
source envset.sh  
```

#### How to compile each analysis script
```sh
# All analysis scripts are available on dual-readout_TB/analysis directory
cd analysis
source compile.sh <analysis code in cpp>

# If you want to compile TBdrawWave.cc script
e.g.) source compile.sh TBdrawWave # This will generate TBdrawWave.exe
```

#### How to run each analysis executable
```sh
./<analysis executable in exe> <run number> <maximum number of events to run>

# If you want to run TBdrawWave to draw waveform of
# 1k events from  Run702 ntuples
e.g.) ./TBdrawWave.exe 702 1000 
```

---
## 3) Pre-exercise
### Suggested runs for pre-exercise

| Runs  | Energy (GeV) | Particle | Module/Tower | Readout | # of events (wave)| # of events (fast)| 
| :---: |:------------:|:--------:|:------------:|:-------:|:-----------------:|:-----------------:|
|**702**|      20      |    e+    |     M1T1     |   PMT   |     **50028**     |     **49984**     |
|**624**|      20      |    e+    |     M1T2     |   PMT   |    **100046**     |    **100032**     |
|**700**|      20      |    e+    |     M1T3     | MCP-PMT |     **50036**     |     **49984**     |
|**704**|      20      |    e+    |     M1T4     |   PMT   |     **50020**     |     **49984**     |
>__Note__ For more details about run, see the run list documentation and DAQ log in below links.\
[Run list](https://github.com)\
[DAQ log](https://google.com)

### Pre-exercise instructions

>__Warning__ All instructions are based on Run 702. Students who are not familiar with DRC analysis are recommended to use the same Run 702 data for pre-exercise.

#### 1. Make ntuple from raw data, and validate them

#### 2. Draw single channel waveform from ntuples

#### 3. Draw DWC position plot using waveform ntuples

#### 4. Draw average time structure of pre-shower detector and module PMT 

#### 5. Draw integrated ADC plot of pre-shower detector, and decide PID cut

#### 6. Draw integrated ADC plot of module PMT

---
## 4) Running on batch scheduler (HTcondor)
```sh
# Sample script for running TBdrawWave.exe is in dual-readout_TB/condor_script/
[swkim@idream]$ cd condor_script
[swkim@idream]$ ls
exe_condor.sh  sub_condor.sub
```
#### 1. Write HTcondor submission script : sub_condor.sub
Sample submission script to run `TBdrawWave.exe` on batch scheduler
This script will run `exe_condor.sh $(Run_number)`, where `$(Run_number)` is line read from the `run_list_m1.txt` file
```sh
# sub_condor.sub
universe = vanilla
executable = exe_condor.sh # This submission script will execute exe_condor.sh script. Change to your own executable script.

arguments = $(Run_number) # Use run number as 1st argument to be passed to exe_condor.sh

# Fix here to locate logs in appropriate directory
output = ./drawWave_log/out/o_$(Run_number).out # output log file location
error  = ./drawWave_log/err/e_$(Run_number).err # error  log file location
log    = ./drawWave_log/log/l_$(Run_number).log # condor log file location

request_memory = 1 GB # 1 GB will be enough for most of jobs

should_transfer_files = YES
when_to_transfer_output = ON_EXIT
# Fix here to properly locate & transfer your executable to be executed by exe_condor.sh, lib64 folder and exe_condor.sh script
transfer_input_files = /u/user/swkim/pre-exercise/dual-readout_TB/analysis/TBdrawWave.exe, \
                       /u/user/swkim/pre-exercise/dual-readout_TB/install/lib64, \
                       /u/user/swkim/pre-exercise/dual-readout_TB/exe_condor.sh

JobBatchName = draw_wave_m1 # Job name
queue Run_number from run_list_m1.txt # Read run_list_m1.txt line by line, and use them as $(Run_number)
# Note that run_list_m1.txt should be in the same directory as your condor submission script
```
#### 2. Write HTcondor execution script : exe_condor.sh
The `sub_condor.sub` script will run `exe_condor.sh`
`exe_condor.sh` will set environment variables on HTcondor, and run `TBdrawWave.exe`
```sh
# exe_condor.sh
#! /bin/bash

source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh

export LD_PRELOAD="/usr/lib64/libpdcap.so"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/lib64/dcap"

# Fix here to your own installation directory!!
export INSTALL_DIR_PATH=/u/user/swkim/pre-exercise/dual-readout_TB/install # FIX!!

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib64
export PYTHONPATH=$PYTHONPATH:$INSTALL_DIR_PATH/lib64

# This script will run TBdrawWave.exe with arguments $1, 100
# $1 : First argument passed to exe_condor.sh by sub_condor.sub. In this case, it is $(Run_number)
# 100 : Maximum number of events to process
# Change TBdrawWave.exe to any other executable to run on batch
./TBdrawWave.exe $1 100
```

#### 3. Submit condor batch jobs using condor_submit
```sh
# In directory with sub_condor.sub, exe_condor.sh, run_list_m1.txt and proper log files
condor_submit sub_condor.sub

# Using condor_q, one can check their batch job status
condor_q
```

>__Note__ For more information about running HTcondor, please refer to [this manual](http://t2-cms.knu.ac.kr/wiki/index.php/HTCondor)

>__Note__ Data stored in KNU Tier-3 Storage Element can also be browsed on batch jobs using dcap protocal. For more information about dcap, please refer to [this manual](http://t2-cms.knu.ac.kr/wiki/index.php/Dcap)

---
## 5) Troubleshooting
#### g++ error while compiling anaylsis scripts 
![image desc](./doc/g%2B%2B_error.png)
```sh
# Make sure that you set environment variable everytime you access to the server
# Also, source envset.sh should be done on dual-readout_TB directory
[swkim@idream]$ pwd
/u/user/swkim/dual-readout_TB/ # Make sure you're in dual-readout_TB directory
[swkim@idream]$ source envset.sh
```

#### Library error while running executable (same solution as above)
![image desc](./doc/lib_error.png)
```sh
# Make sure that you set environment variable everytime you access to the server
# Also, source envset.sh should be done on dual-readout_TB directory
[swkim@idream]$ pwd
/u/user/swkim/dual-readout_TB/ # Make sure you're in dual-readout_TB directory
[swkim@idream]$ source envset.sh
```

#### Environment setup

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<path_to_install>/lib64 # or lib based on your architecture
export PYTHONPATH=$PYTHONPATH:<path_to_install>/lib64 # or lib based on your architecture
```

---
## 6) Useful links

[2022 TB @ CERN main documentation page](https://github.com)\
[Run list](https://github.com)\
[DAQ log](https://google.com)\
[Mapping info](https://github.com)\
[KNU Tier-3 HTcondor manual](http://t2-cms.knu.ac.kr/wiki/index.php/HTCondor)\
[Using dcap](http://t2-cms.knu.ac.kr/wiki/index.php/Dcap)