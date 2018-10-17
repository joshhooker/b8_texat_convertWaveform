//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 21 09:32:38 2018 by ROOT version 6.15/01
// from TTree mfmData/Experimental Event Data
// found on file: /hd3/research/data/run0817a/rootM2R-WaveformModule/run122.root
//////////////////////////////////////////////////////////

#ifndef Convert_h
#define Convert_h

#include <TCanvas.h>
#include <TChain.h>
#include <TCutG.h>
#include <TF2.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraph2D.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TPolyLine3D.h>
#include <TROOT.h>
#include <TStyle.h>

#include <Fit/Fitter.h>

#include <Math/Functor.h>
#include <Math/Vector3D.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "WaveletNew.h"

// Header file for the classes stored in the TTree if any.

class Convert {
public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Int_t           mmMul;
    Int_t           mmCobo[850];   //[mmMul]
    Int_t           mmAsad[850];   //[mmMul]
    Int_t           mmAget[850];   //[mmMul]
    Int_t           mmChan[850];   //[mmMul]
    Float_t         mmTime[850];   //[mmMul]
    Float_t         mmEnergy[850];   //[mmMul]
    Float_t         mmWaveform[850][512];   //[mmMul]

    // List of branches
    TBranch        *b_mmMul;   //!
    TBranch        *b_mmCobo;   //!
    TBranch        *b_mmAsad;   //!
    TBranch        *b_mmAget;   //!
    TBranch        *b_mmChan;   //!
    TBranch        *b_mmTime;   //!
    TBranch        *b_mmEnergy;   //!
    TBranch        *b_mmWaveform;   //!

    Convert(TTree *tree=0);
    virtual ~Convert();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual void     Loop();
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);

    Int_t fMMMul;
    Float_t fMMTime[2176];
    Float_t fMMEnergy[2176];
    Int_t fMMAsad[2176];
    Int_t fMMCobo[2176];
    Int_t fMMAget[2176];
    Int_t fMMChan[2176];
    Float_t fMMPa[2176][5];

// CsI
private:
    std::pair<Float_t, Float_t> FitCsI(Float_t*);

// Wavelets
private:
    std::vector<Double_t> GetPa(Float_t*);
    std::vector<Double_t> widths;

// TCanvas

    void InitCanvas();

    // For central pads
    void DrawCanvas(Int_t count, Float_t* waveform);
    Int_t totalCanvas;
    Int_t canvasNum, canvasXYNum, canvasXNum, canvasYNum;
    TCanvas* canvas[5];
};

#endif

#ifdef Convert_cxx
Convert::Convert(TTree *tree) : fChain(0) {
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
    if (tree == 0) {
        TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/hd3/research/data/run0817a/rootM2R-WaveformModule/run122.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("/hd3/research/data/run0817a/rootM2R-WaveformModule/run122.root");
        }
    f->GetObject("mfmData",tree);

    }
    Init(tree);
}

Convert::~Convert() {
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t Convert::GetEntry(Long64_t entry) {
// Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

Long64_t Convert::LoadTree(Long64_t entry) {
// Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void Convert::Init(TTree *tree) {
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("mmMul", &mmMul, &b_mmMul);
    fChain->SetBranchAddress("mmCobo", mmCobo, &b_mmCobo);
    fChain->SetBranchAddress("mmAsad", mmAsad, &b_mmAsad);
    fChain->SetBranchAddress("mmAget", mmAget, &b_mmAget);
    fChain->SetBranchAddress("mmChan", mmChan, &b_mmChan);
    fChain->SetBranchAddress("mmTime", mmTime, &b_mmTime);
    fChain->SetBranchAddress("mmEnergy", mmEnergy, &b_mmEnergy);
    fChain->SetBranchAddress("mmWaveform", mmWaveform, &b_mmWaveform);
    Notify();
}

Bool_t Convert::Notify() {
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void Convert::Show(Long64_t entry) {
// Print contents of entry.
// If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}

Int_t Convert::Cut(Long64_t entry) {
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
    return 1;
}

inline void Convert::InitCanvas() {
    totalCanvas = 0;
    canvasNum = static_cast<Int_t>(sizeof(canvas)/sizeof(canvas[0]));
    canvasXNum = 4;
    canvasYNum = 4;
    canvasXYNum = canvasXNum*canvasYNum;
    for(Int_t i = 0; i < canvasNum; i++) {
        TString name = Form("center%d", i + 1);
        canvas[i] = new TCanvas(name, name, 1600, 1200);
        canvas[i]->Divide(canvasXNum, canvasYNum);
        canvas[i]->Update();
    }
}

inline void Convert::DrawCanvas(Int_t count, Float_t* waveform) {
  TGraph* graph = new TGraph();

  Int_t i = 0;
  for(UInt_t i = 0; i < 512; i++) {
    graph->SetPoint(i, i, waveform[i]);
  }

  graph->SetMarkerColor(1);
  graph->SetMarkerStyle(8);

  if(count < canvasNum*canvasXYNum) {
    Int_t histNum = count/canvasXYNum;
    canvas[histNum]->cd(count + 1 - histNum*canvasXYNum);
    graph->GetXaxis()->SetLimits(-1, 513);
    graph->Draw("al");
    canvas[histNum]->Update();
  }
}

#endif // #ifdef Convert_cxx
