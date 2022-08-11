import ROOT

file = ROOT.TFile("test.root")
atree = file.Get("events")

achannel = ROOT.TBcid(1,2)

for ievt in range(atree.GetEntries()):
    atree.GetEntry(ievt)
    anevt = getattr(atree,"TBevt")
    adata = anevt.data(achannel)
    print(adata.adc()) # fastmode
