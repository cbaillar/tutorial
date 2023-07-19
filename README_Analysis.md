# RIVET/JETSCAPE TUTORIAL

### Step 0: Get analysis to compare - 



### Step 0 - 
Get the template files for the analysis.
```
mkdir rivet_analyses


### Step 1 a: Make Analysis
Get the template files for the analysis.
```
mkdir rivet_analyses
cd rivet_analyses
rivet-mkanalysis MY_FIRST_ANALYSIS
```
this should create three files in the directory
MY_FIRST_ANALYSIS.cc, MY_FIRST_ANALYSIS.plot, MY_FIRST_ANALYSIS.info
and a directory rivet-plots - this we will ignore for now... 


### Step 1 b: Edit files for Rivet analysis
#### MY_FIRST_ANALYSIS.cc
Outside of the docker container, open up your favorite text text editor - lets start modifying the code to make a simple histogram that gives one histogram that produces charged pion spectra.

### Step 2 a: Edit JETSCAPE file

Open another terminal outside of the docker container. Using your preferred text editor, modify JETSCAPEFIFO/config/jetscape_fifo.xml.

### Step 2 b: Compile and run JETSCAPE
Inside the docker container, compile the Rivet analysis. Then, run JETSCAPE and Rivet in fifo mode. We will apply a 20 second sleep delay to Rivet, so that JETSCAPE has time to begin writing to myfifo.hepmc.

```
cd ~/rivet_analyses

rivet-build RivetSTAR_2006_I709170.so STAR_2006_I709170.cc

cd ../JETSCAPEFIFO/build

./runJetscape ../config/jetscape_fifo.xml
```
### Step 3: Run Rivet

```
rivet --analysis-path ../../rivet_analyses -a STAR_2006_I709170 --ignore-beams -o ../../rivet_analyses/pp200.yoda myfifo.hepmc
```

### Step 4: Make html (alternative)

```
cd ../../rivet_analyses

rivet-mkhtml pp200.yoda
```

### Step 5: Root Histograms


### Step 6: Rivet with Root
