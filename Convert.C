#define Convert_cxx
#include "Convert.h"

Double_t Shaper(Double_t *x, Double_t *p) {
    Double_t semiGaus = p[1]*21.928*TMath::Exp(-3.0*(x[0] - p[2])/p[3])*sin((x[0] - p[2])/p[3])*pow((x[0] - p[2])/p[3], p[4]);
    return (x[0] > p[2]) ? p[0] + x[0]*p[5] + semiGaus : p[0] + x[0]*p[5];
}

int main(int argc, char *argv[]) {

  // Home
  // TString PathToFiles = "/hd3/research/data/run0817a/rootM2R-WaveformModule/run";
  TString PathToFiles = "/hd/research/data/run0817a/rootM2R-WaveformModule/run";

  // Mac Laptop
  //TString PathToFiles = "/Users/joshhooker/Desktop/run";

  PathToFiles += argv[1];
  PathToFiles += ".root";

  std::cout << PathToFiles << std::endl;

  TFile *f = new TFile(PathToFiles);
  TTree *inTree = (TTree*)f->Get("mfmData");

  // TChain *chain = MakeChain();

  Convert t(inTree);
  t.Loop();

  // delete tree;
  // delete f;

  return 0;
}

void Convert::Loop() {
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    InitCanvas();

    widths.push_back(4); widths.push_back(8);
    widths.push_back(16); widths.push_back(32);
    widths.push_back(64);

    TFile* file = new TFile("output.root", "recreate");

    TTree* tree = new TTree("mfmData", "Experimental Event Data");
    tree->Branch("mmMul", &fMMMul, "mmMul/I");
    tree->Branch("mmCobo", fMMCobo, "mmCobo[mmMul]/I");
    tree->Branch("mmAsad", fMMAsad, "mmAsad[mmMul]/I");
    tree->Branch("mmAget", fMMAget, "mmAget[mmMul]/I");
    tree->Branch("mmChan", fMMChan, "mmChan[mmMul]/I");
    tree->Branch("mmTime", fMMTime, "mmTime[mmMul]/F");
    tree->Branch("mmEnergy", fMMEnergy, "mmEnergy[mmMul]/F");
    tree->Branch("mmPa", fMMPa, "mmPa[mmMul][5]/F");

    Long64_t nbytes = 0, nb = 0;
    for(Long64_t jentry = 0; jentry < nentries; jentry++) {
    // for(Long64_t jentry = 0; jentry < 50; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        if(jentry != 0 && jentry % 50 == 0) printf("Processed %lld events\n", jentry);

        for(Int_t i = 0; i < mmMul; i++) {
            Int_t cobo = mmCobo[i];
            Int_t asad = mmAsad[i];
            Int_t aget = mmAget[i];
            Int_t chan = mmChan[i];
            Float_t time = mmTime[i];
            Float_t energy = mmEnergy[i];

            std::vector<Double_t> pa = GetPa(mmWaveform[i]);

            if(cobo == 1 && asad == 1 && aget == 3 && energy > 2800) {
                std::pair<Float_t, Float_t> newCsI = FitCsI(mmWaveform[i]);
                energy = newCsI.first;
                time = newCsI.second;
            }

            fMMCobo[i] = cobo;
            fMMAsad[i] = asad;
            fMMAget[i] = aget;
            fMMChan[i] = chan;
            fMMTime[i] = time;
            fMMEnergy[i] = energy;
            for(UInt_t j = 0; j < 5; j++) {
                fMMPa[i][j] = pa[j];
            }
        }

        fMMMul = mmMul;

        tree->Fill();
    }

    tree->Write();
    file->Close();
}

std::pair<Float_t, Float_t> Convert::FitCsI(Float_t* waveform) {
    std::vector<Float_t> newWaveformX, newWaveformXErr, newWaveformY, newWaveformYErr;
    for(UInt_t i = 100; i < 470; i++) {
        if(waveform[i] > 1) {
            newWaveformX.push_back(i);
            newWaveformXErr.push_back(0);
            newWaveformY.push_back(waveform[i]);
            newWaveformYErr.push_back(sqrt(waveform[i]));
        }
    }

    TF1* Sfunc = new TF1("shape", Shaper, newWaveformX[0], newWaveformX[newWaveformX.size() - 1], 6);
    Sfunc->SetParNames("offset", "amplitude", "peakAt", "sigma", "power", "p2");
    Sfunc->SetParameters(10, 500, 200, 130, 3, 0.2);
    Sfunc->SetParLimits(1, 0, 10000);
    Sfunc->SetParLimits(3, 80, 250);

    TGraphErrors *gErr = new TGraphErrors(newWaveformX.size(), &newWaveformX[0], &newWaveformY[0], &newWaveformXErr[0], &newWaveformYErr[0]);

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000000);
    ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(1000000);
    ROOT::Math::MinimizerOptions::SetDefaultTolerance(0.001);

    gErr->Fit("shape", "QR");

    Float_t maxWaveformX = 0;
    Float_t maxWaveform = 0.;
    for(UInt_t i = newWaveformX[0]; i < newWaveformX[newWaveformX.size() - 1]; i++) {
        Float_t eval = Sfunc->Eval(i);
        if(eval > maxWaveform) {
            maxWaveform = eval;
            maxWaveformX = i*40.;
        }
    }

    delete Sfunc;
    delete gErr;

    return std::make_pair(maxWaveform, maxWaveformX);
}

std::vector<Double_t> Convert::GetPa(Float_t* waveform) {
    std::vector<Double_t> waveform_;
    Float_t maxValue = -1.;
    for(UInt_t i = 0; i < 512; i++) {
        if(waveform[i] > maxValue) {
            maxValue = waveform[i];
        }
    }
    for(UInt_t i = 0; i < 512; i++) {
        waveform_.push_back(waveform[i]/maxValue);
    }

    WaveletNew* wavelet = new WaveletNew(waveform_, widths, true);
    wavelet->CalcCWTFast();
    std::vector<Double_t> scale = wavelet->GetScale();
    std::vector<Double_t> pa = wavelet->GetPa();
    delete wavelet;

    return pa;
}
