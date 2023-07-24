# RIVET/JETSCAPE TUTORIAL Part 2

### Step 0 a: Get Rivet Docker image

1.) Create a **separate** terminal window. In that window, run:
```
docker pull hepstore/rivet
```
Even if you've pulled it before, try pulling to make sure it is up-to-date.

Now, we can run another docker container and mount it onto the same directory we ran Jetscape in previously. Let's go ahead and change to that directory.
```
cd ~/jetscape-rivet-docker
```

## Now, let's run a container:
```
docker run -it -v --rm ~/jetscape-rivet-docker:/home/jetscape-rivet-user --name myRivet hepstore/rivet
```
## A brief explanation of each option in the command:

* -it creates an interactive terminal for the container
* -v "mounts" your container onto a directory on your local computer. This will save you from having to copy files back and forth between your local computer and your container.
* --rm Will remove the container when you exit your container. This is not a big deal though since your progress will be saved through your mounted directory.
* --name names your container. If you don't give your container a name, docker will give it a random and perhaps fun name :). You can check out the names of all of your containers by using the
```docker container ls``` command

* The last argument is the docker image: ```hepstore/rivet```! <br>
For more, run ```docker run --help```

There are a lot more!

## More options for those interested:

* -p XXXX:XXXX where X = a digit. This will use a specified port to run the container. **WARNING**: Since we already ran -p 8888:8888 for the jetscaperivet container, you will get an error if you try running another container on that port.


To know that you have successfully opened the container, you should see something like:
```root@24703fed6010:/work#```

3.) To run rivet-mkhtml, we want to be in the same directory as the **.yoda** files that we want to visualize:
```
cd /home/jetscape-rivet-user
```
Let's compare the pp200.yoda file that we created last time to the sample analysis in the tutorial directory (STAR_2006_I709170). First, we need to make sure all .yoda files are in the same directory

```
mv rivet_analyses/pp200.yoda tutorial/pp200.yoda
```
**Before moving on,** let's make sure this move was successful!

```
cd tutorial && ls *.yoda
```

Output should be: ```STAR_2006_I709170.yoda  pp200.yoda```

 4.) The moment we have all been waiting for... <br>
 :tada: let's visualize our pp200 analysis from last session: :tada:
 ```
 rivet-mkhtml --pwd pp200.yoda
 ```
 ## More rivet-mkhtml options for those interested:

* -errs plots error bars from your monte carlo data
* -o specifies output folder <br>
For more, run ```rivet-mkhtml -h```


### Step 0 b: Get analyses to compare
To start the docker
```
docker run -it -v ~/jetscape-rivet-docker:/home/jetscape-rivet-user --name myJetscapeRivet --rm -p 8888:8888 tmengel/jetscaperivet:latest
```

Now, inside the docker container,
```
cd /home/jetscape-rivet-user/tutorial
git pull
cd /home/jetscape-rivet-user/rivet_analyses/
mkdir heavyion
cp ../tutorial/heavyion_tutorial/calibration_ALICE_PbPb2760GeV.yoda heavyion
```

### Step 0 c: Download necessary code packages

We need to download the following packages (not sure if we need all...)
```
cd /home/jetscape-rivet-user/JETSCAPEFIFO/external_packages
./get_freestream-milne.sh
./get_music.sh
./get_iSS.sh
./get_lbtTab.sh
```
### Step 0 d: Build JETSCAPE with MUSIC and iSS

```
cd ../
rm -r build
mkdir build
cd build
cmake .. -DUSE_MUSIC=ON -DUSE_ISS=ON -DUSE_FREESTREAM=ON
make -j4
```

### Step 0 d: Use Hydro Sample

```
cd ../examples
./get_hydroSample_PbPb2760_cen_00-05.sh
```

### Step 3: Source Rivet and Make Analysis
Let's make sure to source Rivet and get analyses template files.
```
source /home/jetscape-rivet-user/RIVET/local/rivetenv.sh

cd /home/jetscape-rivet-user/rivet_analyses/heavyion
rivet-mkanaylsis heavy_ion_analysis
```

### Step 1: Edit JETSCAPE file

Using your preferred text editor, modify JETSCAPEFIFO/config/jetscape_user_pbpb-grid.xml

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

### Step 2: Compile and run JETSCAPE
Inside the docker container, compile the Rivet analysis. then lets run jetscape to get our output files!

For pbpb:
```
cd /home/jetscape-rivet-user/JETSCAPEFIFO/build
./runJetscape ../config/jetscape_user_pbpb-grid.xml
```
For pp:
```
cd /home/jetscape-rivet-user/JETSCAPEFIFO/build
./runJetscape ../config/jetscape_user_PP19.xml
```

this should give you an output hepmc file!

### Step 2 a : INCASE you dont have jetscape running in the docker yet - please download a sample hepmc file from here -
https://drive.google.com/file/d/11ULqu_4qISVTcNnEInUBxIbFcyll2r3z/view?usp=sharing

### Step 3: Run Rivet
Need to
```
cd /home/jetscape-rivet-user/rivet_analyses/heavyion
rivet-build RivetHEAVY_ION_ANALYSIS.so HEAVY_ION_ANALYSIS.cc
rivet --pwd -p calibration_ALICE_PbPb2760GeV.yoda -a HEAVY_ION_ANALYSIS:cent=GEN --ignore-beams -o PbPb2760.yoda /home/jetscape-rivet-user/JETSCAPEFIFO/build/PbPb2760.hepmc
```
this should run and give you a bunch of output! lets take a look at the yoda file :)

```
ls -alrth
less PbPb2760.yoda
```

### Step 4: Rivet-Make html (Joesph update)


```
docker run -it --rm hepstore/rivet

```

```
rivet-mkhtml PbPb2760.yoda

```