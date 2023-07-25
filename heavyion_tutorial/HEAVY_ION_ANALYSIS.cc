// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Tools/AliceCommon.hh"
#include "Rivet/Projections/AliceCommon.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class HEAVY_ION_ANALYSIS : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(HEAVY_ION_ANALYSIS);


    /// @name Analysis methods
    /// @{

    /// Book histograms and initialise projections before the run
    void init() {

      // Declare centrality projection
      const FinalState fs(Cuts::pT > 1*GeV);
      declare(fs, "fs");


      declareCentrality(ALICE::V0MMultiplicity(), "ALICE_2015_PBPBCentrality", "V0M", "V0M");

      // Book histograms
      book(h["charged_pT_cent0030"], "charged_pT_cent0030", 60, 0.0, 30.0);
      book(h["neutral_pT_cent0030"], "neutral_pT_cent0030", 60, 0.0, 30.0);

      book(h["charged_pT_cent3060"], "charged_pT_cent3060", 60, 0.0, 30.0);
      book(h["neutral_pT_cent3060"], "neutral_pT_cent3060", 60, 0.0, 30.0);

      book(h["charged_pT_cent6090"], "charged_pT_cent6090", 60, 0.0, 30.0);
      book(h["neutral_pT_cent6090"], "neutral_pT_cent6090", 60, 0.0, 30.0);

      book(sow["sow_cent0030"], "sow_cent0030");
      book(sow["sow_cent3060"], "sow_cent3060");
      book(sow["sow_cent6090"], "sow_cent6090");
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      //! get the final state particles!
      Particles fsParticles = applyProjection<FinalState>(event,"fs").particles();

      const CentralityProjection& cent = apply<CentralityProjection>(event,"V0M");
      const double c = cent();

      if (c > 90.) vetoEvent;

      if((c >= 0.) && (c < 30.)){
	sow["sow_cent0030"]->fill();

	//! Loop over all the particles
	for(const Particle& p : fsParticles){
	  if(p.isCharged())
	    h["charged_pT_cent0030"]->fill(p.pT()/GeV);
	  else
	    h["neutral_pT_cent0030"]->fill(p.pT()/GeV);
	}
      }
      else if((c >= 30.) && (c < 60.)){
	sow["sow_cent3060"]->fill();

	//! Loop over all the particles
	for(const Particle& p : fsParticles){
	  if(p.isCharged())
	    h["charged_pT_cent3060"]->fill(p.pT()/GeV);
	  else
	    h["neutral_pT_cent3060"]->fill(p.pT()/GeV);
	}
      }
      else if((c >= 60.) && (c < 90.)){
	sow["sow_cent6090"]->fill();

	//! Loop over all the particles
	for(const Particle& p : fsParticles){
	  if(p.isCharged())
	    h["charged_pT_cent6090"]->fill(p.pT()/GeV);
	  else
	    h["neutral_pT_cent6090"]->fill(p.pT()/GeV);
	}
      }
    }
    


    /// Normalise histograms etc., after the run
    void finalize() {

      h["charged_pT_cent0030"]->scaleW(1./sow["sow_cent0030"]->sumW());
      h["neutral_pT_cent0030"]->scaleW(1./sow["sow_cent0030"]->sumW());

      h["charged_pT_cent3060"]->scaleW(1./sow["sow_cent3060"]->sumW());
      h["neutral_pT_cent3060"]->scaleW(1./sow["sow_cent3060"]->sumW());

      h["charged_pT_cent6090"]->scaleW(1./sow["sow_cent6090"]->sumW());
      h["neutral_pT_cent6090"]->scaleW(1./sow["sow_cent6090"]->sumW());
      
    }

    /// @}


    /// @name Histograms
    /// @{
    map<string, Histo1DPtr> h;
    map<string, CounterPtr> sow;
    /// @}


  };


  RIVET_DECLARE_PLUGIN(HEAVY_ION_ANALYSIS);

}
