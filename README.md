# RIVET/JETSCAPE TUTORIAL

## Installation
Install JETSCAPE FIFO  

### Step 1:
Open the docker container and source Rivet.
    ```
    docker start -ai myJetscapeRivet
    source ~/RIVET/local/rivetenv.sh
    ```

### Step 1: Create New Docker Container
 Get the template files for the analysis.
```
mkdir rivet_analyses
cd rivet_analyses

rivet-mkanalysis STAR_2006_I709170
```
There should be four files in the rivet_analyses: STAR_2006_I709170.cc, STAR_2006_I709170.plot, STAR_2006_I709170.info, and STAR_2006_I709170.yoda.


### Step 2: Edit JETSCAPE file

Open another terminal outside of the docker container. Using your preferred text editor, modify JETSCAPEFIFO/config/jetscape_fifo.xml. Change the following

<nEvents> 10000 </nEvents>

<Hard>
  <PythiaGun>
    <pTHatMin>0</pTHatMin>
    <pTHatMax>1000</pTHatMax>
    <eCM>200</eCM>
  </PythiaGun>
</Hard>


### Step 3: Edit files for Rivet analysis
#### STAR_2006_I709170.cc
Outside of the docker container and using your preferred text editor, modify rivet_analyses/STAR_2006_I709170.cc.

void init() {
  IdentifiedFinalState pionfs(Cuts::abseta < 2.5 && Cuts::pT > 0.3*GeV); pionfs.acceptIdPair(PID::PIPLUS);
  declare(pionfs, "PionFS");

  book(hPionPt["piplus"], 2, 1, 1);
  book(hPionPt["piminus"], 7, 1, 1);
  book(Ratio["piminus_piplus"], 23, 1, 2);
  book(sow["sow_pp"],"sow_pp"); }

void analyze(const Event& event) {
  const IdentifiedFinalState& pionfs = apply<IdentifiedFinalState>(event, "PionFS");

  for (const Particle& p : pionfs.particles()) {
    if (p.absrap() < 0.5) {
      const double pT = p.pT() / GeV;
      ((p.pid() > 0) ? hPionPt["piplus"] : hPionPt["piminus"])->fill(pT, 1.0/pT); }
  }
  sow["sow_pp"]->fill();
}

void finalize() {
  divide(hPionPt["piminus"], hPionPt["piminus"], Ratio["piminus_piplus"]);

  const YODA::Scatter1D factor = (1./(2.*M_PI)) / *sow["sow_pp"];
    scale(hPionPt["piplus"], factor);
    scale(hPionPt["piminus"], factor);
}

map<string, Histo1DPtr> hPionPt;
map<string, Scatter2DPtr> Ratio;
map<string, CounterPtr> sow;


#### STAR_2006_I709170.plot
Outside of the docker container and using your preferred text editor, modify rivet_analyses/STAR_2006_I709170.plot.

# BEGIN PLOT /STAR_2006_I709170/d02-x01-y01 Title=$\pi^+$ transverse momentum
XLabel=$p_\perp$ [GeV]
YLabel=$1/(2\pi) \, d^2N/(p_\perp dy dp_\perp)$ [GeV$^{-2}$] FullRange=1
# END PLOT
# BEGIN PLOT /STAR_2006_I709170/d07-x01-y01 Title=$\pi^-$ transverse momentum
XLabel=$p_\perp$ [GeV]
YLabel=$1/(2\pi) \, d^2N/(p_\perp dy dp_\perp)$ [GeV$^{-2}$] FullRange=1
# END PLOT
# BEGIN PLOT /STAR_2006_I709170/d23-x01-y02 Title=Ratio of $\pi^- / \pi^+$ as function of $p_\perp$ XLabel=$p_\perp$ [GeV]
YLabel=$\pi^- / \pi^+$
LogY=0 YMin=0 YMax=2
# END PLOT


### Step 4: Compile and run JETSCAPE and Rivet
Inside the docker container, compile the Rivet analysis. Then, run JETSCAPE and Rivet in fifo mode. We will apply a 20 second sleep delay to Rivet, so that JETSCAPE has time to begin writing to myfifo.hepmc.

```
cd ~/rivet_analyses

rivet-build RivetSTAR_2006_I709170.so STAR_2006_I709170.cc

cd ../JETSCAPEFIFO/build

./runJetscape ../config/jetscape_fifo.xml & sleep 20 && rivet --analysis-path ../../rivet_analyses -a STAR_2006_I709170 --ignore-beams -o ../../rivet_analyses/pp200.yoda myfifo.hepmc

```
### Step 5: Make plots
We can plot the analysis using rivet-mkhtml using the output file pp200.yoda.

```
cd ../../rivet_analyses

rivet-mkhtml pp200.yoda

```
