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
# parser.add_argument('--doFast', '-f', action='store_true', default=False, help='If true, make fast mode ntuple')
# parser.add_argument('--RunFolder', '-rf', action='store', type=str, default="", help='Destination to the folder containing run folders')

args = parser.parse_args()
rn = args.runNumber

if not args.inputData.endswith("/") :
    inputDataPath = args.inputData + "/"
else :
    inputDataPath = args.inputData

fastDataFileNumbers = []
for mid in range(1,16):
    FastDat= inputDataPath + f"Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/"
    fastDataFileNumbers.append( len(glob(FastDat + "*.dat")) )
    maxDataFastFileNum = min(fastDataFileNumbers) # just in case if MIDs have different # of fast .dat files... which should not happen
fastDataFiles=[]
for fn in range(maxDataFastFileNum) :
    FastDataFilesPerFN = []
    for mid in range(1,16) :
        fileName= inputDataPath + f"Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/Run_{rn}_Fast_MID_{mid}_FILE_{fn}.dat"
        if(os.path.isfile(fileName)) :
            FastDataFilesPerFN.append(fileName)
        else:
            print(fileName,"not found")
    fastDataFiles.append(FastDataFilesPerFN)

if not args.inputNtuple.endswith("/") :
    inputNtuplePath = args.inputNtuple + "/"
else :
    inputNtuplePath = args.inputNtuple

fastNtuples = []
for fn in glob(inputNtuplePath + "ntuple_*_Fast_*.root") :
    fastNtuples.append(fn)
fastNtuples.sort(key = natural_keys)


# 2 ways of getting CID (channel ID)
# 1. Using TBcid constructor >> No need to load mapping file, but need to know which MID & channel does module belongs to 
cid_M2T6S = pydrcTB.TBcid(2, 2)
cid_M2T6C = pydrcTB.TBcid(2, 10)

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
TButils = pydrcTB.TButility()
TButils.loading("mapping_data_MCPPMT_positiveSignal_v3.csv")
# cid_with_util_M2T6S = TButils.getcid(TButils.detid(1), 2, 6, 0)
cid_with_util_M2T6S = TButils.getcid(2, 6, 0)

cid_with_util_M2T6S.print()

# cid_M1T1C
# cid_M1T2C
# cid_M1T3C
# cid_M1T4C

# cid_M2T1C
# cid_M2T2C
# cid_M2T3C
# cid_M2T4C
# cid_M2T6C
# cid_M2T7C
# cid_M2T8C
# cid_M2T9C

# validator = pydrcTB.TBvalid()
# Data_hist = validator.drawFastHistFromData(fastDataFiles, cid_M2T6C, "h_Data_M2T6_C", False)
# Ntup_hist = validator.drawFastHistFromNtuple(fastNtuples, cid_M2T6C, "h_Ntuple_M2T6_C", False)
# validator.drawRatio(Data_hist, Ntup_hist, "h_Ratio_M2T6_C_test")
# validator.checkTrigNum(fastNtuples)

cid_sipm = pydrcTB.TBcid(13, 31)

validator = pydrcTB.TBvalid()

validator.setDataList(fastDataFiles)
validator.setNtupleList(fastNtuples)

Data_hist = validator.drawFastHistFromData(cid_sipm, "h_Data_M2T5_Mid13Ch31_C", False)
Ntup_hist = validator.drawFastHistFromNtuple(cid_sipm, "h_Ntuple_M2T5_Mid13Ch31_C", False)

validator.drawRatio(Data_hist, Ntup_hist, "h_Ratio_M2T5_Mid13Ch31_C_test")
validator.checkTrigNum()

# for MID in range(1,16) :
#     for ch in range(1, 33) : 
#         cid = pydrcTB.TBcid(MID, ch)
#         det = TButils.find(cid)
#         if not ( det.isSiPM() or det.isModule() ) :
#             print("MID : " + str(MID) + ", ch : " + str(ch) + " is enum : " + str(det.detType()))