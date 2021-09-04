#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootResult.h"


#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

#include "DelphesParquet/EventFile.hpp"
#include "DelphesUtils/GhostAssociation.hpp"
#include <unistd.h>
#include <iostream>

using namespace std;

void AnalysisEvents(ExRootTreeReader* treeReader, EventFile* ntuple, bool debug=false)
{
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchGenJet = treeReader->UseBranch("GenJet");

  Long64_t allEntries = treeReader->GetEntries();
  printf("Total %lld entries\n", allEntries);

  Long64_t entry;
  Jet* jet;
  TObject *object;

  GenParticle *particle;
  Track *track;
  Tower *tower;

  Int_t i, j, pdgCode;
  unsigned k;
  float jetRadius, jetPtmin;
  jetRadius = 0.4, jetPtmin = 20.0;
  GhostAssociation::Config jetConfig = {jetRadius, jetPtmin, fastjet::antikt_algorithm};

  for(entry = 0; entry < allEntries; ++entry) {
    treeReader->ReadEntry(entry);
  
    int n_jets=0, n_bjets=0, n_taujets=0;
    for(i = 0; i < branchGenJet->GetEntriesFast(); ++i) {
      jet = (Jet*) branchGenJet->At(i);
      n_jets ++;

      if (debug) printf("Truth Jet: %d, %d, %.2f %.2f %.2f\n", i, jet->Constituents.GetEntriesFast(), jet->PT, jet->Eta, jet->Phi);
      bool hasB = false;
      bool hasTau = false;
      int n_prongs = 0;
      for(j = 0; j < jet->Constituents.GetEntriesFast(); ++j) {
        object = jet->Constituents.At(j);
        if(object == 0) continue;
        if(object->IsA() == GenParticle::Class())
        {
          particle = (GenParticle*) object;
          GenParticle* m1 = (GenParticle*) branchParticle->At(particle->M1);
          if(debug) cout << "    GenPart pt: " << particle->PT << ", eta: " << particle->Eta << ", phi: " \
            << particle->Phi << ", ID: " << particle->PID << ", M1: " << m1->PID << endl;
          if(m1 && abs(m1->PID) == 15) {
            hasTau = true;
            n_prongs ++;
          } 
          if(m1 && abs(m1->PID) == 5) hasB = true;
        }
      }
      if (hasTau) {
        n_taujets ++; jet->TauTag = n_prongs;
      } else jet->TauTag = 0;

      if (hasB) {
        n_bjets ++; jet->BTag = 1;
      }
    }
    ntuple->fill({entry, n_jets, n_bjets, n_taujets});
  }
  ntuple->close();
}

int main(int argc, char** argv)
{
  bool help = false;
  bool debug = false;
  int opt;
  std::string file("/global/cfs/cdirs/m3443/usr/xju/TauStudies/run_Ztautau/Ntuple_ditau.root");
  std::string outname("test.parquet");

  while ((opt = getopt(argc, argv, "hf:o:d")) != -1) {
    switch(opt) {
    case 'f':
      file = optarg;
      break;
    case 'o':
      outname = optarg;
      break;
    case 'd':
      debug = true;
      break;
    case 'h':
      help = true;
    default:
      fprintf(stderr, "Usage: %s [-f FILE] [-o OUTNAME] [-h] [-d]\n", argv[0]);
      if (help) {
        printf("   -f FILE : read input file\n");
        printf("   -o OUTNAME: output anme\n");
        printf("   -h HELP: help message\n");
        printf("   -d DEBUG: print debug messages\n");
      }
      exit(1);
    }
  }

  TChain *chain = new TChain("Delphes");
  chain->Add(file.c_str());
  printf("Processing %s\n", file.c_str());

  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);

  auto parquet = new EventFile();
  parquet->init(outname);

  AnalysisEvents(treeReader, parquet, debug);

  delete parquet;
  delete chain;
  delete treeReader;

  return 0;
}