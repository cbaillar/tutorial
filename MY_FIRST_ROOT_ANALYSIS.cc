// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/DirectFinalState.hh"

//! include root headers
#include "TH1F.h"
#include "TFile.h"

namespace Rivet {


  /// @brief Add a short analysis description here
  class MY_FIRST_ROOT_ANALYSIS : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(MY_FIRST_ROOT_ANALYSIS);


    /// @name Analysis methods
    /// @{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections

      // The basic final-state projection:
      // all final-state particles within
      // the given eta acceptance
      const FinalState fs(Cuts::abseta < 1.0);
      declare(fs, "fs");

      //! declare output file
      fOut = new TFile("outputfile.root", "RECREATE");
     
      // Book histograms
      _hCharged_pT = new TH1F("_hCharged_pT", "charged_pT", 60, 0.0, 30.0);
      _hNeutral_pT = new TH1F("_hNeutral_pT", "neutral_pT", 60, 0.0, 30.0);

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      //! get the final state particles!
      Particles fsParticles = applyProjection<FinalState>(event,"fs").particles();

      //! Loop over all the particles
      for(const Particle& p : fsParticles){
	if(p.isCharged())
	  _hCharged_pT->Fill(p.pT()/GeV);
	else
	  _hNeutral_pT->Fill(p.pT()/GeV);
      }            

    }


    
    /// Normalise histograms etc., after the run
    void finalize() {

      _hCharged_pT->Scale(1./crossSection()/picobarn);
      //! normalize root histogram to cross-section 
      _hNeutral_pT->Scale(1./crossSection()/picobarn);
      //! normalize root histogram to cross-section

      fOut->cd();
      fOut->Write();
      fOut->Close();
      
    }

  private:
    TFile * fOut;
    TH1F * _hCharged_pT;
    TH1F * _hNeutral_pT;
    
  };


  RIVET_DECLARE_PLUGIN(MY_FIRST_ROOT_ANALYSIS);

}
