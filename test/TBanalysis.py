import ROOT
import pydrcTB

file = ROOT.TFile("test.root")
atree = file.Get("events")

utility = pydrcTB.TButility()
utility.loading("mapping_data_MCPPMT_positiveSignal.csv")
utility.loadped("sample_ped.csv")

channelsize = 32

for ievt in range(atree.GetEntries()):
    atree.GetEntry(ievt)
    anevt = getattr(atree,"TBevt")

    adc = 0.

    for imid in range(anevt.size()):
        for ich in range(channelsize):
            # TODO cannot convert ROOT TBcid & boost::python TBcid automatically
            cidboost = pydrcTB.TBcid( imid+1, ich+1 ) # mid 1 - 15, ch 1 - 32
            cidroot  = ROOT.TBcid( imid+1, ich+1 )

            adet = utility.find(cidboost)

            # example: sum adc of all scint SiPMs
            if (not adet.isNull()) and adet.isModule() and adet.isSiPM() and (not adet.isCeren()):
                adata = anevt.data( cidroot )
                adc += adata.adc()

    print(adc)
