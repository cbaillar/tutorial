# RIVET/JETSCAPE TUTORIAL

To start the docker - 
```
docker run -it -v ~/jetscape-rivet-docker:/home/jetscape-rivet-user --name myJetscapeRivet --rm -p 8888:8888 tmengel/jetscaperivet:latest
```

### Step 0 a: Get analyses to compare - 

Now if you do this outside the docker container - 
```
cd ~/jetscape-rivet-docker/
git clone https://github.com/cbaillar/tutorial.git
```

Now if you do this inside the docker container - 
```
cd /home/jetscape-rivet-user/
git clone https://github.com/cbaillar/tutorial.git
```

### Step 0 b: Now onwards - we are inside the docker container! 

Source the RIVET install.
```
source /home/jetscape-rivet-user/RIVET/local/rivetenv.sh
```
### Step 1 a: Make Analysis
Ask RIVET to make the template files for the analysis.
```
cd /home/jetscape-rivet-user
mkdir rivet_analyses
cd rivet_analyses
rivet-mkanalysis MY_FIRST_ANALYSIS
```
this should create three files in the directory
MY_FIRST_ANALYSIS.cc, MY_FIRST_ANALYSIS.plot, and MY_FIRST_ANALYSIS.info 

### Step 1 b: Edit files for Rivet analysis
#### MY_FIRST_ANALYSIS.cc
Outside of the docker container, open up your favorite text text editor - lets start modifying the code to make a simple histogram that gives one histogram that produces charged pion spectra.

We have a comparison analysis in the tutorial github directory which you can compare your code to - MY_FIRST_ANALYSIS_COMPARISON.cc

### Step 2 a: Edit JETSCAPE file

Open another terminal outside of the docker container. Using your preferred text editor, modify JETSCAPEFIFO/config/jetscape_fifo.xml.

Change the following values in the JETSCAPEFIFO/config/jetscape_fifo.xml file
```
  <nEvents> 100 </nEvents>

  <outputFilename>myHepmcoutput</outputFilename>
  <!-- <JetScapeWriterHepMCfifo> on </JetScapeWriterHepMCfifo> -->
  <JetScapeWriterHepMC> on </JetScapeWriterHepMC>

  <Hard>
    <PythiaGun>
      <pTHatMin>5</pTHatMin>
      <pTHatMax>40</pTHatMax>
      <eCM>200</eCM>
    </PythiaGun>
  </Hard>
```

### Step 2 b: Compile and run JETSCAPE
Inside the docker container, compile the Rivet analysis. then lets run jetscape to get our output files!

```
cd /home/jetscape-rivet-user/rivet_analyses

rivet-build RivetMY_FIRST_ANALYSIS.so MY_FIRST_ANALYSIS.cc

cd ../JETSCAPEFIFO/build

./runJetscape ../config/jetscape_fifo.xml
```
this should give you an output hepmc file! 

### Step 2 c : INCASE you dont have jetscape running in the docker yet - please download a sample hepmc file from here - 
https://drive.google.com/file/d/11ULqu_4qISVTcNnEInUBxIbFcyll2r3z/view?usp=sharing

### Step 3: Run Rivet

```
cd /home/jetscape-rivet-user/rivet_analyses 
rivet --pwd -a MY_FIRST_ANALYSIS --ignore-beams -o pp200.yoda /home/jetscape-rivet-user/JETSCAPEFIFO/build/myHepmcoutput.hepmc
```

this should run and give you a bunch of output! lets take a look at the yoda file :) 

```
ls -alrth
less pp200.yoda
```

Lets have some fun now! add ROOT into the game! 

### Step 5: Adding Root Histograms

there is a separate analysis in this repo called MY_FIRST_ROOT_ANALYSIS.cc, so lets create our own! 
```
rivet-mkanalysis MY_FIRST_ROOT_ANALYSIS 
```

Now lets edit the analysis in our favorite editor and then you can build it like so - 

```
rivet-build RivetMY_FIRST_ROOT_ANALYSIS.so MY_FIRST_ROOT_ANALYSIS.cc $(root-config --cflags --libs)
```

### Step 6: Running Rivet with Root

```
rivet -a MY_FIRST_ROOT_ANALYSIS --pwd --ignore-beams ../JETSCAPEFIFO/build/myHepmcoutput.hepmc -o dummy.yoda
```
Lets take a look at the output root file!!

### Step 7 - get analysis from published paper with hepdata 
```
rivet-mkanalysis STAR_2006_I709170
```
then compile as usual after you fix the code from the example provided. then compile and enjoy! 

that ends the day! Looking forward to continuing next week folks! 
