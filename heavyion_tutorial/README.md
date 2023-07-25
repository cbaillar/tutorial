# RIVET/JETSCAPE TUTORIAL Part 2

### Step 0 a: Get Rivet Docker image

1.) Create a **separate** terminal window. In that window, run:
```bash
docker run -it -v ~/jetscape-rivet-docker:/work/ --name myRivet --rm -p 8888:8888 hepstore/rivet:3.1.8
```
Even if you've pulled it before, try pulling to make sure it is up-to-date.


To know that you have successfully opened the container, you should see something like:
```ruby
root@24703fed6010:/work#
```
3.) To run rivet-mkhtml, we want to be in the same directory as the **.yoda** files that we want to visualize:
```
cd /work/rivet_analyses
```
Let's compare the pp200.yoda file that we created last time to the sample analysis in the tutorial directory (STAR_2006_I709170) to the data!  

Make sure the STAR_2006_I709170 analysis is in the directory. If not, you can copy it over
```
cp ../tutorial/STAR_2006_I709170.* . 
```
Now, we need to recompile the analysis -
```
rm RivetSTAR_2006_I709170.so 
rivet-build RivetSTAR_2006_I709170.so STAR_2006_I709170.cc
rivet --pwd -a STAR_2006_I709170 --ignore-beams -o pp200.yoda /home/jetscape-rivet-user/JETSCAPEFIFO/build/myHepmcoutput.hepmc
```
 
 4.) The moment we have all been waiting for... <br>
 :tada: let's visualize our pp200 analysis from last session: :tada:
 ```bash
 rivet-mkhtml --pwd pp200.yoda
 ```
 <details>
<summary>More rivet-mkhtml options for those interested:</summary>
  
* -errs plots error bars from your monte carlo data
* -o specifies output folder <br>
For more, run 
```bash
rivet-mkhtml -h
```
</details>

### Step 0 b: Get analyses to compare
Open another terminal and run the myJetscapeRivet container.
```
docker run -it -v ~/jetscape-rivet-docker:/home/jetscape-rivet-user --name myJetscapeRivet --rm -p 8888:8888 tmengel/jetscaperivet:latest
```

This will basically start our heavy ion analysis! 

Now, we will put our calibration file in a new working directory.
```
cd /home/jetscape-rivet-user/tutorial
git pull
cd /home/jetscape-rivet-user/rivet_analyses/
mkdir heavyion
cp ../tutorial/heavyion_tutorial/calibration_ALICE_PbPb2760GeV.yoda heavyion
```

### Step 0 c: Build JETSCAPE with LBT-tables, MUSIC, and iSS

We will download the following packages 
```
cd /home/jetscape-rivet-user/JETSCAPEFIFO/external_packages
./get_lbtTab.sh
./get_music.sh
./get_iSS.sh
```
then, rebuild JETSCAPE

```
cd ../
rm -r build
mkdir build
cd build
cmake .. -DUSE_MUSIC=ON -DUSE_ISS=ON
make -j4
```

### Step 0 d: Use Hydro Sample
Let's use a hydro sample from the examples for this tutorial.
```
cd ../examples
./get_hydroSample_PbPb2760_cen_00-05.sh
```

### Step 1: Source Rivet and Make Analysis
Make sure to source Rivet and get analyses template files.

```
source /home/jetscape-rivet-user/RIVET/local/rivetenv.sh

cd /home/jetscape-rivet-user/rivet_analyses/heavyion
rivet-mkanalysis HEAVY_ION_ANALYSIS
```

### Step 2: Edit JETSCAPE file

Using your preferred text editor, modify jetscape_user_pbpb-grid.xml

Change the following values in the JETSCAPEFIFO/config/jetscape_user_pbpb-grid.xml file
```
<nEvents> 1000 </nEvents>
<setReuseHydro> true </setReuseHydro>
<nReuseHydro> 1000 </nReuseHydro>

<outputFilename>PbPb2760</outputFilename>
<JetScapeWriterHepMC> on </JetScapeWriterHepMC>

<vlevel> 0 </vlevel>
<nEvents_printout> 100 </nEvents_printout>
```
```
<!-- Inital State Module  -->
<IS>
   <initial_profile_path>../examples/test_hydro_files</initial_profile_path>
</IS>
```

```
<!-- Hard Process -->
<Hard>
  <PythiaGun>
    <FSR_on>0</FSR_on>
    <pTHatMin>5</pTHatMin>
    <pTHatMax>1000</pTHatMax>
    <eCM>2760</eCM>
    <LinesToRead>
      HardQCD:all = on
      PDF:useHardNPDFA=on
      PDF:useHardNPDFB=on
      PDF:nPDFSetA=1
      PDF:nPDFSetB=1
      PDF:nPDFBeamA = 100822080
      PDF:nPDFBeamB = 100822080
    </LinesToRead>
    <useHybridHad>0</useHybridHad>
  </PythiaGun>
</Hard>
```

```
<!-- Hydro  Module  -->
<Hydro>
  <hydro_from_file>
    <name>Hydro from file </name>
    <read_in_multiple_hydro>1</read_in_multiple_hydro>
    <hydro_files_folder>../examples/test_hydro_files</hydro_files_folder>
  </hydro_from_file>
</Hydro>
```

### Step 3: Run JETSCAPE
Run JETSCAPE to get our output files. 

For pbpb:
```
cd /home/jetscape-rivet-user/JETSCAPEFIFO/build
./runJetscape ../config/jetscape_user_pbpb_grid.xml
```
This will take some time, so let's start on our Rivet analysis.

### Step 3 a : INCASE you dont have JETSCAPE running in the docker yet - please download a sample hepmc file from here -
https://drive.google.com/file/d/1hJ0gtYOJ-JrjXcCpoDtRGESiJB8p8pZW/view?usp=sharing

### Step 4: Edit files for Rivet Analysis
Using your preferred text editor, modify HEAVY_ION_ANALYSIS.cc (/home/jetscape-rivet-user/rivet_analyses/heavyion/HEAVY_ION_ANALYSIS.cc)


### Step 5: Run Rivet
Go back to the myJetscapeRivet container. Now, we want to run our analysis!
```
cd /home/jetscape-rivet-user/rivet_analyses/heavyion
rivet-build RivetHEAVY_ION_ANALYSIS.so HEAVY_ION_ANALYSIS.cc
rivet --pwd -p calibration_ALICE_PbPb2760GeV.yoda -a HEAVY_ION_ANALYSIS:cent=GEN --ignore-beams -o PbPb2760.yoda /home/jetscape-rivet-user/JETSCAPEFIFO/build/PbPb2760.hepmc
```
This should run and give you a bunch of output! Let's take a look at the yoda file :)

```
ls -alrth
less PbPb2760.yoda
```

