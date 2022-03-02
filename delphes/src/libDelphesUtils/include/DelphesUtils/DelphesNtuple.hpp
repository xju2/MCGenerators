#ifndef __DELPHES_NTUPLE_H__
#define __DELPHES_NTUPLE_H__

#include "DelphesNtupleBase.hpp"
#include <vector>
#include <math.h>

class Jet;
class Track;
class Tower;
class GenParticle;

using namespace std;
class DelphesNtuple: public DelphesNtupleBase{
  public:
  DelphesNtuple(std::string& filename);
  ~DelphesNtuple();

  void Clear();
  void Fill();

  // Truth jets
  void BookGenJets();
  void FillGenJet(Jet* jet);
  void FillGenJetsCnt(int njets, int nbjets, int ntaujets);

  // Reco Jets
  void BookRecoJets(bool withTowers=false, bool withTauIDVars=false);
  void BookJetTauIDVars();
  void FillRecoJet(Jet* jet);  // Reco Jets
  void FillRecoJetCnt(int njets, int nbjets, int ntaujets);
  void FillRecoJetGhostTracks(vector<int>& trackIdx);
  void FillJetTauIDVars(
    Jet* jet, vector<int>& trackIdx,
    const TClonesArray* branchTrack
  );

  // Towers associated with Reco jets
  void BookJetTowers();
  void FillJetTower(Tower* tower);

  // Tracks
  void BookTracks();
  void FillTrack(const Track* track, const TClonesArray* branchParticle=nullptr);
  // return the parent and grand-parent PID of the track.
  void FindTrackTruth(const Track* track, const TClonesArray* branchParticle,
    Int_t& parent_PID, Int_t& grand_parent_PID);
  
  // Towers
  void BookTowers();
  void FillTower(Tower* tower);

  // Truth Taus
  void BookTruthTaus();
  void FillTruthTau(GenParticle* particle, const TClonesArray* branchParticle);

  // Distance
  float deltaR(float eta1, float phi1, float eta2, float phi2) {
    double dphi = phi2 - phi1;
    if (dphi > M_PI) dphi -= 2*M_PI;
    if (dphi < -M_PI) dphi += 2*M_PI;
    double deta = eta2 - eta1;
    return (float) sqrt(deta*deta + dphi*dphi);
  }

protected:

  // Truth Jet variables
  bool useTruthJets;
  int br_nTruthJets;
  int br_nTruthBJets;
  int br_nTruthTauJets;
  vector<float> br_truthJetPt;
  vector<float> br_truthJetEta;
  vector<float> br_truthJetPhi;
  vector<float> br_truthJetE;
  vector<int> br_truthJetIsBtagged;
  vector<int> br_truthJetIsTautagged;
  void ClearGenJets();

  // ====================
  // Reco Jet variables
  // ====================
  bool useRecoJets;
  int br_nRecoJets;
  int br_nRecoBJets;
  int br_nRecoTauJets;
  vector<float> br_recoJetPt;
  vector<float> br_recoJetEta;
  vector<float> br_recoJetPhi;
  vector<float> br_recoJetE;
  vector<int> br_recoJetIsBtagged;
  vector<int> br_recoJetIsTautagged;
  vector<int> br_recoJetNTowers;
  vector<int> br_recoJetNTracks;
  vector<int> br_recoJetGhostTracksN;
  void ClearRecoJets();

  // Towers associated with Jets
  bool useJetTowers;
  vector<float> br_jetTowerEt;
  vector<float> br_jetTowerEta;
  vector<float> br_jetTowerPhi;
  vector<float> br_jetTowerE;
  vector<float> br_jetTowerEem;
  vector<float> br_jetTowerEhad;
  void ClearJetTower();

  // high-level variables associated with reco. jets
  // those are taken from the ATLAS paper
  // https://arxiv.org/pdf/1412.7086.pdf
  bool useJetTauIDVars;
  vector<float>   br_jetCentralEFrac;
  vector<float>   br_jetLeadingTrackFracP;
  vector<float>   br_jetTrackR;
  vector<float>   br_jetLeadingTrackD0;
  vector<float>   br_jetLeadingTrackD0Sig;
  vector<int>     br_jetNumISOTracks;
  vector<float>   br_jetMaxDRInCore;
  vector<float>   br_jetTrackMass;
  void ClearJetTauIDVars();
  

  // Tracks associated with Jets with ghost matching
  bool useJetGhostTracks;
  vector<int> br_jetGhostTrackIdx;
  void BookGhostTracks();
  void ClearGhostTracks();

  // Tracks
  bool useTracks;
  int br_nTracks;
  vector<int> br_trackPID;
  vector<int> br_trackCharge;
  vector<float> br_trackEtaOut;
  vector<float> br_trackPhiOut;
  vector<float> br_trackPt;
  vector<float> br_trackEta;
  vector<float> br_trackPhi;
  vector<float> br_trackD0;
  vector<float> br_trackZ0;
  vector<int> br_trackVertexIdx;

  vector<int> br_trackParentPID;
  vector<int> br_trackGrandParentPID;

  void ClearTracks();

  // Towers
  bool useTowers;
  int br_nTowers;
  vector<float> br_towerEt;
  vector<float> br_towerEta;
  vector<float> br_towerPhi;
  vector<float> br_towerE;
  vector<float> br_towerEem;
  vector<float> br_towerEhad;
  void ClearTowers();


  // Truth Taus
  bool useTruthTaus;
  int br_nTruthTaus;
  vector<float> br_truthTauEt;
  vector<float> br_truthTauEta;
  vector<float> br_truthTauPhi;
  vector<float> br_truthTauE;
  vector<int>   br_truthTauCharge;
  vector<int>   br_truthTauStatus;
  vector<int>   br_truthTauParentPID;
  void ClearTruthTaus();
};

#endif