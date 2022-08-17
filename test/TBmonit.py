import pydrcTB
import sys, os
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=str, required=True, help='Run number') 
parser.add_argument('--PedNumber', '-pn', action='store', type=str, required=True, help='Pedestal run number') 
parser.add_argument('--FileNumber', '-fn', action='store', type=str, required=True, help='File numver on run') 
parser.add_argument('--DistMaxADC', '-ma', action='store', type=float, required=True, help='Maximum for ADC distribution plot') 
parser.add_argument('--DistBinADC', '-ba', action='store', type=int, required=True, help='# of bin for ADC distribution') 

args = parser.parse_args()

baseDir = os.getcwd();
baseDir = os.path.join(baseDir, 'monit/Run'+args.RunNumber)

rn=args.RunNumber
fn=args.FileNumber

if not (os.path.exists(baseDir)) :
	os.makedirs(baseDir)

WaveFiles=[]
FastFiles=[]

for mid in range(1,16):
	WaveDat=f"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_{rn}/Run_{rn}_Wave/Run_{rn}_Wave_MID_{mid}/Run_{rn}_Wave_MID_{mid}_FILE_{fn}.dat"
	FastDat=f"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/Run_{rn}_Fast_MID_{mid}_FILE_{fn}.dat"
	if(os.path.isfile):WaveFiles.append(WaveDat)
	else:print(WaveDat,"not found")
	if(os.path.isfile):FastFiles.append(FastDat)
	else:print(FastDat,"not found")
		 
ma=args.DistMaxADC
ba=args.DistBinADC

monit = pydrcTB.TBmonit()
monit.setMappingPath("mapping_data_MCPPMT_positiveSignal.csv")
monit.setPedestalPath("ped_343.csv")
monit.setFastmodeFiles(FastFiles)
monit.setWaveformFiles(WaveFiles)
monit.SetADCmax(ma)
monit.SetADCbin(ba)

monit.MonitPlots()







