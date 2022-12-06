#include "TBread.h"
#include "TBmid.h"
#include "TBevt.h"

#include <stdexcept>

#include "TFile.h"
#include "TTree.h"

TBmidbase TBread::readMetadata(FILE* fp) {
  char data[64];
  int data_length;
  int run_number;
  int tcb_trig_type;
  int tcb_trig_number;
  long long tcb_trig_time;
  int mid;
  int local_trig_number;
  int local_trigger_pattern;
  long long local_trig_time;
  long long fine_time;
  long long coarse_time;
  int itmp;
  long long ltmp;

  // read header
  fread(data, 1, 64, fp);

  // data length
  data_length = data[0] & 0xFF;
  itmp = data[1] & 0xFF;
  itmp = itmp << 8;
  data_length = data_length + itmp;
  itmp = data[2] & 0xFF;
  itmp = itmp << 16;
  data_length = data_length + itmp;
  itmp = data[3] & 0xFF;
  itmp = itmp << 24;
  data_length = data_length + itmp;

  // run number
  run_number = data[4] & 0xFF;
  itmp = data[5] & 0xFF;
  itmp = itmp << 8;
  run_number = run_number + itmp;

  // trigger type
  tcb_trig_type = data[6] & 0xFF;

  // TCB trigger #
  tcb_trig_number = data[7] & 0xFF;
  itmp = data[8] & 0xFF;
  itmp = itmp << 8;
  tcb_trig_number = tcb_trig_number + itmp;
  itmp = data[9] & 0xFF;
  itmp = itmp << 16;
  tcb_trig_number = tcb_trig_number + itmp;
  itmp = data[10] & 0xFF;
  itmp = itmp << 24;
  tcb_trig_number = tcb_trig_number + itmp;

  // TCB trigger time
  fine_time = data[11] & 0xFF;
  fine_time = fine_time * 11;     // actually * (1000 / 90)
  coarse_time = data[12] & 0xFF;
  ltmp = data[13] & 0xFF;
  ltmp = ltmp << 8;
  coarse_time = coarse_time + ltmp;
  ltmp = data[14] & 0xFF;
  ltmp = ltmp << 16;
  coarse_time = coarse_time + ltmp;
  ltmp = data[15] & 0xFF;
  ltmp = ltmp << 24;
  coarse_time = coarse_time + ltmp;
  ltmp = data[16] & 0xFF;
  ltmp = ltmp << 32;
  coarse_time = coarse_time + ltmp;
  ltmp = data[17] & 0xFF;
  ltmp = ltmp << 40;
  coarse_time = coarse_time + ltmp;
  coarse_time = coarse_time * 1000;   // get ns
  tcb_trig_time = fine_time + coarse_time;

  // mid
  mid = data[18] & 0xFF;

  // local trigger #
  local_trig_number = data[19] & 0xFF;
  itmp = data[20] & 0xFF;
  itmp = itmp << 8;
  local_trig_number = local_trig_number + itmp;
  itmp = data[21] & 0xFF;
  itmp = itmp << 16;
  local_trig_number = local_trig_number + itmp;
  itmp = data[22] & 0xFF;
  itmp = itmp << 24;
  local_trig_number = local_trig_number + itmp;

  // local trigger #
  local_trigger_pattern = data[23] & 0xFF;
  itmp = data[24] & 0xFF;
  itmp = itmp << 8;
  local_trigger_pattern = local_trigger_pattern + itmp;
  itmp = data[25] & 0xFF;
  itmp = itmp << 16;
  local_trigger_pattern = local_trigger_pattern + itmp;
  itmp = data[26] & 0xFF;
  itmp = itmp << 24;
  local_trigger_pattern = local_trigger_pattern + itmp;

  // local trigger time
  fine_time = data[27] & 0xFF;
  fine_time = fine_time * 11;     // actually * (1000 / 90)
  coarse_time = data[28] & 0xFF;
  ltmp = data[29] & 0xFF;
  ltmp = ltmp << 8;
  coarse_time = coarse_time + ltmp;
  ltmp = data[30] & 0xFF;
  ltmp = ltmp << 16;
  coarse_time = coarse_time + ltmp;
  ltmp = data[31] & 0xFF;
  ltmp = ltmp << 24;
  coarse_time = coarse_time + ltmp;
  ltmp = data[32] & 0xFF;
  ltmp = ltmp << 32;
  coarse_time = coarse_time + ltmp;
  ltmp = data[33] & 0xFF;
  ltmp = ltmp << 40;
  coarse_time = coarse_time + ltmp;
  coarse_time = coarse_time * 1000;   // get ns
  local_trig_time = fine_time + coarse_time;

  auto amid = TBmidbase(tcb_trig_number,run_number,mid);
  amid.setTCB(tcb_trig_type,tcb_trig_number,tcb_trig_time);
  amid.setLocal(local_trig_number,local_trigger_pattern,local_trig_time);

  return std::move(amid);
}

TBmid<TBwaveform> TBread::readWaveform(FILE* fp) {
  const auto base = readMetadata(fp);

  short adc[32736];

  // read waveform
  fread(adc, 2, 32736, fp);

  unsigned channelsize = 32;

  auto amid = TBmid<TBwaveform>(base);
  std::vector<TBwaveform> waveforms;
  waveforms.reserve(channelsize);

  for (unsigned int idx = 0; idx < channelsize; idx++) {
    auto awave = TBwaveform();
    awave.setChannel(idx+1); // WARNING channel number 1 - 32
    awave.init();
    waveforms.emplace_back(awave);
  }

  // fill waveform for channel
  for (int i = 0; i < 1024; i++) {
    for (unsigned int idx = 0; idx < channelsize; idx++)
      waveforms.at(idx).fill(i,adc[i*32+idx]); // should be always 32 here
  }

  amid.setChannels(waveforms);

  return std::move(amid);
}

TBmid<TBfastmode> TBread::readFastmode(FILE* fp) {
  const auto base = readMetadata(fp);

  short data[96];
  int energy;
  int timing;

  // read fast data
  fread(data, 2, 96, fp);

  unsigned int channelsize = 32;

  auto amid = TBmid<TBfastmode>(base);
  std::vector<TBfastmode> fastmodes;
  fastmodes.reserve(channelsize);

  for (unsigned int idx = 0; idx < channelsize; idx++) {
    auto afast = TBfastmode();
    afast.setChannel(idx+1); // WARNING channel number 1 - 32

    // fill waveform for channel to plot
    energy = data[idx * 3 + 1] & 0xFFFF;
    energy = energy * 65536;
    energy = energy + (data[idx * 3] & 0xFFFF);

    timing = data[idx * 3 + 2] & 0xFFFF;

    afast.setAdc(energy);
    afast.setTiming(timing);

    fastmodes.emplace_back(afast);
  }

  amid.setChannels(fastmodes);

  return std::move(amid);
}

void TBread::ntuplizeWaveform(const boost::python::list& alist, const std::string& output) {
  std::vector<std::string> filenames;

  for (unsigned idx = 0; idx < boost::python::len(alist); idx++)
    filenames.push_back(boost::python::extract<std::string>(alist[idx]));

  // get # of events in file
  FILE* fp = fopen(filenames.front().c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  unsigned long long file_size = ftell(fp);
  fclose(fp);
  int nevt = file_size / 65536;

  std::vector<FILE*> files;
  files.reserve(filenames.size());

  for (unsigned int idx = 0; idx < filenames.size(); idx++)
    files.emplace_back( fopen(filenames.at(idx).c_str(), "rb") );

  auto anevt = TBevt<TBwaveform>();
  TFile* rootfile = TFile::Open(output.c_str(),"RECREATE");
  auto roottree = new TTree("events","waveform events");
  roottree->Branch("TBevt",&anevt);

  for (unsigned ievt = 0; ievt < nevt; ievt++) {
    // fill TBevt
    std::vector<TBmid<TBwaveform>> mids;
    mids.reserve(files.size());

    // reference mid
    TBmid<TBwaveform> midref = readWaveform(files.at(0));
    int refevt = midref.evt();
    mids.emplace_back(midref);
    anevt.setTCB(refevt);

    for (unsigned int idx = 1; idx < files.size(); idx++) {
      TBmid<TBwaveform> amid = readWaveform(files.at(idx));

      if (amid.evt()!=refevt) // TODO tcb_trig_number difference handling
        throw std::runtime_error("TCB trig numbers are different!");

      mids.emplace_back(amid);
    }

    anevt.set(mids);

    roottree->Fill();
  }

  for (unsigned int idx = 0; idx < files.size(); idx++)
    fclose(files.at(idx));

  rootfile->WriteTObject(roottree);
  rootfile->Close();
}

void TBread::ntuplizeFastmode(const boost::python::list& alist, const std::string& output) {
  // get max file number (fileNum 0 ~ fileNum N) >> numOfFiles == N+1
  const int numOfFiles = boost::python::len(alist);

  // Vector storing all files (initialized with size of all file numbers * all MIDs)
  // { {file0_MID1, file0_MID2, ... file0_MID15},
  //   {file1_MID1, file1_MID2,... file1_MID15},
  //   ...
  //   {fileN_MID1, fileN_MID2,... fileN_MID15} }
  // totalFiles[file number][MID] >> return single file name
  std::vector< std::vector<std::string> > totalFiles(numOfFiles, std::vector<std::string>(boost::python::len(alist[0])));
  for (unsigned fileNum = 0; fileNum < numOfFiles; fileNum++) {
    for (unsigned MID = 0; MID < boost::python::len(alist[fileNum]); MID++) {
      totalFiles[fileNum][MID] = boost::python::extract<std::string>(alist[fileNum][MID]);
    }
  }

  // get total # of MIDs
  const int numOfMID = totalFiles.front().size();

  // entryPerMID >> to get total number of events by each MIDs, store  minimum value among them.
  // This ensures that the event loop does not exceeds MID containing minimum # of evts
  // entryPerFile >> to get entries of single file, will use for jumping to next file in event loop.
  std::vector<int> entryPerMID(numOfMID);
  std::vector< std::vector<int> > entryPerFile( numOfFiles, std::vector<int>(numOfMID) );
  for (unsigned fileNum = 0; fileNum < numOfFiles; fileNum++) {
    for (unsigned MID = 0; MID < numOfMID; MID++) {
      FILE* fp = fopen(totalFiles[fileNum][MID].c_str(), "rb");
      fseek(fp, 0L, SEEK_END);
      int file_size = ftell(fp);
      fclose(fp);
      entryPerMID[MID] += (int)(file_size / 256);
      entryPerFile[fileNum][MID] = (int)(file_size / 256);
    }
  }
  int minTotalEventNum = *std::min_element(entryPerMID.begin(), entryPerMID.end());
  std::cout << "Total entry = " << minTotalEventNum << std::endl;

  // create container for the binary files, total # of files = MIDs * FileNums
  std::vector< std::vector<FILE*> > files( numOfFiles, std::vector<FILE*>(numOfMID) );
  for (unsigned fileNum = 0; fileNum < numOfFiles; fileNum++) {
    for (unsigned MID = 0; MID < numOfMID; MID++) {
      files[fileNum][MID] = fopen(totalFiles[fileNum][MID].c_str(), "rb");
    }
  }

  // setting ntuple ROOT file
  auto anEvtData = TBevt<TBfastmode>();
  TFile* rootfile = TFile::Open(output.c_str(),"RECREATE");
  auto roottree = new TTree("events","fastmode events");
  roottree->Branch("TBevt",&anEvtData);
  roottree->SetAutoSave(0);

  // vector containing currently opened file number for each MID
  // vector containing event passed for currently opened file for each MID
  std::vector<int> currentOpenFileNum(numOfMID);
  std::vector<int> entryCounted(numOfMID);
  // Loop over total events to fill ntuple ROOT file
  for (unsigned iEvt = 0; iEvt < minTotalEventNum; iEvt++) {
    // if count[MID] exceeds # of entry of corresponding file, move to next file
    for (unsigned MID = 0; MID < numOfMID; MID++) {
      // currently opened file number for each MID
      int fileNum = currentOpenFileNum[MID];
      if ( entryPerFile[fileNum][MID] == entryCounted[MID] ) {
        currentOpenFileNum[MID]++;
        entryCounted[MID] = 0;
      }
    }

    // fill TBevt
    // vector containing fastmode MID info
    std::vector<TBmid<TBfastmode>> MIDs(numOfMID);
    // MIDs.reserve(numOfMID);

    // Use MID 1 as reference MID
    TBmid<TBfastmode> RefMID = readFastmode(files[currentOpenFileNum[0]][0]);
    MIDs.emplace_back(RefMID);
    anEvtData.setTCB(RefMID.evt());
    // Loop over MID 2 ~ 15 & fill MIDs vector
    for (unsigned MID = 1; MID < numOfMID; MID++) {
      int fileNum = currentOpenFileNum[MID];
      TBmid<TBfastmode> aMID = readFastmode(files[fileNum][MID]);
      // throw error if evt from MID 2 ~ 15 differs from MID 1 evt
      if (aMID.tcb_trig_number() != RefMID.tcb_trig_number()) {
        std::cout << "[Error] Current event number : " << iEvt << std::endl;
        std::cout << "[Error] TCB trig num of MID " << MID << " FileNum " << fileNum << " is " << aMID.tcb_trig_number() << std::endl;
        std::cout << "[Error] TCB trig num of MID 1 FileNum " << currentOpenFileNum[0] << " is " << RefMID.tcb_trig_number() << std::endl;
        throw std::runtime_error("TCB trig numbers are different!");
        }
      MIDs[MID] = aMID;
    }
    anEvtData.set(MIDs);
    roottree->Fill();

    // end of evt loop >> increase # of entry counted per file
    for (unsigned MID = 0; MID < numOfMID; MID++) entryCounted[MID]++;

    // print progress
    float progress = (float) iEvt / minTotalEventNum;
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; i++) {
      if ( i < pos ) std::cout << "=";
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
    }
    std::cout << "]  " << iEvt << "/" << minTotalEventNum << "  " << int(progress * 100.0) << "%\r";
    std::cout.flush();
  }
  
  // closing files
  for (unsigned fileNum = 0; fileNum < numOfFiles; fileNum++) {
    for (unsigned MID = 0; MID < numOfMID; MID++) {
      fclose(files[fileNum][MID]);
    }
  }

  // wite Ntuple
  rootfile->WriteTObject(roottree);
  rootfile->Close();
}
