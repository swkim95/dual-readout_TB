import pydrcTB
import sys, os
import argparse
import ROOT
from glob import glob

parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=str, required=True, help='Run number')
# parser.add_argument('--RunFolder', '-rf', action='store', type=str, default="", help='Destination to the folder containing run folders')

args = parser.parse_args()

rn=args.RunNumber
# fn=args.FileNumber

fastFileNumbers = []
for mid in range(1,16):
    FastDat=f"/Users/swkim/DRC/dual-readout_TB/Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/"
    fastFileNumbers.append( len(glob(FastDat + "*.dat")) )
    maxFileNum = min(fastFileNumbers)

fastFiles=[]
for fn in range(maxFileNum) :
    FastFilesPerFN = []
    for mid in range(1,16) :
        fileName=f"/Users/swkim/DRC/dual-readout_TB/Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/Run_{rn}_Fast_MID_{mid}_FILE_{fn}.dat"
        if(os.path.isfile(fileName)) :
            FastFilesPerFN.append(fileName)
        else:
            print(fileName,"not found")
    fastFiles.append(FastFilesPerFN)

# nameFast = f"ntuple_Run_{rn}_Fast_FILE_total.root"
nameFast = f"ntuple_Run_{rn}_Fast"

reader = pydrcTB.TBread()
reader.ntuplizeFastmode(fastFiles,nameFast, -1, 100000)
