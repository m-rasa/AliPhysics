/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.
 *                                                                        *
 * Authors: Omar Vazquez (omar.vazquez.rueda@cern.ch)                     *
 **************************************************************************/

/* This source code yields the histograms for the measurement of the
 * speed of sound using very central Pb-Pb collisions.
 */

class TTree;

class AliPPVsMultUtils;
class AliESDtrackCuts;

#include <AliAnalysisFilter.h>
#include <AliESDVertex.h>
#include <AliHeader.h>
#include <AliMultiplicity.h>
#include <TBits.h>
#include <TDirectory.h>
#include <TMath.h>
#include <TRandom.h>
#include <TTree.h>

#include <iostream>
#include <vector>

#include "AliAnalysisManager.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisTaskESDfilter.h"
#include "AliAnalysisTaskSE.h"
#include "AliAnalysisUtils.h"
#include "AliCentrality.h"
#include "AliESDEvent.h"
#include "AliESDInputHandler.h"
#include "AliESDUtils.h"
#include "AliESDVZERO.h"
#include "AliESDtrack.h"
#include "AliESDtrackCuts.h"
// #include "AliEventCuts.h"
#include "AliGenCocktailEventHeader.h"
#include "AliGenEventHeader.h"
#include "AliInputEventHandler.h"
#include "AliLog.h"
#include "AliMCEvent.h"
#include "AliMCEventHandler.h"
// #include "AliMCParticle.h"
#include "AliMultEstimator.h"
#include "AliMultInput.h"
#include "AliMultSelection.h"
#include "AliMultVariable.h"
#include "AliMultiplicity.h"
#include "AliOADBContainer.h"
#include "AliOADBMultSelection.h"
#include "AliPPVsMultUtils.h"
#include "AliStack.h"
#include "AliVEvent.h"
#include "AliVTrack.h"
#include "AliVVertex.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TLegend.h"
#include "TList.h"
#include "TMath.h"
#include "TParticle.h"
#include "TProfile.h"
#include "TVector3.h"

using std::cout;
using std::endl;
using std::vector;

static constexpr int v0m_Nbins{1};
// static constexpr double v0m_bins[v0m_Nbins + 1] = {0.0, 5.0};
static constexpr double uc_v0m_bins_high[v0m_Nbins] = {5.0};
static constexpr double uc_v0m_bins_low[v0m_Nbins] = {0.0};

#include "AliAnalysisTaskZNZP.h"

class AliAnalysisTaskZNZP;  // your analysis class

ClassImp(AliAnalysisTaskZNZP)  // classimp: necessary for root

    AliAnalysisTaskZNZP::AliAnalysisTaskZNZP()
    : AliAnalysisTaskSE(),
      fESD(0),
      fEventCuts(0x0),
      fMCStack(0),
      fMC(0),
      fUseMC(kFALSE),
      fIsSystematics(true),
      fVaryVtxZPos(false),
      fMinVtxZPos(-5.0),
      fMaxVtxZPos(5.0),
      fSystematic(1),
      fTrigger(AliVEvent::kCentral),
      fMultSelection(0x0),
      fTrackFilter(0x0),
      fTrackFilterwoDCA(0x0),
      fOutputList(0),
      fEtaCut(0.8),
      fPtMin(0.15),
      fV0Mmin(0.0),
      fV0Mmax(80.0),
      ftrackmult08(0),
      fv0mpercentile(0),
      fv0mamplitude(0),
      pV0MAmpChannel(0),
      /*hV0MAmplitude(0),*/
      hV0Percentile(0),
      hBestVtxZ(0),
      hZNvsV0MPer(0),
      hZNvsV0M(0),
      hZNAvsV0M(0),
      hZNCvsV0M(0),
      hAsyN(0),
      /*pZNvsV0MAmp(0),*/
      hZPvsV0MPer(0),
      hZPvsV0M(0),
      hZPAvsV0M(0),
      hZPCvsV0M(0),
      hAsyP(0),
      /*pZPvsV0MAmp(0),*/
      /*hZNvsV0MAmp(0),*/
      /*hZPvsV0MAmp(0),*/
      hZNCpmc(0),
      hZNApmc(0),
      hZPCpmc(0),
      hZPApmc(0) {}
//_____________________________________________________________________________
AliAnalysisTaskZNZP::AliAnalysisTaskZNZP(const char* name)
    : AliAnalysisTaskSE(name),
      fESD(0),
      fEventCuts(0x0),
      fMCStack(0),
      fMC(0),
      fUseMC(kFALSE),
      fIsSystematics(true),
      fVaryVtxZPos(false),
      fMinVtxZPos(-5.0),
      fMaxVtxZPos(5.0),
      fSystematic(1),
      fTrigger(AliVEvent::kCentral),
      fMultSelection(0x0),
      fTrackFilter(0x0),
      fTrackFilterwoDCA(0x0),
      fOutputList(0),
      fEtaCut(0.8),
      fPtMin(0.15),
      fV0Mmin(0.0),
      fV0Mmax(80.0),
      ftrackmult08(0),
      fv0mpercentile(0),
      fv0mamplitude(0),
      pV0MAmpChannel(0),
      /*hV0MAmplitude(0),*/
      hV0Percentile(0),
      hBestVtxZ(0),
      hZNvsV0MPer(0),
      hZNvsV0M(0),
      hZNAvsV0M(0),
      hZNCvsV0M(0),
      hAsyN(0),
      /*pZNvsV0MAmp(0),*/
      hZPvsV0MPer(0),
      hZPvsV0M(0),
      hZPAvsV0M(0),
      hZPCvsV0M(0),
      hAsyP(0),
      /*pZPvsV0MAmp(0),*/
      /*hZNvsV0MAmp(0),*/
      /*hZPvsV0MAmp(0),*/
      hZNCpmc(0),
      hZNApmc(0),
      hZPCpmc(0),
      hZPApmc(0) {
  DefineInput(0, TChain::Class());  // define the input of the analysis: in this
                                    // case you take a 'chain' of events
  // this chain is created by the analysis manager, so no need to worry about
  // it, does its work automatically
  DefineOutput(1, TList::Class());  // define the ouptut of the analysis: in
                                    // this case it's a list of histograms
}
//_____________________________________________________________________________
AliAnalysisTaskZNZP::~AliAnalysisTaskZNZP() {
  // destructor
  if (fOutputList) {
    delete fOutputList;  // at the end of your task, it is deleted from memory
                         // by calling this function
    fOutputList = 0x0;
  }
}

//_____________________________________________________________________________
void AliAnalysisTaskZNZP::UserCreateOutputObjects() {
  if (!fTrackFilter) {
    fTrackFilter = new AliAnalysisFilter("trackFilter2015");
    AliESDtrackCuts* fCuts = new AliESDtrackCuts();
    fCuts->SetMaxFractionSharedTPCClusters(0.4);
    fCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    fCuts->SetCutGeoNcrNcl(3., 130., 1.5, 0.85, 0.7);
    fCuts->SetMaxChi2PerClusterTPC(4);
    fCuts->SetAcceptKinkDaughters(kFALSE);
    fCuts->SetRequireTPCRefit(kTRUE);
    fCuts->SetRequireITSRefit(kTRUE);
    fCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,
                                    AliESDtrackCuts::kAny);
    fCuts->SetMaxDCAToVertexXYPtDep("0.0182+0.0350/pt^1.01");
    fCuts->SetMaxChi2TPCConstrainedGlobal(36);
    fCuts->SetMaxDCAToVertexZ(2);
    fCuts->SetDCAToVertex2D(kFALSE);
    fCuts->SetRequireSigmaToVertex(kFALSE);
    fCuts->SetMaxChi2PerClusterITS(36);
    fCuts->SetEtaRange(-0.8, 0.8);

    if (fIsSystematics) {
      ChangeCut(fCuts);
    }
    fTrackFilter->AddCuts(fCuts);
  }

  // track cuts to find contamination via DCA distribution
  if (!fTrackFilterwoDCA) {
    fTrackFilterwoDCA = new AliAnalysisFilter("trackFilter2015");
    AliESDtrackCuts* fCuts3 = new AliESDtrackCuts();
    fCuts3->SetMaxFractionSharedTPCClusters(0.4);                //
    fCuts3->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);  //
    fCuts3->SetCutGeoNcrNcl(3., 130., 1.5, 0.85, 0.7);           //
    fCuts3->SetMaxChi2PerClusterTPC(4);                          //
    fCuts3->SetAcceptKinkDaughters(kFALSE);                      //
    fCuts3->SetRequireTPCRefit(kTRUE);                           //
    fCuts3->SetRequireITSRefit(kTRUE);                           //
    fCuts3->SetClusterRequirementITS(AliESDtrackCuts::kSPD,
                                     AliESDtrackCuts::kAny);  //
    // fCuts3->SetMaxDCAToVertexXYPtDep("0.0182+0.0350/pt^1.01");//
    // fCuts3->SetMaxChi2TPCConstrainedGlobal(36);//
    fCuts3->SetMaxDCAToVertexZ(2);            //
    fCuts3->SetDCAToVertex2D(kFALSE);         //
    fCuts3->SetRequireSigmaToVertex(kFALSE);  //
    fCuts3->SetMaxChi2PerClusterITS(36);      //
    fCuts3->SetEtaRange(-0.8, 0.8);

    if (fIsSystematics) {
      ChangeCut(fCuts3);
    }
    fTrackFilterwoDCA->AddCuts(fCuts3);
  }

  if (fVaryVtxZPos) {
    fEventCuts.SetManualMode();  //! Enable manual mode
    fEventCuts.fMinVtz = fMinVtxZPos;
    fEventCuts.fMaxVtz = fMaxVtxZPos;
  }

  // create output objects
  OpenFile(1);
  fOutputList = new TList();
  fOutputList->SetOwner(kTRUE);

  /*constexpr double v0mAmp_width{25.0};*/
  /*constexpr int v0mAmp_Nbins{1720};*/
  /*double v0mAmp_bins[v0mAmp_Nbins + 1] = {0};*/
  /*for (int i = 0; i <= v0mAmp_Nbins; ++i) {*/
  /*  v0mAmp_bins[i] = 0.0 + i * v0mAmp_width;*/
  /*}*/

  const int nBinsV0M090{90};
  double BinsV0M090[nBinsV0M090 + 1] = {0.0};
  for (int i = 0; i <= nBinsV0M090; ++i) {
    BinsV0M090[i] = 0.0 + (double)i;
  }

  /*constexpr int v0m_Nbins080{6};*/
  /*constexpr double v0m_bins080[v0m_Nbins080 + 1] = {0.0,  1.0,  5.0, 10.0,*/
  /*                                                  20.0, 50.0, 80.0};*/

  hV0Percentile = new TH1F("hV0M", ";V0M (%);Entries", nBinsV0M090, BinsV0M090);
  pV0MAmpChannel =
      new TProfile("pV0Channel", ";Channel; Amplitude;", 64, -0.5, 63.5);
  /*hV0MAmplitude =*/
  /*    new TH1F("hV0MAmp", ";V0M amplitude;Counts", v0mAmp_Nbins,
   * v0mAmp_bins);*/
  hBestVtxZ =
      new TH1F("hBestVtxZ", ";Vertex_{#it{z}} (cm); Counts;", 400, -11, 11);

  hZNvsV0M = new TH2F("hZNvsV0M", "ZNA+ZNC;V0M Per; #it{E}_{ZN} [TeV]/2.511;",
                      nBinsV0M090, BinsV0M090, 200, 0.0, 200.0);
  hZPvsV0M = new TH2F("hZPvsV0M", "ZPA+ZPC;V0M Per; #it{E}_{ZP} [TeV]/2.511;",
                      nBinsV0M090, BinsV0M090, 60, 0.0, 60.0);

  hZNAvsV0M = new TH2F("hZNAvsV0M", "ZNA;V0M Per; #it{E}_{ZN} [TeV]/2.511;",
                       nBinsV0M090, BinsV0M090, 100, 0.0, 100.0);
  hZNCvsV0M = new TH2F("hZNCvsV0M", "ZNC;V0M Per; #it{E}_{ZN} [TeV]/2.511;",
                       nBinsV0M090, BinsV0M090, 100, 0.0, 100.0);

  hZPAvsV0M = new TH2F("hZPAvsV0M", "ZPA;V0M Per; #it{E}_{ZP} [TeV]/2.511;",
                       nBinsV0M090, BinsV0M090, 30, 0.0, 30.0);
  hZPCvsV0M = new TH2F("hZPCvsV0M", "ZPC;V0M Per; #it{E}_{ZP} [TeV]/2.511;",
                       nBinsV0M090, BinsV0M090, 30, 0.0, 30.0);
  hAsyN =
      new TH2F("hAsyN", "Neutron asymmetry;V0M Per; N_{C}-N_{A}/N_{C}+N_{A};",
               nBinsV0M090, BinsV0M090, 50, -1.0, 1.0);
  hAsyP =
      new TH2F("hAsyP", "Proton asymmetry;V0M Per; P_{C}-P_{A}/P_{C}+P_{A};",
               nBinsV0M090, BinsV0M090, 50, -1.0, 1.0);
  hZNvsV0MPer =
      new TH2F("hZNvsV0MPer", "(ZNA+ZNC)/2;V0M Per; #it{E}_{ZN} [TeV]/2.511;",
               nBinsV0M090, BinsV0M090, 100, 0.0, 100.0);
  hZPvsV0MPer =
      new TH2F("hZPvsV0MPer", "(ZPA+ZPC)/2;V0M Per; #it{E}_{ZP} [TeV]/2.511;",
               nBinsV0M090, BinsV0M090, 30, 0.0, 30.0);
  /*hZNvsV0MAmp =*/
  /*    new TH2F("hZNvsV0MAmp", "(ZNA+ZNC)/2;V0M Amp; #it{E}_{ZN}
   * [TeV]/2.511;",*/
  /*             v0mAmp_Nbins, v0mAmp_bins, 100, 0.0, 100.0);*/
  /*hZPvsV0MAmp =*/
  /*    new TH2F("hZPvsV0MAmp", "(ZPA+ZPC)/2;V0M Amp; #it{E}_{ZP}
   * [TeV]/2.511;",*/
  /*             v0mAmp_Nbins, v0mAmp_bins, 30, 0.0, 30.0);*/
  /*pZNvsV0MAmp =*/
  /*    new TProfile("pZNvsV0MAmp", ";V0M Amp;<ZN>;", v0mAmp_Nbins,
   * v0mAmp_bins);*/
  /*pZPvsV0MAmp =*/
  /*    new TProfile("pZPvsV0MAmp", ";V0M Amp;<ZP>;", v0mAmp_Nbins,
   * v0mAmp_bins);*/
  hZNCpmc = new TH1F("hZNCpmc", "ZNC PMC;ZNC energy;Entries", 520, 0., 130.);
  hZNApmc = new TH1F("hZNApmc", "ZNA PMC;ZNA energy;Entries", 520, 0., 130.);
  hZPCpmc = new TH1F("hZPCpmc", "ZPC PMC;ZPC energy;Entries", 120, 0., 30.);
  hZPApmc = new TH1F("hZPApmc", "ZPA PMC;ZPA energy;Entries", 120, 0., 30.);

  fOutputList->Add(hBestVtxZ);
  fOutputList->Add(hV0Percentile);
  /*fOutputList->Add(hV0MAmplitude);*/
  /*fOutputList->Add(pV0MAmpChannel);*/
  /*fOutputList->Add(hZNvsV0MAmp);*/
  fOutputList->Add(hZNvsV0MPer);
  fOutputList->Add(hZNvsV0M);
  fOutputList->Add(hZNAvsV0M);
  fOutputList->Add(hZNCvsV0M);
  fOutputList->Add(hAsyN);
  fOutputList->Add(hZNCpmc);
  fOutputList->Add(hZNApmc);
  /*fOutputList->Add(pZNvsV0MAmp);*/

  /*fOutputList->Add(hZPvsV0MAmp);*/
  fOutputList->Add(hZPvsV0MPer);
  fOutputList->Add(hZPvsV0M);
  fOutputList->Add(hZPAvsV0M);
  fOutputList->Add(hZPCvsV0M);
  fOutputList->Add(hAsyP);
  fOutputList->Add(hZPCpmc);
  fOutputList->Add(hZPApmc);
  /*fOutputList->Add(pZPvsV0MAmp);*/

  fEventCuts.AddQAplotsToList(fOutputList);
  PostData(1, fOutputList);  // postdata will notify the analysis manager of
                             // changes / updates to the
}
//_____________________________________________________________________________
void AliAnalysisTaskZNZP::UserExec(Option_t*) {
  AliVEvent* event = InputEvent();
  if (!event) {
    Error("UserExec", "Could not retrieve event");
    return;
  }

  fESD = dynamic_cast<AliESDEvent*>(event);

  if (!fESD) {
    Printf("%s:%d ESDEvent not found in Input Manager", (char*)__FILE__,
           __LINE__);
    this->Dump();
    return;
  }

  if (fUseMC) {
    //      E S D
    fMC = dynamic_cast<AliMCEvent*>(MCEvent());
    if (!fMC) {
      Printf("%s:%d MCEvent not found in Input Manager", (char*)__FILE__,
             __LINE__);
      this->Dump();
      return;
    }
    fMCStack = fMC->Stack();
  }

  ftrackmult08 = -999.0;
  fv0mpercentile = -999.0;
  fv0mamplitude = -999.0;

  fMultSelection = (AliMultSelection*)fESD->FindListObject("MultSelection");
  fv0mpercentile = fMultSelection->GetMultiplicityPercentile("V0M");
  ftrackmult08 = AliESDtrackCuts::GetReferenceMultiplicity(
      fESD, AliESDtrackCuts::kTrackletsITSTPC, 0.8);

  //! Analyze only the 0--80 % V0M range
  if (!(fv0mpercentile >= fV0Mmin && fv0mpercentile < fV0Mmax)) {
    return;
  }

  //! Trigger selection
  bool isEventTriggered{false};
  UInt_t fSelectMask = fInputHandler->IsEventSelected();
  isEventTriggered = fSelectMask & fTrigger;
  if (!isEventTriggered) {
    return;
  }

  // Good events
  if (!fEventCuts.AcceptEvent(event)) {
    PostData(1, fOutputList);
    return;
  }

  // Good vertex
  bool hasRecVertex{false};
  hasRecVertex = HasRecVertex();
  if (!hasRecVertex) {
    return;
  }

  VertexPosition();

  //! Get calibrated V0 amplitude
  /*GetCalibratedV0Amplitude();*/

  //! Get ZDC Centrality
  hV0Percentile->Fill(fv0mpercentile);
  GetZDC();

  PostData(1, fOutputList);
}
//____________________________________________________________
void AliAnalysisTaskZNZP::Terminate(Option_t*) {}
//____________________________________________________________
void AliAnalysisTaskZNZP::VertexPosition() {
  //! best primary vertex available
  const AliVVertex* vtx = fEventCuts.GetPrimaryVertex();
  hBestVtxZ->Fill(vtx->GetZ());
}
//____________________________________________________________
void AliAnalysisTaskZNZP::GetZDC() {
  AliESDZDC* esdZDC = fESD->GetESDZDC();
  if (!esdZDC) {
    return;
  }

  const double znc{esdZDC->GetZDCN1Energy() / 1000.0};
  const double zna{esdZDC->GetZDCN2Energy() / 1000.0};
  const double zpc{esdZDC->GetZDCP1Energy() / 1000.0};
  const double zpa{esdZDC->GetZDCP2Energy() / 1000.0};

  // Non-average ZN & ZP
  hZNvsV0M->Fill(fv0mpercentile, (znc + zna) / 2.511);
  hZPvsV0M->Fill(fv0mpercentile, (zpc + zpa) / 2.511);

  hZNCvsV0M->Fill(fv0mpercentile, znc / 2.511);
  hZNAvsV0M->Fill(fv0mpercentile, zna / 2.511);

  hZPCvsV0M->Fill(fv0mpercentile, zpc / 2.511);
  hZPAvsV0M->Fill(fv0mpercentile, zpa / 2.511);

  hAsyN->Fill(fv0mpercentile, (znc - zna) / (znc + zna));
  hAsyP->Fill(fv0mpercentile, (zpc - zpa) / (zpc + zpa));

  const double* towZNC{esdZDC->GetZN1TowerEnergy()};
  const double* towZPC{esdZDC->GetZP1TowerEnergy()};
  const double* towZNA{esdZDC->GetZN2TowerEnergy()};
  const double* towZPA{esdZDC->GetZP2TowerEnergy()};
  hZNApmc->Fill(towZNA[0] / 1000.0);
  hZNCpmc->Fill(towZNC[0] / 1000.0);
  hZPApmc->Fill(towZPA[0] / 1000.0);
  hZPCpmc->Fill(towZPC[0] / 1000.0);

  /*const double* towZNCLG{esdZDC->GetZN1TowerEnergyLR()};*/
  /*const double* towZNALG{esdZDC->GetZN2TowerEnergyLR()};*/
  /*cout << "ZNC LG= " << towZNCLG[0] << " | ZNA LG= " << towZNALG[0] << '\n';*/

  // Average the energy detected in each calorimeter
  hZNvsV0MPer->Fill(fv0mpercentile, (znc + zna) / (2.0 * 2.511));
  /*hZNvsV0MAmp->Fill(fv0mamplitude, (znc + zna) / (2.0 * 2.511));*/
  /*pZNvsV0MAmp->Fill(fv0mamplitude, (znc + zna) / 2.0);*/

  hZPvsV0MPer->Fill(fv0mpercentile, (zpc + zpa) / (2.0 * 2.511));
  /*hZPvsV0MAmp->Fill(fv0mamplitude, (zpc + zpa) / (2.0 * 2.511));*/
  /*pZPvsV0MAmp->Fill(fv0mamplitude, (zpc + zpa) / 2.0);*/
}
//____________________________________________________________
void AliAnalysisTaskZNZP::GetCalibratedV0Amplitude() {
  float mV0M{0.0};
  for (int i = 0; i < 64; i++) {
    mV0M += fESD->GetVZEROEqMultiplicity(i);
    pV0MAmpChannel->Fill(i, fESD->GetVZEROEqMultiplicity(i));
  }
  fv0mamplitude = mV0M;

  /*hV0MAmplitude->Fill(fv0mamplitude);*/
}
//____________________________________________________________
void AliAnalysisTaskZNZP::DCAxyDistributions() const {
  int index{-1};
  for (int i = 0; i < v0m_Nbins; ++i) {
    if (fv0mpercentile >= uc_v0m_bins_low[i] &&
        fv0mpercentile < uc_v0m_bins_high[i]) {
      index = i;
      break;
    }
  }

  if (index < 0) {
    return;
  }

  const int n_tracks{fESD->GetNumberOfTracks()};
  for (int i = 0; i < n_tracks; ++i) {
    AliESDtrack* track = static_cast<AliESDtrack*>(fESD->GetTrack(i));
    if (!track) {
      continue;
    }
    if (!fTrackFilterwoDCA->IsSelected(track)) {
      continue;
    }
    if (track->Pt() < fPtMin) {
      continue;
    }
    if (TMath::Abs(track->Eta()) > fEtaCut) {
      continue;
    }
    if (track->Charge() == 0) {
      continue;
    }

    float dcaxy = -999;
    float dcaz = -999;
    track->GetImpactParameters(dcaxy, dcaz);
  }
}

//____________________________________________________________

Bool_t AliAnalysisTaskZNZP::HasRecVertex() {
  float fMaxDeltaSpdTrackAbsolute = 0.5f;
  float fMaxDeltaSpdTrackNsigmaSPD = 1.e14f;
  float fMaxDeltaSpdTrackNsigmaTrack = 1.e14;
  float fMaxResolutionSPDvertex = 0.25f;
  float fMaxDispersionSPDvertex = 1.e14f;

  Bool_t fRequireTrackVertex = true;
  unsigned long fFlag;
  fFlag = BIT(AliEventCuts::kNoCuts);

  const AliVVertex* vtTrc = fESD->GetPrimaryVertex();
  bool isTrackV = true;
  if (vtTrc->IsFromVertexer3D() || vtTrc->IsFromVertexerZ()) isTrackV = false;
  const AliVVertex* vtSPD = fESD->GetPrimaryVertexSPD();

  if (vtSPD->GetNContributors() > 0) fFlag |= BIT(AliEventCuts::kVertexSPD);

  if (vtTrc->GetNContributors() > 1 && isTrackV)
    fFlag |= BIT(AliEventCuts::kVertexTracks);

  if (((fFlag & BIT(AliEventCuts::kVertexTracks)) || !fRequireTrackVertex) &&
      (fFlag & BIT(AliEventCuts::kVertexSPD)))
    fFlag |= BIT(AliEventCuts::kVertex);

  const AliVVertex*& vtx =
      bool(fFlag & BIT(AliEventCuts::kVertexTracks)) ? vtTrc : vtSPD;
  AliVVertex* fPrimaryVertex = const_cast<AliVVertex*>(vtx);
  if (!fPrimaryVertex) return kFALSE;

  /// Vertex quality cuts
  double covTrc[6], covSPD[6];
  vtTrc->GetCovarianceMatrix(covTrc);
  vtSPD->GetCovarianceMatrix(covSPD);
  double dz = bool(fFlag & AliEventCuts::kVertexSPD) &&
                      bool(fFlag & AliEventCuts::kVertexTracks)
                  ? vtTrc->GetZ() - vtSPD->GetZ()
                  : 0.;  /// If one of the two vertices is not available
                         /// this cut is always passed.
  double errTot = TMath::Sqrt(covTrc[5] + covSPD[5]);
  double errTrc =
      bool(fFlag & AliEventCuts::kVertexTracks) ? TMath::Sqrt(covTrc[5]) : 1.;
  double nsigTot = TMath::Abs(dz) / errTot, nsigTrc = TMath::Abs(dz) / errTrc;
  /// vertex dispersion for run1, only for ESD, AOD code to be added here
  const AliESDVertex* vtSPDESD = dynamic_cast<const AliESDVertex*>(vtSPD);
  double vtSPDdispersion = vtSPDESD ? vtSPDESD->GetDispersion() : 0;
  if ((TMath::Abs(dz) <= fMaxDeltaSpdTrackAbsolute &&
       nsigTot <= fMaxDeltaSpdTrackNsigmaSPD &&
       nsigTrc <= fMaxDeltaSpdTrackNsigmaTrack) &&  // discrepancy
                                                    // track-SPD vertex
      (!vtSPD->IsFromVertexerZ() ||
       TMath::Sqrt(covSPD[5]) <= fMaxResolutionSPDvertex) &&
      (!vtSPD->IsFromVertexerZ() ||
       vtSPDdispersion <= fMaxDispersionSPDvertex)  /// vertex dispersion cut
                                                    /// for run1, only for ESD
      )  // quality cut on vertexer SPD z
    fFlag |= BIT(AliEventCuts::kVertexQuality);

  Bool_t hasVtx = (TESTBIT(fFlag, AliEventCuts::kVertex)) &&
                  (TESTBIT(fFlag, AliEventCuts::kVertexQuality));

  return hasVtx;
}

//____________________________________________________________

void AliAnalysisTaskZNZP::ChangeCut(AliESDtrackCuts* fCuts) {
  cout << "Changing track cut (systematic variation): " << fSystematic << '\n';
  switch (fSystematic) {
    case 0:
      fCuts->SetMaxDCAToVertexZ(1);
      break;
    case 1:
      fCuts->SetMaxDCAToVertexZ(5);
      break;
    case 2:
      fCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.7);
      break;
    case 3:
      fCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.9);
      break;
    case 4:
      fCuts->SetMaxFractionSharedTPCClusters(0.2);
      break;
    case 5:
      fCuts->SetMaxFractionSharedTPCClusters(1);
      break;
    case 6:
      fCuts->SetMaxChi2PerClusterTPC(3);
      break;
    case 7:
      fCuts->SetMaxChi2PerClusterTPC(5);
      break;
    case 8:
      fCuts->SetMaxChi2PerClusterITS(25);
      break;
    case 9:
      fCuts->SetMaxChi2PerClusterITS(49);
      break;
    case 10:
      fCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,
                                      AliESDtrackCuts::kNone);
      break;
    case 11:
      fCuts->SetCutGeoNcrNcl(2., 130., 1.5, 0.85, 0.7);
      break;
    case 12:
      fCuts->SetCutGeoNcrNcl(4., 130., 1.5, 0.85, 0.7);
      break;
    case 13:
      fCuts->SetCutGeoNcrNcl(3., 120., 1.5, 0.85, 0.7);
      break;
    case 14:
      fCuts->SetCutGeoNcrNcl(3., 140., 1.5, 0.85, 0.7);
      break;
    case 15:
      fCuts->SetMaxChi2TPCConstrainedGlobal(25);
      break;
    case 16:
      fCuts->SetMaxChi2TPCConstrainedGlobal(49);
      break;
    default:
      cout << "fSystematic not defined!" << '\n';
  }
}
