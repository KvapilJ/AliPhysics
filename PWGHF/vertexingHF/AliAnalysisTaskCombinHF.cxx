/**************************************************************************
 * Copyright(c) 1998-2018, ALICE Experiment at CERN, All rights reserved. *
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
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: $ */

//*************************************************************************
// Class AliAnalysisTaskCombinHF
// AliAnalysisTaskSE to build D meson candidates by combining tracks
//  background is computed LS and track rotations is
// Authors: F. Prino, A. Rossi
/////////////////////////////////////////////////////////////

#include <TList.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <THnSparse.h>

#include "AliAnalysisManager.h"
#include "AliInputEventHandler.h"
#include "AliPIDResponse.h"
#include "AliAODHandler.h"
#include "AliAODEvent.h"
#include "AliAODMCParticle.h"
#include "AliAODMCHeader.h"
#include "AliAODVertex.h"
#include "AliAODTrack.h"
#include "AliVertexingHFUtils.h"
#include "AliAnalysisTaskCombinHF.h"

ClassImp(AliAnalysisTaskCombinHF)


//________________________________________________________________________
AliAnalysisTaskCombinHF::AliAnalysisTaskCombinHF():
  AliAnalysisTaskSE(),
  fOutput(0x0),
  fHistNEvents(0x0),
  fHistTrackStatus(0x0),
  fHistCheckOrigin(0x0),
  fHistCheckOriginSel(0x0),
  fHistCheckDecChan(0x0),
  fHistCheckDecChanAcc(0x0),
  fPtVsYGen(0x0),
  fPtVsYGenLargeAcc(0x0),
  fPtVsYGenLimAcc(0x0),
  fPtVsYGenAcc(0x0),
  fPtVsYReco(0x0),
  fMassVsPtVsY(0x0),
  fMassVsPtVsYRot(0x0),
  fMassVsPtVsYLSpp(0x0),
  fMassVsPtVsYLSmm(0x0),
  fMassVsPtVsYSig(0x0),
  fMassVsPtVsYRefl(0x0),
  fMassVsPtVsYBkg(0x0),
  fNSelected(0x0),
  fNormRotated(0x0),
  fDeltaMass(0x0),
  fDeltaMassFullAnalysis(0x0),
  fMassVsPtVsYME(0x0),
  fFilterMask(BIT(4)),
  fTrackCutsAll(0x0),
  fTrackCutsPion(0x0),
  fTrackCutsKaon(0x0),
  fPidHF(new AliAODPidHF()),
  fAnalysisCuts(0x0),
  fMinMass(1.720),
  fMaxMass(2.150),
  fMaxPt(10.),
  fPtBinWidth(0.5),
  fEtaAccCut(0.9),
  fPtAccCut(0.1),
  fNRotations(9),
  fMinAngleForRot(5*TMath::Pi()/6),
  fMaxAngleForRot(7*TMath::Pi()/6),
  fMinAngleForRot3(2*TMath::Pi()/6),
  fMaxAngleForRot3(4*TMath::Pi()/6),
  fCounter(0x0),
  fMeson(kDzero),
  fReadMC(kFALSE),
  fPromptFeeddown(kPrompt),
  fGoUpToQuark(kTRUE),
  fFullAnalysis(0),
  fPIDstrategy(knSigma),
  fmaxPforIDPion(0.8),
  fmaxPforIDKaon(2.),
  fKeepNegID(kFALSE),
  fPIDselCaseZero(0),
  fBayesThresKaon(0.4),
  fBayesThresPion(0.4),
  fDoEventMixing(kTRUE),
  fMaxNumberOfEventsForMixing(20),
  fMaxzVertDistForMix(20.),
  fMaxMultDiffForMix(9999.),
  fUsePoolsZ(kFALSE),
  fUsePoolsM(kFALSE),
  fNzVertPools(1),
  fNzVertPoolsLimSize(2),
  fzVertPoolLims(0x0),
  fNMultPools(1),
  fNMultPoolsLimSize(2),
  fMultPoolLims(0x0),
  fEventBuffer(0x0),
  fVtxZ(0),
  fMultiplicity(0),
  fKaonTracks(0x0),
  fPionTracks(0x0)
{
  // default constructor
}

//________________________________________________________________________
AliAnalysisTaskCombinHF::AliAnalysisTaskCombinHF(Int_t meson, AliRDHFCuts* analysiscuts):
  AliAnalysisTaskSE("DmesonCombin"),
  fOutput(0x0),
  fHistNEvents(0x0),
  fHistTrackStatus(0x0),
  fHistCheckOrigin(0x0),
  fHistCheckOriginSel(0x0),
  fHistCheckDecChan(0x0),
  fHistCheckDecChanAcc(0x0),
  fPtVsYGen(0x0),
  fPtVsYGenLargeAcc(0x0),
  fPtVsYGenLimAcc(0x0),
  fPtVsYGenAcc(0x0),
  fPtVsYReco(0x0),
  fMassVsPtVsY(0x0),
  fMassVsPtVsYRot(0x0),
  fMassVsPtVsYLSpp(0x0),
  fMassVsPtVsYLSmm(0x0),
  fMassVsPtVsYSig(0x0),
  fMassVsPtVsYRefl(0x0),
  fMassVsPtVsYBkg(0x0),
  fNSelected(0x0),
  fNormRotated(0x0),
  fDeltaMass(0x0),
  fDeltaMassFullAnalysis(0x0),
  fMassVsPtVsYME(0x0),
  fFilterMask(BIT(4)),
  fTrackCutsAll(0x0),
  fTrackCutsPion(0x0),
  fTrackCutsKaon(0x0),
  fPidHF(new AliAODPidHF()),
  fAnalysisCuts(analysiscuts),
  fMinMass(1.720),
  fMaxMass(2.150),
  fMaxPt(10.),
  fPtBinWidth(0.5),
  fEtaAccCut(0.9),
  fPtAccCut(0.1),
  fNRotations(9),
  fMinAngleForRot(5*TMath::Pi()/6),
  fMaxAngleForRot(7*TMath::Pi()/6),
  fMinAngleForRot3(2*TMath::Pi()/6),
  fMaxAngleForRot3(4*TMath::Pi()/6),
  fCounter(0x0),
  fMeson(meson),
  fReadMC(kFALSE),
  fPromptFeeddown(1),
  fGoUpToQuark(kTRUE),
  fFullAnalysis(0),
  fPIDstrategy(knSigma),
  fmaxPforIDPion(0.8),
  fmaxPforIDKaon(2.),
  fKeepNegID(kFALSE),
  fPIDselCaseZero(0),
  fBayesThresKaon(0.4),
  fBayesThresPion(0.4),
  fDoEventMixing(kTRUE),
  fMaxNumberOfEventsForMixing(20),
  fMaxzVertDistForMix(20.),
  fMaxMultDiffForMix(9999.),
  fUsePoolsZ(kFALSE),
  fUsePoolsM(kFALSE),
  fNzVertPools(1),
  fNzVertPoolsLimSize(2),
  fzVertPoolLims(0x0),
  fNMultPools(1),
  fNMultPoolsLimSize(2),
  fMultPoolLims(0x0),
  fEventBuffer(0x0),
  fVtxZ(0),
  fMultiplicity(0),
  fKaonTracks(0x0),
  fPionTracks(0x0)
{
  // standard constructor

  DefineOutput(1,TList::Class());  //My private output
  DefineOutput(2,AliNormalizationCounter::Class());
}

//________________________________________________________________________
AliAnalysisTaskCombinHF::~AliAnalysisTaskCombinHF()
{
  //
  // Destructor
  //
  if(!fOutput->IsOwner()){
    delete fHistNEvents;
    delete fHistTrackStatus;
    delete fHistCheckOrigin;
    delete fHistCheckOriginSel;
    delete fHistCheckDecChan;
    delete fHistCheckDecChanAcc;
    delete fPtVsYGen;
    delete fPtVsYGenLargeAcc;
    delete fPtVsYGenLimAcc;
    delete fPtVsYGenAcc;
    delete fPtVsYReco;
    delete fMassVsPtVsY;
    delete fMassVsPtVsYLSpp;
    delete fMassVsPtVsYLSmm;
    delete fMassVsPtVsYRot;
    delete fMassVsPtVsYSig;
    delete fMassVsPtVsYRefl;
    delete fMassVsPtVsYBkg;
    delete fNSelected;
    delete fNormRotated;
    delete fDeltaMass;
    delete fDeltaMassFullAnalysis;
    delete fMassVsPtVsYME;
  }

  delete fOutput;
  delete fCounter;
  delete fTrackCutsAll;
  delete fTrackCutsPion;
  delete fTrackCutsKaon;
  delete fPidHF;
  delete fAnalysisCuts;
  fKaonTracks->Delete();
  fPionTracks->Delete();
  delete fKaonTracks;
  delete fPionTracks;
  delete fEventBuffer;
  delete [] fzVertPoolLims;
  delete [] fMultPoolLims;
}

//________________________________________________________________________
void AliAnalysisTaskCombinHF::ConfigureZVertPools(Int_t nPools, Double_t*  zVertLimits)
{
  // sets the pools for event mizing in zvertex
  if(fzVertPoolLims) delete [] fzVertPoolLims;
  fNzVertPools=nPools;
  fNzVertPoolsLimSize=nPools+1;
  fzVertPoolLims = new Double_t[fNzVertPoolsLimSize];
  for(Int_t ib=0; ib<fNzVertPoolsLimSize; ib++) fzVertPoolLims[ib]=zVertLimits[ib];
  fUsePoolsZ=kTRUE;
  return;  
}
//________________________________________________________________________
void AliAnalysisTaskCombinHF::ConfigureMultiplicityPools(Int_t nPools, Double_t*  multLimits)
{
  // sets the pools for event mizing in zvertex
  if(fMultPoolLims) delete [] fMultPoolLims;
  fNMultPools=nPools;
  fNMultPoolsLimSize=nPools+1;
  fMultPoolLims = new Double_t[fNMultPoolsLimSize];
  for(Int_t ib=0; ib<nPools+1; ib++) fMultPoolLims[ib]=multLimits[ib];
  fUsePoolsM=kTRUE;
  return;  
}
//________________________________________________________________________
void AliAnalysisTaskCombinHF::UserCreateOutputObjects()
{
  // Create the output container
  //
  if(fDebug > 1) printf("AnalysisTaskCombinHF::UserCreateOutputObjects() \n");
  
  fOutput = new TList();
  fOutput->SetOwner();
  fOutput->SetName("OutputHistos");
  
  fHistNEvents = new TH1F("hNEvents", "number of events ",8,-0.5,7.5);
  fHistNEvents->GetXaxis()->SetBinLabel(1,"nEventsAnal");
  fHistNEvents->GetXaxis()->SetBinLabel(2,"n. passing IsEvSelected");
  fHistNEvents->GetXaxis()->SetBinLabel(3,"n. rejected due to trigger");
  fHistNEvents->GetXaxis()->SetBinLabel(4,"n. rejected due to not reco vertex");
  fHistNEvents->GetXaxis()->SetBinLabel(5,"n. rejected for contr vertex");
  fHistNEvents->GetXaxis()->SetBinLabel(6,"n. rejected for vertex out of accept");
  fHistNEvents->GetXaxis()->SetBinLabel(7,"n. rejected for pileup events");
  fHistNEvents->GetXaxis()->SetBinLabel(8,"no. of out centrality events");
  
  fHistNEvents->GetXaxis()->SetNdivisions(1,kFALSE);
  fHistNEvents->Sumw2();
  fHistNEvents->SetMinimum(0);
  fOutput->Add(fHistNEvents);
  
  fHistTrackStatus  = new TH1F("hTrackStatus", "",8,-0.5,7.5);
  fHistTrackStatus->GetXaxis()->SetBinLabel(1,"Not OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(2,"Track OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(3,"Kaon, Not OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(4,"Kaon OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(5,"Pion, Not OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(6,"Pion OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(7,"Kaon||Pion, Not OK");
  fHistTrackStatus->GetXaxis()->SetBinLabel(8,"Kaon||Pion OK");
  fHistTrackStatus->GetXaxis()->SetNdivisions(1,kFALSE);
  fHistTrackStatus->Sumw2();
  fHistTrackStatus->SetMinimum(0);
  fOutput->Add(fHistTrackStatus);
  
  Int_t nPtBins = (Int_t)(fMaxPt/fPtBinWidth+0.001);
  Double_t maxPt=fPtBinWidth*nPtBins;

  if(fReadMC){
    
    fHistCheckOrigin=new TH1F("hCheckOrigin","",7,-1.5,5.5);
    fHistCheckOrigin->Sumw2();
    fHistCheckOrigin->SetMinimum(0);
    fOutput->Add(fHistCheckOrigin);
    
    fHistCheckOriginSel=new TH1F("hCheckOriginSel","",7,-1.5,5.5);
    fHistCheckOriginSel->Sumw2();
    fHistCheckOriginSel->SetMinimum(0);
    fOutput->Add(fHistCheckOriginSel);
    
    fHistCheckDecChan=new TH1F("hCheckDecChan","",7,-2.5,4.5);
    fHistCheckDecChan->Sumw2();
    fHistCheckDecChan->SetMinimum(0);
    fOutput->Add(fHistCheckDecChan);
    
    fHistCheckDecChanAcc=new TH1F("hCheckDecChanAcc","",7,-2.5,4.5);
    fHistCheckDecChanAcc->Sumw2();
    fHistCheckDecChanAcc->SetMinimum(0);
    fOutput->Add(fHistCheckDecChanAcc);
    
    fPtVsYGen= new TH2F("hPtVsYGen","",nPtBins,0.,maxPt,20,-1.,1.);
    fPtVsYGen->Sumw2();
    fPtVsYGen->SetMinimum(0);
    fOutput->Add(fPtVsYGen);
    
    fPtVsYGenLargeAcc= new TH2F("hPtVsYGenLargeAcc","",nPtBins,0.,maxPt,20,-1.,1.);
    fPtVsYGenLargeAcc->Sumw2();
    fPtVsYGenLargeAcc->SetMinimum(0);
    fOutput->Add(fPtVsYGenLargeAcc);
    
    fPtVsYGenLimAcc= new TH2F("hPtVsYGenLimAcc","",nPtBins,0.,maxPt,20,-1.,1.);
    fPtVsYGenLimAcc->Sumw2();
    fPtVsYGenLimAcc->SetMinimum(0);
    fOutput->Add(fPtVsYGenLimAcc);
    
    fPtVsYGenAcc= new TH2F("hPtVsYGenAcc","",nPtBins,0.,maxPt,20,-1.,1.);
    fPtVsYGenAcc->Sumw2();
    fPtVsYGenAcc->SetMinimum(0);
    fOutput->Add(fPtVsYGenAcc);
    
    fPtVsYReco= new TH2F("hPtVsYReco","",nPtBins,0.,maxPt,20,-1.,1.);
    fPtVsYReco->Sumw2();
    fPtVsYReco->SetMinimum(0);
    fOutput->Add(fPtVsYReco);
  }
  
  
  Int_t nMassBins=fMaxMass*1000.-fMinMass*1000.;
  Double_t maxm=fMinMass+nMassBins*0.001;
  fMassVsPtVsY=new TH3F("hMassVsPtVsY","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
  fMassVsPtVsY->Sumw2();
  fMassVsPtVsY->SetMinimum(0);
  fOutput->Add(fMassVsPtVsY);
  
  fMassVsPtVsYRot=new TH3F("hMassVsPtVsYRot","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
  fMassVsPtVsYRot->Sumw2();
  fMassVsPtVsYRot->SetMinimum(0);
  fOutput->Add(fMassVsPtVsYRot);
  
  if(fMeson==kDzero){
    fMassVsPtVsYLSpp=new TH3F("hMassVsPtVsYLSpp","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
    fMassVsPtVsYLSpp->Sumw2();
    fMassVsPtVsYLSpp->SetMinimum(0);
    fOutput->Add(fMassVsPtVsYLSpp);
    fMassVsPtVsYLSmm=new TH3F("hMassVsPtVsYLSmm","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
    fMassVsPtVsYLSmm->Sumw2();
    fMassVsPtVsYLSmm->SetMinimum(0);
    fOutput->Add(fMassVsPtVsYLSmm);
  }
  
  fMassVsPtVsYSig=new TH3F("hMassVsPtVsYSig","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
  fMassVsPtVsYSig->Sumw2();
  fMassVsPtVsYSig->SetMinimum(0);
  fOutput->Add(fMassVsPtVsYSig);
  
  fMassVsPtVsYRefl=new TH3F("hMassVsPtVsYRefl","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
  fMassVsPtVsYRefl->Sumw2();
  fMassVsPtVsYRefl->SetMinimum(0);
  fOutput->Add(fMassVsPtVsYRefl);
  
  fMassVsPtVsYBkg=new TH3F("hMassVsPtVsYBkg","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
  fMassVsPtVsYBkg->Sumw2();
  fMassVsPtVsYBkg->SetMinimum(0);
  fOutput->Add(fMassVsPtVsYBkg);
  
  fNSelected=new TH1F("hNSelected","",100,-0.5,99.5);
  fNSelected->Sumw2();
  fNSelected->SetMinimum(0);
  fOutput->Add(fNSelected);
  
  fNormRotated=new TH1F("hNormRotated","",11,-0.5,10.5);
  fNormRotated->Sumw2();
  fNormRotated->SetMinimum(0);
  fOutput->Add(fNormRotated);
  
  fDeltaMass=new TH1F("hDeltaMass","",100,-0.4,0.4);
  fDeltaMass->Sumw2();
  fDeltaMass->SetMinimum(0);
  fOutput->Add(fDeltaMass);
  
  Int_t binSparseDMassRot[5]={nMassBins,100,24,40,20};
  Double_t edgeLowSparseDMassRot[5]={fMinMass,-0.4,0.,-4.,0};
  Double_t edgeHighSparseDMassRot[5]={maxm,0.4,12.,4.,3.14};
  fDeltaMassFullAnalysis=new THnSparseF("fDeltaMassFullAnalysis","fDeltaMassFullAnalysis;inv mass (GeV/c);#Delta inv mass (GeV/c) ; p_{T}^{D} (GeV/c); #Delta p_{T} (GeV/c); daughter angle (2prongs) (rad);",5,binSparseDMassRot,edgeLowSparseDMassRot,edgeHighSparseDMassRot);
  fOutput->Add(fDeltaMassFullAnalysis);
  
  fMassVsPtVsYME=new TH3F("hMassVsPtVsYME","",nMassBins,fMinMass,maxm,nPtBins,0.,maxPt,20,-1.,1.);
  fMassVsPtVsYME->Sumw2();
  fMassVsPtVsYME->SetMinimum(0);
  fOutput->Add(fMassVsPtVsYME);
 

  //Counter for Normalization
  fCounter = new AliNormalizationCounter("NormalizationCounter");
  fCounter->Init();
  
  fKaonTracks = new TObjArray();
  fPionTracks=new TObjArray();
  fKaonTracks->SetOwner();
  fPionTracks->SetOwner();
  fEventBuffer = new TTree("EventBuffer", "Temporary buffer for event mixing");
  fEventBuffer->Branch("zVertex", &fVtxZ);
  fEventBuffer->Branch("multiplicity", &fMultiplicity);
  fEventBuffer->Branch("karray", "TObjArray", &fKaonTracks);
  fEventBuffer->Branch("parray", "TObjArray", &fPionTracks);

  PostData(1,fOutput);
  PostData(2,fCounter);
}

//________________________________________________________________________
void AliAnalysisTaskCombinHF::UserExec(Option_t */*option*/){
  //Build the 3-track combinatorics (+-+ and -+-) for D+->Kpipi decays
  
  AliAODEvent *aod = dynamic_cast<AliAODEvent*> (InputEvent());
  if(!aod && AODEvent() && IsStandardAOD()) {
    // In case there is an AOD handler writing a standard AOD, use the AOD
    // event in memory rather than the input (ESD) event.
    aod = dynamic_cast<AliAODEvent*> (AODEvent());
  }
  if(!aod){
    printf("AliAnalysisTaskCombinHF::UserExec: AOD not found!\n");
    return;
  }
  
  // fix for temporary bug in ESDfilter
  // the AODs with null vertex pointer didn't pass the PhysSel
  if(!aod->GetPrimaryVertex() || TMath::Abs(aod->GetMagneticField())<0.001) return;
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler *inputHandler=(AliInputEventHandler*)mgr->GetInputEventHandler();
  AliPIDResponse *pidResp=inputHandler->GetPIDResponse();
  fPidHF->SetPidResponse(pidResp);
  
  
  fHistNEvents->Fill(0); // count event
  // Post the data already here
  PostData(1,fOutput);
  
  fCounter->StoreEvent(aod,fAnalysisCuts,fReadMC);
  
  Bool_t isEvSel=fAnalysisCuts->IsEventSelected(aod);
  if(fAnalysisCuts->IsEventRejectedDueToTrigger())fHistNEvents->Fill(2);
  if(fAnalysisCuts->IsEventRejectedDueToNotRecoVertex())fHistNEvents->Fill(3);
  if(fAnalysisCuts->IsEventRejectedDueToVertexContributors())fHistNEvents->Fill(4);
  if(fAnalysisCuts->IsEventRejectedDueToZVertexOutsideFiducialRegion())fHistNEvents->Fill(5);
  //  if(fAnalysisCuts->IsEventRejectedDueToPileup())fHistNEvents->Fill(6);
  if(fAnalysisCuts->IsEventRejectedDueToCentrality()) fHistNEvents->Fill(7);
  
  if(!isEvSel)return;
  
  fHistNEvents->Fill(1);
  
  TClonesArray *arrayMC=0;
  AliAODMCHeader *mcHeader=0;
  if(fReadMC){
    arrayMC =  (TClonesArray*)aod->GetList()->FindObject(AliAODMCParticle::StdBranchName());
    if(!arrayMC) {
      printf("AliAnalysisTaskCombinHF::UserExec: MC particles branch not found!\n");
      return;
    }
    
    // load MC header
    mcHeader =  (AliAODMCHeader*)aod->GetList()->FindObject(AliAODMCHeader::StdBranchName());
    if(!mcHeader) {
      printf("AliAnalysisTaskCombinHF::UserExec: MC header branch not found!\n");
      return;
    }
    FillGenHistos(arrayMC);
  }
  
  
  Int_t ntracks=aod->GetNTracks();
  fVtxZ = aod->GetPrimaryVertex()->GetZ();
  fMultiplicity = AliVertexingHFUtils::GetNumberOfTrackletsInEtaRange(aod,-1.,1.); 

  // select and flag tracks
  UChar_t* status = new UChar_t[ntracks];
  for(Int_t iTr=0; iTr<ntracks; iTr++){
    status[iTr]=0;
    AliAODTrack* track=aod->GetTrack(iTr);
    if(IsTrackSelected(track)) status[iTr]+=1;
    
    // PID
    if (fPIDstrategy == knSigma) {
      // nsigma PID
      if(IsKaon(track)) status[iTr]+=2;
      if(IsPion(track)) status[iTr]+=4;
    }
    else if (fPIDstrategy == kBayesianMaxProb || fPIDstrategy == kBayesianThres) {
      // Bayesian PID
      Double_t *weights = new Double_t[AliPID::kSPECIES];
      fPidHF->GetPidCombined()->ComputeProbabilities(track, fPidHF->GetPidResponse(), weights);
      if (fPIDstrategy == kBayesianMaxProb) {
        if (TMath::MaxElement(AliPID::kSPECIES, weights) == weights[AliPID::kKaon]) status[iTr] += 2;
        if (TMath::MaxElement(AliPID::kSPECIES, weights) == weights[AliPID::kPion]) status[iTr] += 4;
      }
      if (fPIDstrategy == kBayesianThres) {
        if (weights[AliPID::kKaon] > fBayesThresKaon) status[iTr] += 2;
        if (weights[AliPID::kPion] > fBayesThresPion) status[iTr] += 4;
      }
      delete[] weights;
    }
    
    fHistTrackStatus->Fill(status[iTr]);
  }
  
  // build the combinatorics
  Int_t nSelected=0;
  Int_t nFiltered=0;
  Double_t dummypos[3]={0.,0.,0.};
  AliAODVertex* v2=new AliAODVertex(dummypos,999.,-1,2);
  AliAODVertex* v3=new AliAODVertex(dummypos,999.,-1,3);
  // dummy values of track impact parameter, needed to build an AliAODRecoDecay object
  Double_t d02[2]={0.,0.};
  Double_t d03[3]={0.,0.,0.};
  AliAODRecoDecay* tmpRD2 = new AliAODRecoDecay(0x0,2,0,d02);
  AliAODRecoDecay* tmpRD3 = new AliAODRecoDecay(0x0,3,1,d03);
  UInt_t pdg0[2]={321,211};
  UInt_t pdgp[3]={321,211,211};
  //  UInt_t pdgs[3]={321,321,211};
  Double_t tmpp[3];
  Double_t px[3],py[3],pz[3];
  Int_t dgLabels[3];
  fKaonTracks->Delete();
  fPionTracks->Delete();
 
  for(Int_t iTr1=0; iTr1<ntracks; iTr1++){
    AliAODTrack* trK=aod->GetTrack(iTr1);
    if((status[iTr1] & 1)==0) continue;
    if(fDoEventMixing){
      if(status[iTr1] & 2) fKaonTracks->AddLast(new TLorentzVector(trK->Px(),trK->Py(),trK->Pz(),trK->Charge()));
      if(status[iTr1] & 4) fPionTracks->AddLast(new TLorentzVector(trK->Px(),trK->Py(),trK->Pz(),trK->Charge()));
    }
    if((status[iTr1] & 2)==0) continue;
    Int_t chargeK=trK->Charge();
    trK->GetPxPyPz(tmpp);
    px[0] = tmpp[0];
    py[0] = tmpp[1];
    pz[0] = tmpp[2];
    dgLabels[0]=trK->GetLabel();
    for(Int_t iTr2=0; iTr2<ntracks; iTr2++){
      if((status[iTr2] & 1)==0) continue;
      if((status[iTr2] & 4)==0) continue;
      if(iTr1==iTr2) continue;
      AliAODTrack* trPi1=aod->GetTrack(iTr2);
      Int_t chargePi1=trPi1->Charge();
      trPi1->GetPxPyPz(tmpp);
      px[1] = tmpp[0];
      py[1] = tmpp[1];
      pz[1] = tmpp[2];
      dgLabels[1]=trPi1->GetLabel();
      if(chargePi1==chargeK){
        if(fMeson==kDzero) FillLSHistos(421,2,tmpRD2,px,py,pz,pdg0,chargePi1);
        continue;
      }
      if(fMeson==kDzero){
        nFiltered++;
        v2->AddDaughter(trK);
        v2->AddDaughter(trPi1);
        tmpRD2->SetSecondaryVtx(v2);
        Bool_t ok=FillHistos(421,2,tmpRD2,px,py,pz,pdg0,arrayMC,dgLabels);
        v2->RemoveDaughters();
        if(ok) nSelected++;
      }else{
        for(Int_t iTr3=iTr2+1; iTr3<ntracks; iTr3++){
          if((status[iTr3] & 1)==0) continue;
          if((status[iTr3] & 4)==0) continue;
          if(iTr1==iTr3) continue;
          AliAODTrack* trPi2=aod->GetTrack(iTr3);
          Int_t chargePi2=trPi2->Charge();
          if(chargePi2==chargeK) continue;
          nFiltered++;
          trPi2->GetPxPyPz(tmpp);
          px[2] = tmpp[0];
          py[2] = tmpp[1];
          pz[2] = tmpp[2];
          dgLabels[2]=trPi2->GetLabel();
          v3->AddDaughter(trK);
          v3->AddDaughter(trPi1);
          v3->AddDaughter(trPi2);
          tmpRD3->SetSecondaryVtx(v3);
          Bool_t ok=FillHistos(411,3,tmpRD3,px,py,pz,pdgp,arrayMC,dgLabels);
          v3->RemoveDaughters();
          if(ok) nSelected++;
        }
      }
    }
  }
  
  delete [] status;
  delete v2;
  delete v3;
  delete tmpRD2;
  delete tmpRD3;
  
  fNSelected->Fill(nSelected);
  
  fCounter->StoreCandidates(aod,nFiltered,kTRUE);
  fCounter->StoreCandidates(aod,nSelected,kFALSE);

  if(fDoEventMixing) fEventBuffer->Fill();
  PostData(1,fOutput);
  PostData(2,fCounter);
  
  return;
}

//________________________________________________________________________
void AliAnalysisTaskCombinHF::FillLSHistos(Int_t pdgD,Int_t nProngs, AliAODRecoDecay* tmpRD, Double_t* px, Double_t* py, Double_t* pz, UInt_t *pdgdau, Int_t charge){
  // Fill histos for LS candidates
  
  tmpRD->SetPxPyPzProngs(nProngs,px,py,pz);
  Double_t pt = tmpRD->Pt();
  Double_t minv2 = tmpRD->InvMass2(nProngs,pdgdau);
  if(minv2>fMinMass*fMinMass && minv2<fMaxMass*fMaxMass){
    Double_t rapid = tmpRD->Y(pdgD);
    if(fAnalysisCuts->IsInFiducialAcceptance(pt,rapid)){
      if(charge>0) fMassVsPtVsYLSpp->Fill(TMath::Sqrt(minv2),pt,rapid);
      else fMassVsPtVsYLSmm->Fill(TMath::Sqrt(minv2),pt,rapid);
    }
  }
  return;
}

//________________________________________________________________________
void AliAnalysisTaskCombinHF::FillGenHistos(TClonesArray* arrayMC){
  // Fill histos with generated quantities
  Int_t totPart=arrayMC->GetEntriesFast();
  Int_t thePDG=411;
  Int_t nProng=3;
  if(fMeson==kDzero){
    thePDG=421;
    nProng=2;
  }
  for(Int_t ip=0; ip<totPart; ip++){
    AliAODMCParticle *part = (AliAODMCParticle*)arrayMC->At(ip);
    if(TMath::Abs(part->GetPdgCode())==thePDG){
      Int_t orig=AliVertexingHFUtils::CheckOrigin(arrayMC,part,fGoUpToQuark);
      fHistCheckOrigin->Fill(orig);
      if(fPromptFeeddown==kFeeddown && orig!=5) continue;
      else if(fPromptFeeddown==kPrompt && orig!=4) continue;
      else if(fPromptFeeddown==kBoth && orig<4) continue;
      fHistCheckOriginSel->Fill(orig);
      Int_t deca=0;
      Bool_t isGoodDecay=kFALSE;
      Int_t labDau[4]={-1,-1,-1,-1};
      if(fMeson==kDzero){
        deca=AliVertexingHFUtils::CheckD0Decay(arrayMC,part,labDau);
        if(part->GetNDaughters()!=2) continue;
        if(deca==1) isGoodDecay=kTRUE;
      }else if(fMeson==kDplus){
        deca=AliVertexingHFUtils::CheckDplusDecay(arrayMC,part,labDau);
        if(deca>0) isGoodDecay=kTRUE;
      }
      fHistCheckDecChan->Fill(deca);
      if(labDau[0]==-1){
        //	printf(Form("Meson %d Label of daughters not filled correctly -- %d\n",fMeson,isGoodDecay));
        continue; //protection against unfilled array of labels
      }
      Bool_t isInAcc=CheckAcceptance(arrayMC,nProng,labDau);
      if(isInAcc) fHistCheckDecChanAcc->Fill(deca);
      if(isGoodDecay){
        Double_t ptgen=part->Pt();
        Double_t ygen=part->Y();
	if(fAnalysisCuts->IsInFiducialAcceptance(ptgen,ygen)){
	  fPtVsYGen->Fill(ptgen,ygen);
	  if(TMath::Abs(ygen)<0.5) fPtVsYGenLimAcc->Fill(ptgen,ygen);
	  if(isInAcc) fPtVsYGenAcc->Fill(ptgen,ygen);
	}
        if(TMath::Abs(ygen)<0.9) fPtVsYGenLargeAcc->Fill(ptgen,ygen);
      }
    }
  }
}

//________________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::FillHistos(Int_t pdgD,Int_t nProngs, AliAODRecoDecay* tmpRD, Double_t* px, Double_t* py, Double_t* pz, UInt_t *pdgdau, TClonesArray *arrayMC, Int_t* dgLabels){
  // Fill histos for candidates with proper charge sign
  
  Bool_t accept=kFALSE;
  
  tmpRD->SetPxPyPzProngs(nProngs,px,py,pz);
  Double_t pt = tmpRD->Pt();
  Double_t minv2 = tmpRD->InvMass2(nProngs,pdgdau);
  Double_t mass=TMath::Sqrt(minv2);
  
  if(minv2>fMinMass*fMinMass && minv2<fMaxMass*fMaxMass){
    Double_t rapid = tmpRD->Y(pdgD);
    if(fAnalysisCuts->IsInFiducialAcceptance(pt,rapid)){
      fMassVsPtVsY->Fill(mass,pt,rapid);
      accept=kTRUE;
      if(fReadMC){
        Int_t signPdg[3]={0,0,0};
        for(Int_t iii=0; iii<nProngs; iii++) signPdg[iii]=pdgdau[iii];
        Int_t labD = tmpRD->MatchToMC(pdgD,arrayMC,nProngs,signPdg);
        if(labD>=0){
          AliAODMCParticle* part = dynamic_cast<AliAODMCParticle*>(arrayMC->At(TMath::Abs(dgLabels[0])));
          if(part){
            Int_t orig=AliVertexingHFUtils::CheckOrigin(arrayMC,part,fGoUpToQuark);
            if((fPromptFeeddown==kFeeddown && orig==5)|| (fPromptFeeddown==kPrompt && orig==4) || (fPromptFeeddown==kBoth && orig>=4)) {
              
              Int_t pdgCode = TMath::Abs( part->GetPdgCode() );
              if(pdgCode==321){
                fMassVsPtVsYSig->Fill(mass,pt,rapid);
                AliAODMCParticle* dmes =  dynamic_cast<AliAODMCParticle*>(arrayMC->At(labD));
                fPtVsYReco->Fill(dmes->Pt(),dmes->Y());
              }else{
                fMassVsPtVsYRefl->Fill(mass,pt,rapid);
              }
            }
          }
        }else{
          fMassVsPtVsYBkg->Fill(mass,pt,rapid);
        }
      }
    }
  }
  
  Int_t nRotated=0;
  Double_t massRot=0;// calculated later only if candidate is acceptable
  Double_t angleProngXY;
  if(TMath::Abs(pdgD)==421)angleProngXY=TMath::ACos((px[0]*px[1]+py[0]*py[1])/TMath::Sqrt((px[0]*px[0]+py[0]*py[0])*(px[1]*px[1]+py[1]*py[1])));
  else {
    angleProngXY=TMath::ACos(((px[0]+px[1])*px[2]+(py[0]+py[1])*py[2])/TMath::Sqrt(((px[0]+px[1])*(px[0]+px[1])+(py[0]+py[1])*(py[0]+py[1]))*(px[2]*px[2]+py[2]*py[2])));
  }
  Double_t ptOrig=pt;
  
  
  Double_t rotStep=(fMaxAngleForRot-fMinAngleForRot)/(fNRotations-1); // -1 is to ensure that the last rotation is done with angle=fMaxAngleForRot
  Double_t rotStep3=(fMaxAngleForRot3-fMinAngleForRot3)/(fNRotations-1); // -1 is to ensure that the last rotation is done with angle=fMaxAngleForRot
  
  for(Int_t irot=0; irot<fNRotations; irot++){
    Double_t phirot=fMinAngleForRot+rotStep*irot;
    Double_t tmpx=px[0];
    Double_t tmpy=py[0];
    Double_t tmpx2=px[2];
    Double_t tmpy2=py[2];
    px[0]=tmpx*TMath::Cos(phirot)-tmpy*TMath::Sin(phirot);
    py[0]=tmpx*TMath::Sin(phirot)+tmpy*TMath::Cos(phirot);
    if(pdgD==411 || pdgD==431){
      Double_t phirot2=fMinAngleForRot3+rotStep3*irot;
      px[2]=tmpx*TMath::Cos(phirot2)-tmpy*TMath::Sin(phirot2);
      py[2]=tmpx*TMath::Sin(phirot2)+tmpy*TMath::Cos(phirot2);
    }
    tmpRD->SetPxPyPzProngs(nProngs,px,py,pz);
    pt = tmpRD->Pt();
    minv2 = tmpRD->InvMass2(nProngs,pdgdau);
    if(minv2>fMinMass*fMinMass && minv2<fMaxMass*fMaxMass){
      Double_t rapid = tmpRD->Y(pdgD);
      if(fAnalysisCuts->IsInFiducialAcceptance(pt,rapid)){
        massRot=TMath::Sqrt(minv2);
        fMassVsPtVsYRot->Fill(massRot,pt,rapid);
        nRotated++;
        fDeltaMass->Fill(massRot-mass);
        if(fFullAnalysis){
          Double_t pointRot[5]={mass,massRot-mass,ptOrig,pt-ptOrig,angleProngXY};
          fDeltaMassFullAnalysis->Fill(pointRot);
        }
      }
    }
    px[0]=tmpx;
    py[0]=tmpy;
    if(pdgD==411 || pdgD==431){
      px[2]=tmpx2;
      py[2]=tmpy2;
    }
  }
  fNormRotated->Fill(nRotated);
  
  return accept;
  
}
//________________________________________________________________________
void AliAnalysisTaskCombinHF::FillMEHistos(Int_t pdgD,Int_t nProngs, AliAODRecoDecay* tmpRD, Double_t* px, Double_t* py, Double_t* pz, UInt_t *pdgdau){
  // Fill histos for candidates in MixedEvents
    
  tmpRD->SetPxPyPzProngs(nProngs,px,py,pz);
  Double_t pt = tmpRD->Pt();
  Double_t minv2 = tmpRD->InvMass2(nProngs,pdgdau);
  Double_t mass=TMath::Sqrt(minv2);
  
  if(minv2>fMinMass*fMinMass && minv2<fMaxMass*fMaxMass){
    Double_t rapid = tmpRD->Y(pdgD);
    if(fAnalysisCuts->IsInFiducialAcceptance(pt,rapid)){
      fMassVsPtVsYME->Fill(mass,pt,rapid);
    }
  }
  return;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::IsTrackSelected(AliAODTrack* track){
  // track selection cuts
  
  if(track->Charge()==0) return kFALSE;
  if(track->GetID()<0&&!fKeepNegID)return kFALSE;
  if(!(track->TestFilterMask(fFilterMask))) return kFALSE;
  if(!SelectAODTrack(track,fTrackCutsAll)) return kFALSE;
  return kTRUE;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::IsKaon(AliAODTrack* track){
  // kaon selection cuts
  
  if(!fPidHF) return kTRUE;
  Int_t isKaon=fPidHF->MakeRawPid(track,AliPID::kKaon);
  Double_t mom=track->P();
  if(SelectAODTrack(track,fTrackCutsKaon)) {
    if(isKaon>=1)    return kTRUE;
    if(isKaon<=-1) return kFALSE;
    switch(fPIDselCaseZero){// isKaon=0
      case 0:
      {
        return kTRUE;// always accept
      }
        break;
      case 1:
      {
        if(isKaon>=0 && track->P()>fmaxPforIDKaon)return kTRUE;// accept only if in a compatibility band starting from p=fmaxPforIDKaon
      }
        break;
      case 2:
      {
        if(track->P()>fmaxPforIDKaon)return kTRUE;
        AliPIDResponse *pidResp=fPidHF->GetPidResponse();// more elaborated strategy: asymmetric cuts, with fix momenta and nsigma ranges for the moment
        Double_t nsigma=pidResp->NumberOfSigmasTPC(track,AliPID::kKaon);
        if(nsigma>-2.&& nsigma<3. && mom<0.6)isKaon=1;
        else if(nsigma>-1.&& nsigma<3.&& mom<0.8)isKaon=1;
        if(isKaon==1)return kTRUE;
      }
        break;
      default:
      {
        AliWarning(Form("WRONG CASE OF PID STRATEGY SELECTED: %d (can range from 0 to 2)",fPIDselCaseZero));
        return kFALSE;// actually case 0 could be set as the default and return kTRUE
      }
    }
  }
  
  return kFALSE;
}
//_______________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::IsPion(AliAODTrack* track){
  // pion selection cuts
  
  if(!fPidHF) return kTRUE;
  Int_t isPion=fPidHF->MakeRawPid(track,AliPID::kPion);
  Double_t mom=track->P();
  if(SelectAODTrack(track,fTrackCutsPion)) {
    if(isPion>=1)    return kTRUE;
    if(isPion<=-1) return kFALSE;
    switch(fPIDselCaseZero){// isPion=0
      case 0:
      {
        return kTRUE;// always accept
      }
        break;
      case 1:
      {
        if(track->P()>fmaxPforIDPion)return kTRUE;// accept only if in a compatibility band starting from p=fmaxPforIDPion
      }
        break;
      case 2:
      {
        // more elaborated strategy: asymmetric cuts, with fix momenta and nsigma ranges for the moment
        if(track->P()>fmaxPforIDPion)return kTRUE;
        AliPIDResponse *pidResp=fPidHF->GetPidResponse();
        Double_t nsigma=pidResp->NumberOfSigmasTPC(track,AliPID::kPion);
        if(nsigma<2.&& nsigma>-3. && mom<0.6)isPion=1;
        else if(nsigma<1. && nsigma> -3. && mom<0.8)isPion=1;
        if(isPion==1)return kTRUE;
      }
        break;
      default:
      {
        AliWarning(Form("WRONG CASE OF PID STRATEGY SELECTED: %d (can range from 0 to 2)",fPIDselCaseZero));
        return kFALSE;// actually case 0 could be set as the default and return kTRUE
      }
    }
  }
  
  return kFALSE;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::SelectAODTrack(AliAODTrack *track, AliESDtrackCuts *cuts){
  // AOD track selection
  
  if(!cuts) return kTRUE;
  
  AliESDtrack esdTrack(track);
  // set the TPC cluster info
  esdTrack.SetTPCClusterMap(track->GetTPCClusterMap());
  esdTrack.SetTPCSharedMap(track->GetTPCSharedMap());
  esdTrack.SetTPCPointsF(track->GetTPCNclsF());
  if(!cuts->IsSelected(&esdTrack)) return kFALSE;
  
  return kTRUE;
}

//_________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::CheckAcceptance(TClonesArray* arrayMC,Int_t nProng, Int_t *labDau){
  // check if the decay products are in the good eta and pt range
  for (Int_t iProng = 0; iProng<nProng; iProng++){
    AliAODMCParticle* mcPartDaughter=dynamic_cast<AliAODMCParticle*>(arrayMC->At(labDau[iProng]));
    if(!mcPartDaughter) return kFALSE;
    Double_t eta = mcPartDaughter->Eta();
    Double_t pt = mcPartDaughter->Pt();
    if (TMath::Abs(eta) > fEtaAccCut || pt < fPtAccCut) return kFALSE;
  }
  return kTRUE;
}
//_________________________________________________________________
Bool_t AliAnalysisTaskCombinHF::CanBeMixed(Double_t zv1, Double_t zv2, Double_t mult1, Double_t mult2){
  if(fUsePoolsZ && fzVertPoolLims){
    Int_t theBin1=TMath::BinarySearch(fNzVertPoolsLimSize,fzVertPoolLims,zv1);
    Int_t theBin2=TMath::BinarySearch(fNzVertPoolsLimSize,fzVertPoolLims,zv2);
    if(theBin1!=theBin2) return kFALSE;
    if(theBin1<0 || theBin2<0) return kFALSE;
    if(theBin1>=fNzVertPools || theBin2>=fNzVertPools) return kFALSE;    
  }else{
    if(TMath::Abs(zv2-zv1)>fMaxzVertDistForMix) return kFALSE;
  }

  if(fUsePoolsM && fMultPoolLims){
    Int_t theBin1=TMath::BinarySearch(fNMultPoolsLimSize,fMultPoolLims,mult1);
    Int_t theBin2=TMath::BinarySearch(fNMultPoolsLimSize,fMultPoolLims,mult2);
    if(theBin1!=theBin2) return kFALSE;
    if(theBin1<0 || theBin2<0) return kFALSE;
    if(theBin1>=fNMultPools || theBin2>=fNMultPools) return kFALSE;
  }else{
    if(TMath::Abs(mult2-mult1)>fMaxMultDiffForMix) return kFALSE;
  }
  return kTRUE;
}

//_________________________________________________________________
void AliAnalysisTaskCombinHF::FinishTaskOutput()
{
  // perform mixed event analysis
  if(!fDoEventMixing) return;

  Int_t nEvents=fEventBuffer->GetEntries();
  printf("Start Event Mixing of %d events\n",nEvents);

  TObjArray* karray=0x0;
  TObjArray* parray=0x0;
  Double_t zVertex,mult;
  fEventBuffer->SetBranchAddress("karray", &karray);
  fEventBuffer->SetBranchAddress("parray", &parray);
  fEventBuffer->SetBranchAddress("zVertex", &zVertex);
  fEventBuffer->SetBranchAddress("multiplicity", &mult);

  // dummy values of track impact parameter, needed to build an AliAODRecoDecay object
  Double_t d02[2]={0.,0.};
  Double_t d03[3]={0.,0.,0.};
  AliAODRecoDecay* tmpRD2 = new AliAODRecoDecay(0x0,2,0,d02);
  AliAODRecoDecay* tmpRD3 = new AliAODRecoDecay(0x0,3,1,d03);
  UInt_t pdg0[2]={321,211};
  UInt_t pdgp[3]={321,211,211};
  Double_t px[3],py[3],pz[3];

  for(Int_t iEv1=0; iEv1<nEvents; iEv1++){
    fEventBuffer->GetEvent(iEv1);
    Double_t zVertex1=zVertex;
    Double_t mult1=mult;
    TObjArray* karray1=new TObjArray(*karray);
    for(Int_t iEv2=0; iEv2<fMaxNumberOfEventsForMixing; iEv2++){
      Int_t iToMix=iEv1+iEv2+1;
      if(iEv1>=(nEvents-fMaxNumberOfEventsForMixing)) iToMix=iEv1-iEv2-1;
      if(iToMix<0) continue;
      if(iToMix==iEv1) continue;
      fEventBuffer->GetEvent(iToMix);
      Double_t zVertex2=zVertex;
      Double_t mult2=mult;
      if(CanBeMixed(zVertex1,zVertex2,mult1,mult2)){
	TObjArray* parray2=new TObjArray(*parray);
	Int_t nKaons=karray1->GetEntries();
	Int_t nPions=parray2->GetEntries();
	Bool_t doThird=kTRUE;
	Int_t iToMix3=iEv1+2*fMaxNumberOfEventsForMixing+1;
	if(iToMix3>=nEvents) iToMix3=iEv1-2*fMaxNumberOfEventsForMixing-1;
	if(iToMix3<0 || iToMix3==iEv1 || iToMix3==iToMix) doThird=kFALSE;
	TObjArray* parray3=0x0;
	Double_t zVertex3=-999.;
	Double_t mult3=999999;
	if(fMeson!=kDzero && doThird){
	  fEventBuffer->GetEvent(iToMix3);
	  zVertex3=zVertex;
	  mult3=mult;
	  if(CanBeMixed(zVertex1,zVertex3,mult1,mult3)){
	    parray3=new TObjArray(*parray);
	  }else{
	    doThird=kFALSE;
	  }
	}

	for(Int_t iTr1=0; iTr1<nKaons; iTr1++){
	  TLorentzVector* trK=(TLorentzVector*)karray1->At(iTr1);
	  Double_t chargeK=trK->T();
	  px[0] = trK->Px();
	  py[0] = trK->Py();
	  pz[0] = trK->Pz();
	  for(Int_t iTr2=0; iTr2<nPions; iTr2++){
	    TLorentzVector* trPi1=(TLorentzVector*)parray2->At(iTr2);
	    Double_t chargePi1=trPi1->T();
	    px[1] = trPi1->Px();
	    py[1] = trPi1->Py();
	    pz[1] = trPi1->Pz();
	    if(chargePi1*chargeK<0){
	      if(fMeson==kDzero){
		FillMEHistos(421,2,tmpRD2,px,py,pz,pdg0);
	      }else{
		if(doThird && parray3){
		  Int_t nPions3=parray3->GetEntries();
		  for(Int_t iTr3=iTr2+1; iTr3<nPions3; iTr3++){
		    TLorentzVector* trPi2=(TLorentzVector*)parray3->At(iTr3);
		    Double_t chargePi2=trPi2->T();
		    px[2] = trPi2->Px();
		    py[2] = trPi2->Py();
		    pz[2] = trPi2->Pz();
		    if(chargePi2*chargeK<0){
		      FillMEHistos(411,3,tmpRD3,px,py,pz,pdgp);
		    }
		  }
		}
	      }
	    }
	  }
	}
	delete parray3;
	delete parray2;
      }
    }
    delete karray1;
  }
  delete tmpRD2;
  delete tmpRD3;
}
//_________________________________________________________________
void AliAnalysisTaskCombinHF::Terminate(Option_t */*option*/)
{
  // Terminate analysis
  //
  if(fDebug > 1) printf("AliAnalysisTaskCombinHF: Terminate() \n");
  fOutput = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutput) {
    printf("ERROR: fOutput not available\n");
    return;
  }
  fHistNEvents = dynamic_cast<TH1F*>(fOutput->FindObject("hNEvents"));
  if(fHistNEvents){
    printf("Number of analyzed events = %d\n",(Int_t)fHistNEvents->GetBinContent(2));
  }else{
    printf("ERROR: fHistNEvents not available\n");
    return;
  }
  return;
}

