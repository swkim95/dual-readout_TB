#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "TBevt.h"
#include "TButility.h"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>

float getTime(float bin) {
  return 400. * (bin / 1000.);
}

int getPeakBin(std::vector<short> waveform) {
    return ( std::min_element(waveform.begin()+1, waveform.end()-23) - waveform.begin() );
}

float getPeakTime(std::vector<short> waveform) {
    int peakBin = getPeakBin(waveform);
    return (getTime(peakBin));
}

//////////////////////////////////////////////////////////////////////////////
// For DWC position
//////////////////////////////////////////////////////////////////////////////

std::vector<float> getDWC1position(std::vector<float> dwc1Time, float xOffset = 0.f, float yOffset = 0.f) {
    static float dwc1horizontalSlope = -0.1740806676;
    static float dwc1horizontalOffset = -0.1680572999;
    static float dwc1VerticalSlope = -0.17424779576;
    static float dwc1VerticalOffset = -0.053701300;

    std::vector<float> dwc1Position;

    float horizontalPos = -( ( (float)(dwc1Time.at(0) - dwc1Time.at(1)) * dwc1horizontalSlope ) + dwc1horizontalOffset ) - xOffset;
    float verticalPos = ( (float)(dwc1Time.at(2) - dwc1Time.at(3)) * dwc1VerticalSlope ) + dwc1VerticalOffset - yOffset;

    dwc1Position.push_back(horizontalPos);
    dwc1Position.push_back(verticalPos);

    return dwc1Position;
}

std::vector<float> getDWC2position(std::vector<float> dwc2Time, float xOffset = 0.f, float yOffset = 0.f) {
    static float dwc2horizontalSlope = -0.17257273;
    static float dwc2horizontalOffset = -0.579927452;
    static float dwc2VerticalSlope = -0.1741203164;
    static float dwc2VerticalOffset = -0.278179655;

    std::vector<float> dwc2Position;

    float horizontalPos = -( ( (float)(dwc2Time.at(0) - dwc2Time.at(1)) * dwc2horizontalSlope ) + dwc2horizontalOffset ) - xOffset;
    float verticalPos = ( (float)(dwc2Time.at(2) - dwc2Time.at(3)) * dwc2VerticalSlope ) + dwc2VerticalOffset - yOffset;

    dwc2Position.push_back(horizontalPos);
    dwc2Position.push_back(verticalPos);

    return dwc2Position;
}

std::vector<float> getDWC1positionUsingClock(std::vector<float> dwc1Time) {
    static float dwc1horizontalSlope = -0.1740806676;
    static float dwc1horizontalOffset = -0.1680572999;
    static float dwc1VerticalSlope = -0.17424779576;
    static float dwc1VerticalOffset = -0.053701300;

    std::vector<float> dwc1Position;

    float horizontalPos = -( ( (float)(dwc1Time.at(0) - dwc1Time.at(1)) * 25. / 1000. * dwc1horizontalSlope ) + dwc1horizontalOffset );
    float verticalPos = ( (float)(dwc1Time.at(2) - dwc1Time.at(3)) * 25. / 1000. * dwc1VerticalSlope ) + dwc1VerticalOffset;

    dwc1Position.push_back(horizontalPos);
    dwc1Position.push_back(verticalPos);

    return dwc1Position;
}

std::vector<float> getDWC2positionUsingClock(std::vector<float> dwc2Time) {
    static float dwc2horizontalSlope = -0.17257273;
    static float dwc2horizontalOffset = -0.579927452;
    static float dwc2VerticalSlope = -0.1741203164;
    static float dwc2VerticalOffset = -0.278179655;

    std::vector<float> dwc2Position;

    float horizontalPos = -( ( (float)(dwc2Time.at(0) - dwc2Time.at(1)) * 25. / 1000. * dwc2horizontalSlope ) + dwc2horizontalOffset );
    float verticalPos = ( (float)(dwc2Time.at(2) - dwc2Time.at(3)) * 25. / 1000. * dwc2VerticalSlope ) + dwc2VerticalOffset;

    dwc2Position.push_back(horizontalPos);
    dwc2Position.push_back(verticalPos);

    return dwc2Position;
}

std::vector<float> getDWCoffset(TH2D* dwcHist) {
    float xOffset = dwcHist->GetMean(1);
    float yOffset = dwcHist->GetMean(2);

    std::vector<float> dwcOffset;
    dwcOffset.push_back(xOffset);
    dwcOffset.push_back(yOffset);

    return dwcOffset;
}

std::vector<float> getPedCorrectedWaveform(std::vector<short> waveform, float ped) {
    std::vector<float> correctedWaveform;

    for (float binContent : waveform) {
        float binValue = ped - binContent;
        correctedWaveform.push_back(binValue);
    }

    return correctedWaveform;
}

//////////////////////////////////////////////////////////////////////////////
// For PID
//////////////////////////////////////////////////////////////////////////////

bool dwcCorrelationPID(std::vector<float> dwc1_correctedPosition, std::vector<float> dwc2_correctedPosition, float threshold = 2.f) {
    bool passed = false;

    float x_diff = std::abs( dwc1_correctedPosition.at(0) - dwc2_correctedPosition.at(0) );
    float y_diff = std::abs( dwc1_correctedPosition.at(1) - dwc2_correctedPosition.at(1) );

    if ( (x_diff <= threshold) && (y_diff <= threshold) ) passed = true;

    return passed;
}

//////////////////////////////////////////////////////////////////////////////
// For Leading edge method
//////////////////////////////////////////////////////////////////////////////
float getLinearInterpolatedX(float y, float x1, float x2, float y1, float y2) {
    return (x1 + (x2 - x1) * (y - y1) / (y2 - y1));
}

float getLinearInterpolatedY(float x, float x1, float x2, float y1, float y2) {
    return (y1 + (y2 - y1) * (x - x1) / (x2 - x1));
}

float getLeadingEdgeBin(std::vector<float> pedCorrected_waveform, float fraction = 0.2f) {
    float threshold = (float)*std::max_element(pedCorrected_waveform.begin()+1, pedCorrected_waveform.end()-23) * fraction;
    int peakBin = std::max_element(pedCorrected_waveform.begin()+1, pedCorrected_waveform.end()-23) - pedCorrected_waveform.begin();

    for (int bin = peakBin; bin > 0; bin--) {
        if (pedCorrected_waveform.at(bin) < threshold) {
            return getLinearInterpolatedX(threshold, bin, bin+1, pedCorrected_waveform.at(bin), pedCorrected_waveform.at(bin+1));
        }
    }

    return 0.f;
}

float getLeadingEdgeTime(std::vector<float> pedCorrected_waveform, float fraction = 0.2f) {
    float bin = getLeadingEdgeBin(pedCorrected_waveform, fraction);
    return getTime(bin);
}

//////////////////////////////////////////////////////////////////////////////
// print progress
//////////////////////////////////////////////////////////////////////////////
void printProgress(const int currentStep, const int totalStep) {
    float progress = (float) currentStep / totalStep;
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; i++) {
        if ( i < pos ) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "]  " << currentStep << "/" << totalStep << "  " << int(progress * 100.0) << "%\r";
    if ( (currentStep + 1) != totalStep) std::cout.flush();
    else {
        std::cout<< std::endl;
        std::cout<< "Finished!" << std::endl;
    }
}