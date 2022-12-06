import pydrcTB
import sys, os
import argparse
import ROOT
from glob import glob

parser = argparse.ArgumentParser()

parser.add_argument('--runNumber', '-rn', action='store', type=str, required=True, help='Run number')
parser.add_argument('--maxEntry', '-me', action='store', type=int, default=-1, help='Maximum # of entries process, unlimited if -1')
parser.add_argument('--fileEntry', '-fe', action='store', type=int, default=-1, help='# of entries per single ntuple, unlimited if -1')
parser.add_argument('--doFast', '-f', action='store_true', default=False, help='If true, make fast mode ntuple')
# parser.add_argument('--RunFolder', '-rf', action='store', type=str, default="", help='Destination to the folder containing run folders')

args = parser.parse_args()

rn=args.runNumber
# fn=args.FileNumber


## Do wave
if not args.doFast :
    waveFileNumbers = []
    for mid in range(1,16):
        WaveDat=f"/Users/swkim/DRC/dual-readout_TB/Run_{rn}/Run_{rn}_Wave/Run_{rn}_Wave_MID_{mid}/"
        waveFileNumbers.append( len(glob(WaveDat + "*.dat")) )
        maxWaveFileNum = min(waveFileNumbers)

    waveFiles=[]
    for fn in range(maxWaveFileNum) :
        WaveFilesPerFN = []
        for mid in range(1,16) :
            fileName=f"/Users/swkim/DRC/dual-readout_TB/Run_{rn}/Run_{rn}_Wave/Run_{rn}_Wave_MID_{mid}/Run_{rn}_Wave_MID_{mid}_FILE_{fn}.dat"
            if(os.path.isfile(fileName)) :
                WaveFilesPerFN.append(fileName)
            else:
                print(fileName,"not found")
        waveFiles.append(WaveFilesPerFN)

    # nameFast = f"ntuple_Run_{rn}_Fast_FILE_total.root"
    nameWave = f"ntuple_Run_{rn}_Wave"

    reader = pydrcTB.TBread()
    reader.ntuplizeWaveform(waveFiles, nameWave, args.maxEntry, args.fileEntry)

## Do fast
if args.doFast :
    fastFileNumbers = []
    for mid in range(1,16):
        FastDat=f"/Users/swkim/DRC/dual-readout_TB/Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/"
        fastFileNumbers.append( len(glob(FastDat + "*.dat")) )
        maxFastFileNum = min(fastFileNumbers)

    fastFiles=[]
    for fn in range(maxFastFileNum) :
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
    reader.ntuplizeFastmode(fastFiles, nameFast, args.maxEntry, args.fileEntry)