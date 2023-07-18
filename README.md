# RIVET/JETSCAPE TUTORIAL

## Installation
Install JETSCAPE FIFO: https://github.com/tmengel/JETSCAPEFIFO/tree/main

### Step 1: Setup
Open the docker container and source Rivet.
```
docker start -ai myJetscapeRivet

source ~/RIVET/local/rivetenv.sh
```

### Step 2: Make Analysis
Get the template files for the analysis.
```
mkdir rivet_analyses

cd rivet_analyses

rivet-mkanalysis STAR_2006_I709170
```
There should be four files in the rivet_analyses: STAR_2006_I709170.cc, STAR_2006_I709170.plot, STAR_2006_I709170.info, and STAR_2006_I709170.yoda.


### Step 3: Edit JETSCAPE file

Open another terminal outside of the docker container. Using your preferred text editor, modify JETSCAPEFIFO/config/jetscape_fifo.xml. (A modified version is included this directory.)

### Step 4: Edit files for Rivet analysis
#### STAR_2006_I709170.cc
Outside of the docker container and using your preferred text editor, modify rivet_analyses/STAR_2006_I709170.cc. (A modified version is included this directory.)

#### STAR_2006_I709170.plot
Outside of the docker container and using your preferred text editor, modify rivet_analyses/STAR_2006_I709170.plot. (A modified version is included this directory.)


### Step 5: Compile and run JETSCAPE and Rivet
Inside the docker container, compile the Rivet analysis. Then, run JETSCAPE and Rivet in fifo mode. We will apply a 20 second sleep delay to Rivet, so that JETSCAPE has time to begin writing to myfifo.hepmc.

```
cd ~/rivet_analyses

rivet-build RivetSTAR_2006_I709170.so STAR_2006_I709170.cc

cd ../JETSCAPEFIFO/build

./runJetscape ../config/jetscape_fifo.xml & sleep 20 && rivet --analysis-path ../../rivet_analyses -a STAR_2006_I709170 --ignore-beams -o ../../rivet_analyses/pp200.yoda myfifo.hepmc
```
### Step 6: Make plots
We can plot the analysis using rivet-mkhtml using the output file pp200.yoda.

```
cd ../../rivet_analyses

rivet-mkhtml pp200.yoda
```
