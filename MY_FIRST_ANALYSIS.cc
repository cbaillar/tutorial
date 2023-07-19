// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/DirectFinalState.hh"

namespace Rivet {

  /// @brief Add a short analysis description here
  class MY_FIRST_ANALYSIS : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(MY_FIRST_ANALYSIS);

    
    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections

      // The basic final-state projection:
      // all final-state particles within
      // the given eta acceptance
      const FinalState fs(Cuts::abseta < 1.0);
      declare(fs, "fs");      
     
      // Book histograms
      book(_h["charged_pT"], "charged_pT", 60, 0.0, 30.0);
      book(_h["neutral_pT"], "neutral_pT", 60, 0.0, 30.0);

    }

    /// Perform the per-event analysis
    void analyze(const Event& event) {

      //! get the final state particles!
      Particles fsParticles = applyProjection<FinalState>(event,"fs").particles();

      //! Loop over all the particles
      for(const Particle& p : fsParticles){
	      if(p.isCharged())
	        _h["charged_pT"]->fill(p.pT()/GeV);
	      else
	       _h["neutral_pT"]->fill(p.pT()/GeV);	  
      }            

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      //normalize(_h["XXXX"]); // normalize to unity
      normalize(_h["charged_pT"], crossSection()/picobarn); // normalize to generated cross-section in pb (no cuts)
      normalize(_h["neutral_pT"], crossSection()/picobarn); // normalize to generated cross-section in pb (no cuts)
      //scale(_h["ZZZZ"], crossSection()/picobarn/sumW()); // norm to generated cross-section in pb (after cuts)

    }

    /// @name Histograms
    /// @{
    map<string, Histo1DPtr> _h;
    /// @}


  };


  RIVET_DECLARE_PLUGIN(MY_FIRST_ANALYSIS);

}
