import pydrcTB
import sys, os
import argparse
import ROOT

parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=str, required=True, help='Run number')
parser.add_argument('--FileNumber', '-fn', action='store', type=str, required=True, help='File nubver on run')

args = parser.parse_args()

rn=args.RunNumber
fn=args.FileNumber

WaveFiles=[]
FastFiles=[]

for mid in range(1,16):
    WaveDat=f"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_{rn}/Run_{rn}_Wave/Run_{rn}_Wave_MID_{mid}/Run_{rn}_Wave_MID_{mid}_FILE_{fn}.dat"
    FastDat=f"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/Run_{rn}_Fast_MID_{mid}_FILE_{fn}.dat"

    if(os.path.isfile):
        WaveFiles.append(WaveDat)
    else:
        print(WaveDat,"not found")

    if(os.path.isfile):
        FastFiles.append(FastDat)
    else:
        print(FastDat,"not found")

nameWave = f"ntuple_Run_{rn}_Wave_FILE_{fn}.root"
nameFast = f"ntuple_Run_{rn}_Fast_FILE_{fn}.root"

reader = pydrcTB.TBread()
reader.ntuplizeWaveform(WaveFiles,nameWave)

reader = pydrcTB.TBread()
reader.ntuplizeFastmode(FastFiles,nameFast)
