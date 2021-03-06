/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskUPCforward_H
#define AliAnalysisTaskUPCforward_H

#include "AliAnalysisTaskSE.h"
#include "TBits.h"

class AliMuonTrackCuts; 											// Include class for standard muon tack cuts

/**
 * \file AliAnalysisTaskUPCforward.h
 * \brief Contains the declaration of the AliAnalysisTaskUPCforward class
 */

/**
 * \class AliAnalysisTaskUPCforward
 * \brief Contains the tools to every part of my analysis
 */
class AliAnalysisTaskUPCforward : public AliAnalysisTaskSE
{
    public:
                                /**
                                 * Create a new AliAnalysisTaskUPCforward with
                                 * default constructor. Based on my understanding
                                 * this is mostly related to input/output
                                 * processes.
                                 */
                                AliAnalysisTaskUPCforward();

                                /**
                                 * Create a new AliAnalysisTaskUPCforward with
                                 * a specific name. This serves to instantiate
                                 * the AliAnalysisTaskSE object built with the
                                 * constructor, that will take this specific
                                 * name.
                                 *
                                 * \param name , the name taken by the AliAnalysisTaskSE object.
                                 */
                                AliAnalysisTaskUPCforward(const char *name);

                                /**
                                 * Virtual destructor for the class. It will
                                 * take care of all the particular objects to be
                                 * destroyed for the class.
                                 */
        virtual                 ~AliAnalysisTaskUPCforward();

                                /**
                                 * The function related to the instantiation of
                                 * all the histograms and the output list.
                                 */
        virtual void            UserCreateOutputObjects();

                                /**
                                 * Everything happens here. Here, the cuts are
                                 * applied, the histograms are filled and the
                                 * J/Psi peak is manifested inside the histograms.
                                 *
                                 * \param option , actually it is not used for now...
                                 */
        virtual void            UserExec(Option_t* option);

                                /**
                                 * Called at the END of the analysis (when all
                                 * events are processed). But it is not actually
                                 * doing anything! I guess it is mostly needed
                                 * for I/O purposes and GRID interfacing...
                                 */
        virtual void            Terminate(Option_t* option);

                                /**
                                 * Implement the NotifyRun to search for the new
                                 * parameters at each new runs. Sets the run
                                 * number for the successive cuts.
                                 */
        virtual void   			    NotifyRun();

                                /**
                                 * This will fill the vector containing the good
                                 * run numbers. For now this function will be
                                 * inside the constructor of the class.
                                 */
        void   			            FillGoodRunVector(std::vector<Int_t> &fVectorGoodRunNumbers);

                                /**
                                 * Use the class as a data member. It contains
                                 * the cuts for the muon track.
                                 */
        AliMuonTrackCuts* 		  fMuonTrackCuts;



        // Bool_t                  CheckIfPassedCuts(vector<>)

    private:

                                /// The input events for the analysis.
        AliAODEvent*            fAOD;               //!

                                /**
                                 * The output list containing all the histograms
                                 * required for the analysis. In a second time
                                 * I will probably make it so to include every
                                 * possible cut variation to better compute the
                                 * systematics.
                                 */
        TList*                  fOutputList;        //!

                                /**
                                 * Utility type histo. It counts the GOOD muons
                                 * per event.
                                 */
        TH1F*                   fNumberMuonsH;      //!

                                /**
                                 * In this histogram the number of events passing
                                 * each cut is recorded. So it is highest at the
                                 * 0-th cut, and lowest, even possibly null, at
                                 * the last possible cut.
                                 */
        TH1F*                   fCounterH;          //!

                                /**
                                 * As far as I understand, it should be the
                                 * pseudorapidity distribution of the many muons.
                                 */
	      TH1F*                   fEtaMuonH;          //!

                                /**
                                 *
                                 */
	      TH1F*                   fRAbsMuonH;         //!


                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system.
                                 */
        TH1F*                   fInvariantMassDistributionH;         //!

                                /**
                                 * This histogram records the number of entries
                                 * against the respective run number.
                                 */
        TH1F*                   fEntriesAgainstRunNumberH;         //!

                                /**
                                 * This histogram records the number of entries
                                 * against the respective run number, but the
                                 * binning is done properly, like normally
                                 * found for this type of histograms.
                                 */
        TH1F*                   fEntriesAgainstRunNumberProperlyH;         //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt < 0.25 GeV/c for pt of the dimuon pair.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt > 0.25 GeV/c for pt of the dimuon pair.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentH;     //!

                                /**
                                 * This histogram records the pt-ditribution
                                 * of the dimuon pairs.
                                 */
        TH1F*                   fDimuonPtDistributionH;         //!

                                /**
                                 * This histogram records the energy distri-
                                 * bution of the neutron ZDC. This plot should
                                 * show the relative components of the 0 neutron
                                 * peak, the 1 neutron peak and possibly the
                                 * 2 neutrons peak. Anything higher than that,
                                 * requires help from the user and is more like
                                 * a guess...
                                 */
        TH1F*                   fZNCEnergyAgainstEntriesH;         //!

                                /**
                                 * This histogram records the energy distri-
                                 * bution of the neutron ZDC. This plot should
                                 * show the relative components of the 0 neutron
                                 * peak, the 1 neutron peak and possibly the
                                 * 2 neutrons peak. Anything higher than that,
                                 * requires help from the user and is more like
                                 * a guess...
                                 */
        TH1F*                   fZNAEnergyAgainstEntriesH;         //!


                                /**
                                 * This histogram records the invariant mass
                                 * distribution pt-integrated
                                 * of the dimuon pairs when the
                                 * neutron ZNC has not seen any neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionNoNeutronsH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution pt-integrated
                                 * of the dimuon pairs when the
                                 * neutron ZNC has seen a SINGLE neutron.
                                 */
        TH1F*                   fInvariantMassDistributionOneNeutronH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution pt-integrated
                                 * of the dimuon pairs when the
                                 * neutron ZNC has seen at LEAST a single
                                 * neutron.
                                 */
        TH1F*                   fInvariantMassDistributionAtLeastOneNeutronH;//!


                                /**
                                 * This histogram records the invariant mass
                                 * distribution for the COHERENT component
                                 * of the dimuon pairs when the
                                 * neutron ZNC has not seen any neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentNoNeutronsH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution for the COHERENT component
                                 * of the dimuon pairs when the
                                 * neutron ZNC has seen a SINGLE neutron.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentOneNeutronH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution for the COHERENT component
                                 * of the dimuon pairs when the
                                 * neutron ZNC has seen at LEAST a single
                                 * neutron.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentAtLeastOneNeutronH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution for the INCOHERENT component
                                 * of the dimuon pairs when the
                                 * neutron ZNC has not seen any neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentNoNeutronsH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution for the INCOHERENT component
                                 * of the dimuon pairs when the
                                 * neutron ZNC has seen a SINGLE neutron.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentOneNeutronH;//!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution for the INCOHERENT component
                                 * of the dimuon pairs when the
                                 * neutron ZNC has seen at LEAST a single
                                 * neutron.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentAtLeastOneNeutronH;//!

                                /**
                                 * This histogram records the time ditribution
                                 * of neutron ZDC.
                                 */
        TH1F*                   fZNCTimeAgainstEntriesH;         //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, but the
                                 * novelty is the use of a different method to
                                 * extract the data. While before we were always
                                 * using the fMomentum, this histogram will be
                                 * filled wth the information from the
                                 * fMomentumAtDCA, hopefully this may lead to a
                                 * change in the sigma of the peaks. If not, it
                                 * will just do nothing!
                                 */
        TH1F*                   fInvariantMassDistributionAtDcaH;         //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, but the
                                 * novelty is the use of a different method to
                                 * extract the data. While before we were always
                                 * using the fMomentum, this histogram will be
                                 * filled wth the information from the
                                 * fMomentumAtDCA, hopefully this may lead to a
                                 * change in the sigma of the peaks. If not, it
                                 * will just do nothing!
                                 */
        TH2F*                   fDcaAgainstInvariantMassH;         //!


        //_______________________________
        // Cloned histograms with EXTENDED Range (0,20)->(0,40).

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system.
                                 */
        TH1F*                   fInvariantMassDistributionExtendedH;         //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt < 0.25 GeV/c for pt of the dimuon pair.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentExtendedH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt > 0.25 GeV/c for pt of the dimuon pair.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentExtendedH;     //!
        // Finished cloned histograms.
        //_______________________________


        //_______________________________
        // DIFFERENTIAL NEUTRON EMISSION PLOTS

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt < 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has 0 neutrons.
                                 * The ZNA has 0 neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentZNCzeroZNAzeroH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt < 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has 0 neutrons.
                                 * The ZNA has ANY neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentZNCzeroZNAanyH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt < 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has ANY neutrons.
                                 * The ZNA has 0 neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentZNCanyZNAzeroH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt < 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has ANY neutrons.
                                 * The ZNA has ANY neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionCoherentZNCanyZNAanyH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt > 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has 0 neutrons.
                                 * The ZNA has 0 neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentZNCzeroZNAzeroH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt > 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has 0 neutrons.
                                 * The ZNA has ANY neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentZNCzeroZNAanyH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt > 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has ANY neutrons.
                                 * The ZNA has 0 neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentZNCanyZNAzeroH;       //!

                                /**
                                 * This histogram records the invariant mass
                                 * distribution of the dimuon system, only
                                 * coherent component, so as to say, only
                                 * pt > 0.25 GeV/c for pt of the dimuon pair.
                                 * The ZNC has ANY neutrons.
                                 * The ZNA has ANY neutrons.
                                 */
        TH1F*                   fInvariantMassDistributionIncoherentZNCanyZNAanyH;       //!


        // END DIFFERENTIAL NEUTRON EMISSION PLOTS
        //_______________________________

        //_______________________________
        // CUTS
        /*
         * The following is all the possible checks for the event selections
         * and the track selection as well. Enjoy.
         */
        Int_t                   fRunNum;        //!
        Int_t                   fTracklets;     //!

        UInt_t                  fL0inputs;      //!
      	UInt_t                  fL1inputs;      //!

      	Double_t                fZem1Energy;    //!
      	Double_t                fZem2Energy;    //!

      	Double_t                fZNCEnergy;     //!
      	Double_t                fZNAEnergy;     //!
      	Double_t                fZPCEnergy;     //!
      	Double_t                fZPAEnergy;     //!
      	Double_t                fZNATDC[4];     //!
      	Double_t                fZNCTDC[4];     //!
      	Double_t                fZPATDC[4];     //!
      	Double_t                fZPCTDC[4];     //!
      	Double_t                fZNATime;       //!
      	Double_t                fZNCTime;       //!
      	Int_t                   fV0ADecision;   //!
      	Int_t                   fV0CDecision;   //!
      	Int_t                   fADADecision;   //!
      	Int_t                   fADCDecision;   //!
        TBits                   fIR1Map;        //!
        TBits                   fIR2Map;        //!


        Bool_t                  fV0Hits[64];    //!
        Int_t                   fV0TotalNCells; //!
        //_______________________________

        /**
         * This is the vector containing the GOOD RunNumbers.
         */
        std::vector<Int_t> fVectorGoodRunNumbers;



        /**
         * Not implemented yet...
         */
        AliAnalysisTaskUPCforward(const AliAnalysisTaskUPCforward&);

        /**
         * Not implemented yet...
         */
        AliAnalysisTaskUPCforward& operator=(const AliAnalysisTaskUPCforward&);


        /**
         * This is important for ROOT only. I do not remember the reason anymore.
         * If I happen to encounter it again in the future, I will make sure to
         * record it!
         */
        ClassDef(AliAnalysisTaskUPCforward, 1);
};

#endif
