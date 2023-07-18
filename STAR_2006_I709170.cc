// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/DirectFinalState.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class STAR_2006_I709170 : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(STAR_2006_I709170);


    /// @name Analysis methods
    /// @{

    /// Book histograms and initialise projections before the run
    void init() {
      IdentifiedFinalState pionfs(Cuts::abseta < 2.5 && Cuts::pT > 0.3*GeV);
      pionfs.acceptIdPair(PID::PIPLUS);
      declare(pionfs, "PionFS");

      book(hPionPt["piplus"], 2, 1, 1);
      book(hPionPt["piminus"], 7, 1, 1);
      book(Ratio["piminus_piplus"], 23, 1, 2);

      book(sow["sow_pp"],"sow_pp");
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const IdentifiedFinalState& pionfs = apply<IdentifiedFinalState>(event, "PionFS");

      for (const Particle& p : pionfs.particles()) {
        if (p.absrap() < 0.5) {
          const double pT = p.pT() / GeV;
          ((p.pid() > 0) ? hPionPt["piplus"] : hPionPt["piminus"])->fill(pT, 1.0/pT);
        }
      }

      sow["sow_pp"]->fill();

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      divide(hPionPt["piminus"], hPionPt["piplus"], Ratio["piminus_piplus"]);

      const YODA::Scatter1D factor = (1./(2.*M_PI)) / *sow["sow_pp"];
      scale(hPionPt["piplus"], factor);
      scale(hPionPt["piminus"], factor);
    }

    /// @}


    /// @name Histograms
    /// @{
    map<string, Histo1DPtr> hPionPt;
    map<string, Scatter2DPtr> Ratio;
    map<string, CounterPtr> sow;

    /// @}


  };


  RIVET_DECLARE_PLUGIN(STAR_2006_I709170);

}
