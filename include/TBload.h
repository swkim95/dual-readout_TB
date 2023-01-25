#ifndef TBload_h
#define TBload_h 1

#include <vector>
#include <string>

#include "TBmid.h"
#include "TBevt.h"
#include "TBdetector.h"

#include <TChain.h>
#include <TTree.h>

enum datamode
{
	kFastmode = 0,
	kWaveform
};

template <class T>
class TBload
{
public:

	TBload(int runNum_);
	~TBload();

	void loading();
	int currenLoader() { return currentEntry; }
	int getEntries() { return entries; }
	TBevt<T>* getEvent() { return aEvent; }

	void close();
	
private:

	void init();
	datamode getType(TBevt<TBwaveform>* evt) { return datamode::kWaveform; }
	datamode getType(TBevt<TBfastmode>* evt) { return datamode::kFastmode; }

	int runNum;

	int entries;
	int currentEntry;

	TChain* aChain;
	TBevt<T>* aEvent;
};

#endif