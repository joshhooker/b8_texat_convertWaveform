#define Convert_cxx
#include "Convert.h"

Double_t Shaper(Double_t *x, Double_t *p) {
    Double_t semiGaus = p[1]*21.928*TMath::Exp(-3.0*(x[0] - p[2])/p[3])*sin((x[0] - p[2])/p[3])*pow((x[0] - p[2])/p[3], p[4]);
    return (x[0] > p[2]) ? p[0] + x[0]*p[5] + semiGaus : p[0] + x[0]*p[5];
}

int main(int argc, char *argv[]) {

  // Home
  // TString PathToFiles = "/hd3/research/data/run0817a/rootM2R-WaveformModule/run";
  // TString PathToFiles = "/hd/research/data/run0817a/rootM2R-WaveformModule/run";

  // Mac Laptop
  TString PathToFiles = "/Users/joshhooker/Desktop/run";

  TChain *chain = new TChain("mfmData");

  chain->Add(PathToFiles + "158_normal.root");

  Convert t(chain);
  t.Loop();

  return 0;
}

void Convert::Loop() {
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    InitCanvas();

    TFile* file = new TFile("output.root", "recreate");

    auto* fpn = new TMultiGraph();
    auto* waveform = new TMultiGraph();

    Long64_t nbytes = 0, nb = 0;
    for(Long64_t jentry = 10; jentry < 11; jentry++) {
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

            if(cobo != 0) continue;
            if(asad != 0) continue;
            if(aget != 0) continue;

            std::cout << cobo << '\t' << asad << '\t' << aget << '\t' << chan << std::endl;

            if(chan == 11 || chan ==  22 || chan == 45 || chan == 56) {
              auto fpnGraph = new TGraph();

              for(int j = 0; j < 512; j++) {
                fpnGraph->SetPoint(j, j, mmWaveform[i][j]);
              }
              fpn->Add(fpnGraph);
            }
            else {
              auto waveGraph = new TGraph();
              for(int j = 0; j < 512; j++) {
                waveGraph->SetPoint(j, j, mmWaveform[i][j]);
              }
              waveform->Add(waveGraph);
            }

        }
    }

    fpn->SetName("fpn");
    fpn->Write();

    waveform->SetName("waveform");
    waveform->Write();

    file->Close();
}
