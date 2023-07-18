# RIVET/JETSCAPE TUTORIAL

## Installation
Install JETSCAPE FIFO  https://github.com/tmengel/JETSCAPEFIFO/tree/main

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

#### STAR_2006_I709170.plot
Outside of the docker container and using your preferred text editor, modify rivet_analyses/STAR_2006_I709170.plot.


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
