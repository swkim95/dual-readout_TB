# dual-readout_TB

FWcore for TB data analysis and pre-exercise scripts for 2023 Feb DRC workshop @GWNU

## 1) Dependencies
* ROOT
* python3
* boost::python
>__Note__ All the dependencies can be sourced on KNU / iDream server using CVMFS

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

## 3) Pre-exercise
### Suggested runs for pre-exercise

| Runs  | Energy (GeV) | Particle | Module/Tower | Readout | # of events (wave)| # of events (fast)| 
| :---: |:------------:|:--------:|:------------:|:-------:|:-----------------:|:-----------------:|
|**702**|      20      |    e+    |     M1T1     |   PMT   |     **50028**     |     **49984**     |
|**624**|      20      |    e+    |     M1T2     |   PMT   |    **100046**     |    **100032**     |
|**700**|      20      |    e+    |     M1T3     | MCP-PMT |     **50036**     |     **49984**     |
|**704**|      20      |    e+    |     M1T4     |   PMT   |     **50020**     |     **49984**     |
>__Note__ For more details about run, see the run list documentation and DAQ log in below links.

### Pre-exercise instructions
#### 1. Make ntuple from raw data, and validate them

#### 2. Draw single channel waveform from ntuples

#### 3. Draw DWC position plot using waveform ntuples

#### 4. Draw average time structure of pre-shower detector and module PMT 

#### 5. Draw integrated ADC plot of pre-shower detector, and decide PID cut

#### 6. Draw integrated ADC plot of module PMT


## 4) Troubleshooting
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