import pydrcTB
import re
import os
import argparse
import ROOT
from ROOT import TH1F
from glob import glob


def atoi(text) :
    return int(text) if text.isdigit() else text

def natural_keys(text) :
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]

parser = argparse.ArgumentParser()

parser.add_argument('--runNumber', '-rn', action='store', type=int, required=True, help="Run number")
parser.add_argument('--inputNtuple', '-in', action='store', type=str, help="Path to input ntuples")
parser.add_argument('--inputData', '-id', action='store', type=str, help="Path to input data files")
parser.add_argument('--doFast', '-f', action='store_true', default=False, help='If true, validate in fastmode. Else, validate in waveform mode')
parser.add_argument('--output', '-o', action='store', type=str, default="./", help="Path to store output png files")

args = parser.parse_args()
rn = args.runNumber
mode = "Wave" if not args.doFast else "Fast"
print(f"Validating in {mode} mode with run number {rn}...")

if args.inputData.endswith("/") :
    inputDataPath = args.inputData
else :
    inputDataPath = args.inputData + "/"

DataFileNumbers = []
for mid in range(1,16):
    FullDataPath = inputDataPath + f"Run_{rn}/Run_{rn}_{mode}/Run_{rn}_{mode}_MID_{mid}/*.dat"
    FullData_list = [files for files in glob(FullDataPath) if not os.stat(files).st_size == 0]
    DataFileNumbers.append( len(FullData_list) )
maxDataFileNum = min(DataFileNumbers) # just in case if MIDs have different # of fast .dat files... which should not happen
DataFiles=[]
for fn in range(maxDataFileNum) :
    DataFilesPerFN = []
    for mid in range(1,16) :
        fileName= inputDataPath + f"Run_{rn}/Run_{rn}_{mode}/Run_{rn}_{mode}_MID_{mid}/Run_{rn}_{mode}_MID_{mid}_FILE_{fn}.dat"
        if( os.path.isfile(fileName) and (os.stat(fileName).st_size != 0) ) :
            DataFilesPerFN.append(fileName)
        else:
            print(fileName,"not found or has size of zero")
    DataFiles.append(DataFilesPerFN)


if not args.inputNtuple.endswith("/") :
    inputNtuplePath = args.inputNtuple + f"/Run_{rn}/{mode}/"
else :
    inputNtuplePath = args.inputNtuple + f"Run_{rn}/{mode}/"

Ntuples = []
for fn in glob(inputNtuplePath + f"ntuple_Run_{rn}_{mode}_*.root") :
    Ntuples.append(fn)
Ntuples.sort(key = natural_keys)


if args.output.endswith("/") :
    outDir = args.output + f"valid_plots_Run_{rn}/{mode}/"
else :
    outDir = args.output + f"/valid_plots_Run_{rn}/{mode}/"
if not os.path.exists(outDir) : 
    os.makedirs(outDir)


# 2 ways of getting CID (channel ID)
# 1. Using TBcid constructor >> No need to load mapping file, but need to know which MID & channel does module belongs to 
#    Ex) cid_M2T6S = pydrcTB.TBcid(2, 2), cid_M2T6C = pydrcTB.TBcid(2, 10)

# Enum detid list :
# nulldet      = -1
# SiPM         = 0
# PMT          = 1
# MCPPMT_nC    = 8 // not used
# MCPPMT_nS    = 9 // not used
# DWC1analogue = 10
# DWC1digital  = 12
# DWC2analogue = 16
# DWC2digital  = 18
# extTrig1     = 22
# extTrig2     = 23
# veto         = 24
# preshower    = 25
# tail         = 26
# muon         = 27
# Module3D_C   = 28
# Module3D_S   = 29  
# 
# 2. Using getcid function of TButility class (cannot use TBdetector::detid enum, not implemented properly yet...)
#    getcid( int module, int tower, bool isCeren) or getcid(int detid, int module, int tower, bool isCeren)
#    can use both function, and for "int detid", enter int corresponding to enum
#    Also, before using getcid, one must load mapping information using loading function
#    Lastly, this method cannot be used for SiPM
#    Ex) cid_M2T6S = TButils.getcid(2, 6, 0) or use TButils.getcid(TButils.detid(1), 2, 6, 0)

# cid_M1T1S = TButils.getcid(1, 1, 0)
# cid_M1T2S = TButils.getcid(1, 2, 0)
# cid_M1T3S = TButils.getcid(1, 3, 0)
# cid_M1T4S = TButils.getcid(1, 4, 0)

# cid_M1T1C = TButils.getcid(1, 1, 1)
# cid_M1T2C = TButils.getcid(1, 2, 1)
# cid_M1T3C = TButils.getcid(1, 3, 1)
# cid_M1T4C = TButils.getcid(1, 4, 1)

# cid_M2T1S = TButils.getcid(2, 1, 0)
# cid_M2T2S = TButils.getcid(2, 2, 0)
# cid_M2T3S = TButils.getcid(2, 3, 0)
# cid_M2T4S = TButils.getcid(2, 4, 0)
# cid_M2T6S = TButils.getcid(2, 6, 0)
# cid_M2T7S = TButils.getcid(2, 7, 0)
# cid_M2T8S = TButils.getcid(2, 8, 0)
# cid_M2T9S = TButils.getcid(2, 9, 0)

# cid_M2T1C = TButils.getcid(2, 1, 1)
# cid_M2T2C = TButils.getcid(2, 2, 1)
# cid_M2T3C = TButils.getcid(2, 3, 1)
# cid_M2T4C = TButils.getcid(2, 4, 1)
# cid_M2T6C = TButils.getcid(2, 6, 1)
# cid_M2T7C = TButils.getcid(2, 7, 1)
# cid_M2T8C = TButils.getcid(2, 8, 1)
# cid_M2T9C = TButils.getcid(2, 9, 1)

# One can validate MID 13 ch 31 (single channel) fastmode like this :
# TButils = pydrcTB.TButility()
# TButils.loading("mapping_data_MCPPMT_positiveSignal_v3.csv")
# cid_sipm = pydrcTB.TBcid(13, 31)
# validator = pydrcTB.TBvalid()
# validator.setDataList(DataFiles)
# validator.setNtupleList(Ntuples)
# Data_hist = validator.drawFastHistFromData(cid_sipm, "h_Data_Mid13Ch31", False)
# Ntup_hist = validator.drawFastHistFromNtuple(cid_sipm, "h_Ntuple_Mid13Ch31", False)
# validator.drawRatio(Data_hist, Ntup_hist, "h_Ratio_Mid13Ch31", outDir)
# validator.checkTrigNum()

print("Data files : ", DataFiles)
print("Ntuples : ", Ntuples)

cid_mc = pydrcTB.TBcid(1, 16)
cid_tc = pydrcTB.TBcid(1, 14)
cid_ps = pydrcTB.TBcid(1, 12)

cid_dwc1_r = pydrcTB.TBcid(1, 17)
cid_dwc1_l = pydrcTB.TBcid(1, 19)
cid_dwc1_u = pydrcTB.TBcid(1, 21)
cid_dwc1_d = pydrcTB.TBcid(1, 23)

cid_dwc2_r = pydrcTB.TBcid(1, 25)
cid_dwc2_l = pydrcTB.TBcid(1, 27)
cid_dwc2_u = pydrcTB.TBcid(1, 29)
cid_dwc2_d = pydrcTB.TBcid(1, 31)

cid_M2T1_S = pydrcTB.TBcid(2, 1)
cid_M2T6_S = pydrcTB.TBcid(2, 2)
cid_M2T2_S = pydrcTB.TBcid(2, 3)
cid_M2T7_S = pydrcTB.TBcid(2, 4)
cid_M2T3_S = pydrcTB.TBcid(2, 5)
cid_M2T8_S = pydrcTB.TBcid(2, 6)
cid_M2T4_S = pydrcTB.TBcid(2, 7)
cid_M2T9_S = pydrcTB.TBcid(2, 8)

cid_M2T1_C = pydrcTB.TBcid(2, 9)
cid_M2T6_C = pydrcTB.TBcid(2, 10)
cid_M2T2_C = pydrcTB.TBcid(2, 11)
cid_M2T7_C = pydrcTB.TBcid(2, 12)
cid_M2T3_C = pydrcTB.TBcid(2, 13)
cid_M2T8_C = pydrcTB.TBcid(2, 14)
cid_M2T4_C = pydrcTB.TBcid(2, 15)
cid_M2T9_C = pydrcTB.TBcid(2, 16)

cid_list = [cid_mc, cid_tc, cid_ps,
            cid_dwc1_r, cid_dwc1_l, cid_dwc1_u, cid_dwc1_d,
            cid_dwc2_r, cid_dwc2_l, cid_dwc2_u, cid_dwc2_d,
            cid_M2T1_S, cid_M2T2_S, cid_M2T3_S, cid_M2T4_S, cid_M2T6_S, cid_M2T7_S, cid_M2T8_S, cid_M2T9_S,
            cid_M2T1_C, cid_M2T2_C, cid_M2T3_C, cid_M2T4_C, cid_M2T6_C, cid_M2T7_C, cid_M2T8_C, cid_M2T9_C]
# For validating whole single run, do this :
TButils = pydrcTB.TButility()
TButils.loading("mapping_data_MCPPMT_positiveSignal_v3.csv")
validator = pydrcTB.TBvalid()
validator.setDataList(DataFiles)
validator.setNtupleList(Ntuples)
# if (args.doFast) :
#     validator.checkFastTrigNum()
# else :
#     validator.checkWaveTrigNum()
for cid in cid_list :
    MID = cid.mid()
    ch = cid.channe()
    print(f"Validating MID : {MID} Ch : {ch}")
    det = TButils.find(cid)
    Data_hist_name = f"h_Data_MID{MID}Ch{ch}"
    Ntuples_hist_name = f"h_Ntuple_MID{MID}Ch{ch}"
    Ratio_hist_name = f"h_Ratio_MID{MID}Ch{ch}"
    if not ( det.isNull() ) :
        if args.doFast :
            # Data_hist = validator.drawFastHistFromData(cid, Data_hist_name, False)
            # Ntup_hist = validator.drawFastHistFromNtuple(cid, Ntuples_hist_name, False)
            # validator.drawRatio(Data_hist, Ntup_hist, Ratio_hist_name, outDir)
            validator.simpleValidFast(cid)
        else :
            # Data_hist = validator.drawWaveHistFromData(cid, Data_hist_name)
            # Ntup_hist = validator.drawWaveHistFromNtuple(cid, Ntuples_hist_name)                
            # validator.drawRatio(Data_hist, Ntup_hist, Ratio_hist_name, outDir)
            validator.simpleValidWave(cid)