// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

//! fastjet base headers 
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"

//! grooming and subtraction relevant headers 
#include "fastjet/contrib/SoftDrop.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/contrib/ConstituentSubtractor.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class JET_MASS_AA : public Analysis {
  public:

    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(JET_MASS_AA);

    /// Book histograms and initialise projections before the run
    void init() {

      const FinalState fs(Cuts::abseta < 1.0);
      declare(fs, "fs");

      //! parameters for splitting function
      z_cut_default=0.1;
      beta_default = 0;

      z_cut_strong=0.5;
      beta_strong = 2;
      
      
      // Book histograms
      //      book(_h["charged_particle_pT"], "charged_particle_pT", 20, 0.0, 10.0);

      book(_h["raw_jet_pT"], "raw_jet_pT", 20, 0.0, 100.0);
      book(_h["subtracted_jet_pT"], "subtracted_jet_pT", 20, 0.0, 100.0);
            
      
      book(_h["jet_Mass_R04_defGrooming_5_pT_10"], "jet_Mass_R04_defGrooming_5_pT_10", 20, 0.0, 10.0);
      
      book(_h["jet_Mass_R04_stgGrooming_5_pT_10"], "jet_Mass_R04_stgGrooming_5_pT_10", 20, 0.0, 10.0);


      book(_h["subtracted_jet_Mass_R04_defGrooming_5_pT_10"], "subtracted jet_Mass_R04_defGrooming_5_pT_10", 20, 0.0, 10.0);
      
      book(_h["subtracted_jet_Mass_R04_stgGrooming_5_pT_10"], "subtracted jet_Mass_R04_stgGrooming_5_pT_10", 20, 0.0, 10.0);
      

      
    }//! init function 


    /// Perform the per-event analysis
    void analyze(const Event& event) {


      //! get the final state particles and start clustering jets -
      Particles fsParticles = applyProjection<FinalState>(event,"fs").particles();

      //! necessary jet selectors 
      fastjet::Selector select_eta_R04  = fastjet::SelectorAbsEtaMax(0.6);
      fastjet::Selector select_pt   = fastjet::SelectorPtMin(5.0);
      fastjet::Selector select_both_R04 = select_pt && select_eta_R04;

      fastjet::JetDefinition jet_def_R04(fastjet::antikt_algorithm, 0.4);

      
      fastjet::contrib::RecursiveSymmetryCutBase::SymmetryMeasure  symmetry_measure = fastjet::contrib::RecursiveSymmetryCutBase::scalar_z;

      fastjet::contrib::SoftDrop sd_def(beta_default, z_cut_default,
					symmetry_measure, 0.4);

      fastjet::contrib::SoftDrop sd_stg(beta_strong, z_cut_strong,
					symmetry_measure, 0.4);
      
      PseudoJets parts;
      
      for(const Particle& p : fsParticles){
	parts.push_back(PseudoJet(p.px(), p.py(), p.pz(), p.E()));
      }

      //! particles in the event as pseudojets - parts

      fastjet::contrib::ConstituentSubtractor subtractor;

      fastjet::GridMedianBackgroundEstimator bge_rho(2,0.5);
      // Maximal pseudo-rapidity cut max_eta is used inside ConstituentSubtraction, but in GridMedianBackgroundEstimator, the range is specified by maximal rapidity cut. Therefore, it is important to apply the same pseudo-rapidity cut also for particles used for background estimation (specified by function "set_particles") and also derive the rho dependence on rapidity using this max pseudo-rapidity cut to get the correct rescaling function!
      
      subtractor.set_distance_type(fastjet::contrib::ConstituentSubtractor::deltaR); // free parameter for the type of distance between particle i and ghost k. There are two options: "deltaR" or "angle" which are defined as deltaR=sqrt((y_i-y_k)^2+(phi_i-phi_k)^2) or Euclidean angle between the momenta
      subtractor.set_max_distance(0.3); // free parameter for the maximal allowed distance between particle i and ghost k
      subtractor.set_alpha(1);  // free parameter for the distance measure (the exponent of particle pt). The larger the parameter alpha, the more are favoured the lower pt particles in the subtraction process
      subtractor.set_ghost_area(0.01); // free parameter for the density of ghosts. The smaller, the better - but also the computation is slower.
      subtractor.set_max_eta(1.0); // parameter for the maximal eta cut
      subtractor.set_background_estimator(&bge_rho); // specify the background estimator to estimate rho.
	      
      
      //! do the background subtraction -
      bge_rho.set_particles(parts);
	
      // the correction of the whole event with ConstituentSubtractor
      PseudoJets corrected_event=subtractor.subtract_event(parts);

      std::cout<<" total background rho = "<<bge_rho.rho()<<std::endl;
      std::cout<<" total background sigma = "<<bge_rho.sigma()<<std::endl;

      //! Analyze R = 0.4 Jets 
      fastjet::ClusterSequence cs_R04(parts, jet_def_R04);
      PseudoJets jets_R04 = select_both_R04(cs_R04.inclusive_jets(5.0));

      //! Analyze R = 0.4 bkg subtracted Jets 
      fastjet::ClusterSequence cs_sub_R04(corrected_event, jet_def_R04);
      PseudoJets jets_sub_R04 = select_both_R04(cs_sub_R04.inclusive_jets(5.0));
      
      
      if(jets_R04.size()!=0){
	for(const PseudoJet& jet : jets_R04){

	  _h["raw_jet_pT"]->fill(jet.pt());
	  
	  if(jet.pt() > 5 && jet.pt() < 10){
	    PseudoJet sd_def_jet = sd_def(jet);
	    if(sd_def_jet!=0)
	      _h["jet_Mass_R04_defGrooming_5_pT_10"]->fill(sd_def_jet.m());
	    PseudoJet sd_stg_jet = sd_stg(jet);
	    if(sd_stg_jet!=0)
	      _h["jet_Mass_R04_stgGrooming_5_pT_10"]->fill(sd_stg_jet.m());

	  }
	}
      }//! 0.4 jets
      
      
      if(jets_sub_R04.size()!=0){
	for(const PseudoJet& jet : jets_sub_R04){

	  _h["subtracted_jet_pT"]->fill(jet.pt());
	  
	  if(jet.pt() > 5 && jet.pt() < 10){
	    PseudoJet sd_def_jet = sd_def(jet);
	    if(sd_def_jet!=0)
	      _h["subtracted_jet_Mass_R04_defGrooming_5_pT_10"]->fill(sd_def_jet.m());
	    PseudoJet sd_stg_jet = sd_stg(jet);
	    if(sd_stg_jet!=0)
	      _h["subtracted_jet_Mass_R04_stgGrooming_5_pT_10"]->fill(sd_stg_jet.m());

	  }
	}
      }//! 0.4 jets 
      	   
      
    }//! analyze function 


    /// Normalise histograms etc., after the run
    void finalize() {

      //! normalize all histograms

      normalize(_h["raw_jet_pT"], crossSection()/picobarn);
      normalize(_h["subtracted_jet_pT"], crossSection()/picobarn);
      
      normalize(_h["jet_Mass_R04_defGrooming_5_pT_10"]);
      normalize(_h["jet_Mass_R04_stgGrooming_5_pT_10"]);
      
      normalize(_h["subtracted_jet_Mass_R04_defGrooming_5_pT_10"]);
      normalize(_h["subtracted_jet_Mass_R04_stgGrooming_5_pT_10"]);
      
    }//! finalize functiomn 

    double z_cut_default;
    double beta_default;

    double z_cut_strong;
    double beta_strong;
    
    /// @name Histograms
    map<string, Histo1DPtr> _h;

  private:

  };


  DECLARE_RIVET_PLUGIN(JET_MASS_AA);

}
