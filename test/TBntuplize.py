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
parser.add_argument('--output', '-o', action='store', type=str, default="./", help='Directory to write output ntuples')

args = parser.parse_args()

rn=args.runNumber
mode = "Fast" if args.doFast else "Wave"

# Only possible in KNU server ( for more info about using dcap : http://t2-cms.knu.ac.kr/wiki/index.php/Dcap )
# dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_Data

FileNumbers = []
for mid in range(1,16):
    DataPath=f"dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_Data/HDD_Run_{rn}_validated/Run_{rn}_{mode}/Run_{rn}_{mode}_MID_{mid}/"
    FileNumbers.append( len(glob(DataPath + "*.dat")) )
    maxFileNum = min(FileNumbers)

DataFiles = []
for fn in range(maxFileNum) :
    FilesPerFN = []
    for mid in range(1,16) :
        fileName=f"dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_Data/HDD_Run_{rn}_validated/Run_{rn}_{mode}/Run_{rn}_{mode}_MID_{mid}/Run_{rn}_{mode}_MID_{mid}_FILE_{fn}.dat"
        if(os.path.isfile(fileName)) :
            FilesPerFN.append(fileName)
        else:
            print(fileName,"not found")
    DataFiles.append(FilesPerFN)

dir_mode = "fastmode" if args.doFast else "waveform"
if args.output.endswith("/") : 
    outputPath = args.output + f"{dir_mode}/Run_{rn}/"
else :
    outputPath = args.output + f"/{dir_mode}/Run_{rn}/"
if not os.path.exists(outputPath) :
    os.makedirs(outputPath)
output_name = outputPath + f"ntuple_Run_{rn}_{mode}"

reader = pydrcTB.TBread()
if args.doFast : 
    reader.ntuplizeFastmode(DataFiles, output_name, args.maxEntry, args.fileEntry)
else :
    reader.ntuplizeWaveform(DataFiles, output_name, args.maxEntry, args.fileEntry)