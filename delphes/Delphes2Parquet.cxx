#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootResult.h"

#include "DelphesUtils/DelphesParquet.hpp"
#include <unistd.h>
#include <iostream>
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

  // TChain *chain = new TChain("Delphes");
  // chain->Add(file.c_str());
  // printf("Processing %s\n", file.c_str());

  auto parquet = new DelphesParquet(outname);
  delete parquet;

  return 0;
}