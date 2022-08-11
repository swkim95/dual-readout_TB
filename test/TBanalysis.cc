#include <iostream>
#include <stdexcept>

#include "TBevt.h"
#include "TButility.h"

#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

// compile rule
// g++ -I<path_to_include_dir> -L<path_to_lib_dir> <path_to_libdrcTB.so> `root-config --cflags --libs` TBanalysis.cc -o TBanalysis
// example
// g++ -I/afs/cern.ch/work/s/sako/public/kfc-dream/dual-readout_TB/install/include -L/afs/cern.ch/work/s/sako/public/kfc-dream/dual-readout_TB/install/lib64 /afs/cern.ch/work/s/sako/public/kfc-dream/dual-readout_TB/install/lib64/libdrcTB.so `root-config --cflags --libs` TBanalysis.cc -o TBanalysis

int main(int argc, char** argv) {
  TFile* afile = new TFile("test.root");
  TTree* atree = (TTree*)afile->Get("events");

  TBevt<TBfastmode>* anevt = new TBevt<TBfastmode>();
  atree->SetBranchAddress("TBevt",&anevt);

  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal.csv");
  utility.loadped("sample_ped.csv");

  unsigned channelsize = 32;

  for (unsigned ievt = 0; ievt < atree->GetEntries(); ievt++) {
    atree->GetEntry(ievt);

    double adc = 0.;

    for (int nmid = 0; nmid < anevt->size(); nmid++) {
      for (int ch = 0; ch < channelsize; ch++) {
        TBcid cid = TBcid(nmid+1,ch+1); // mid 1 - 15, ch 1 - 32
        TBdetector det = utility.find(cid);

        // example: sum adc of all scint SiPMs
        if (!det.isNull() && det.isModule() && det.isSiPM() && !det.isCeren()) {
          TBfastmode adata = anevt->data(cid);
          adc += adata.adc();
        }
      } // channel loop
    } // mid loop

    std::cout << adc << std::endl;
  } // event loop

  return 0;
}
