// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"

namespace Rivet {

  class STAR_2006_I709170_OFFICIAL: public Analysis {
  public:

    /// Constructor
    STAR_2006_I709170_OFFICIAL()
      : Analysis("STAR_2006_I709170_OFFICIAL")
    {  }


    /// Book projections and histograms
    void init() {

      IdentifiedFinalState pionfs(Cuts::abseta < 2.5 && Cuts::pT > 0.3*GeV);
      pionfs.acceptIdPair(PID::PIPLUS);
      declare(pionfs, "PionFS");      

      book(_h_pT_piplus     ,2, 1, 1); // full range pion binning

      book(_sumWeightSelected, "_sumWeightSelected");
      
    }


    /// Do the analysis
    void analyze(const Event& event) {

      const IdentifiedFinalState& pionfs = apply<IdentifiedFinalState>(event, "PionFS");
      for (const Particle& p : pionfs.particles()) {
        if (p.absrap() < 0.5) {
          const double pT = p.pT() / GeV;
	  if (p.pid() > 0)
	    _h_pT_piplus->fill(pT, 1.0/pT);
	  
        }
      }
      
      _sumWeightSelected->fill();
      
    }


    /// Finalize
    void finalize() {
      const YODA::Scatter1D factor = (1./(2.*M_PI)) / *_sumWeightSelected;

      scale(_h_pT_piplus,     factor);
      cout<<"_sumWeightSelected = " << _sumWeightSelected->val()<<endl;
      cout<<"sumOfWeights()     = "<< sumOfWeights()<<endl;
      cout<<"crossSection()  = " << crossSection()/microbarn<<endl;
    }


  private:

    CounterPtr _sumWeightSelected;
    Histo1DPtr _h_pT_piplus;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(STAR_2006_I709170_OFFICIAL);

}
