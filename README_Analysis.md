# RIVET/JETSCAPE TUTORIAL

To start the docker - 
```
docker run -it -v ~/jetscape-rivet-docker:/home/jetscape-rivet-user --name myJetscapeRivet --rm -p 8888:8888 tmengel/jetscaperivet:latest
```

### Step 0: Get analysis to compare - 

Now if you do this outside the docker container - 
```
cd ~/jetscape-rivet-docker/rivet_analysis
git clone https://github.com/cbaillar/tutorial.git
```

Now if you do this inside the docker container - 
```
cd /home/jetscape-rivet-user/rivet_analysis
git clone https://github.com/cbaillar/tutorial.git
```

### Step 0 - a Now onwards - we are inside the docker container! 

Ask RIVET to make the template files for the analysis.
### Step 1 a: Make Analysis
Get the template files for the analysis.
```
cd /home/jetscape-rivet-user/rivet_analysis
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

We have a comparison analysis in the tutorial github directory which you can compare your code to - MY_FIRST_ANALYSIS_COMPARISON.cc

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
