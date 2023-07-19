# JETSCAPE w/ RIVET install 

### Step 0: Create  New Docker Container
 Create a new container using a JETSCAPE image that has RIVET dependencies per-installed.
 #### macOS
```
cd ~
mkdir jetscape-rivet-docker
cd jetscape-rivet-docker
docker run -it -v ~/jetscape-rivet-docker:/home/jetscape-rivet-user --name
    myJetscapeRivet -p 8888:8888 tmengel/jetscaperivet:latest
```

 #### linux
```
docker run -it -v ~/jetscape-docker:/home/jetscape-rivet-user --name
    myJetscapeRivet -p 8888:8888 --user $(id -u):$(id -g)
    tmengel/jetscaperivet:latest
```

For **Windows**, please follow the analogous instructions: https://docs.docker.com/install/

Please note that if you have an older OS, you may need to download an older version of docker.]

### Step 0 - a THIS IS VERY IMPORTANT! 
Make sure that you are inside the correct directory in the docker.  
```
cd /home/jetscape-rivet-user
```

### Step 1: Clone JETSCAPEFIFO
Now clone a modified version of JETSCAPE which contains the necessary HepMC3 Fifo writing modules. The only changes are an addition module for HepMC3-fifo output and a directory called **JetScapeWriterHepMCfifo** with an example XML file to run the new module.

```
 git clone https://github.com/tmengel/JETSCAPEFIFO.git
 ```

### Step 2: Bootstrap RIVET
Within the docker container you should see **I have no name!@...:~**. This shows that you are in the container. **Make sure you see this before continuing**.

#### linux/windows
```
 cd JETSCAPEFIFO/external_packages
./get_rivet.sh
```
#### mac
```
 cd JETSCAPEFIFO/external_packages
./get_rivet.sh mac
```
This will bootstrap rivet in a new directory `/home/jetscape-rivet-user/JETSCAPEFIFO/RIVET`. This process will take about 15-20 minutes.

### Step 2 - a Source the rivet install
```
source /home/jetscape-rivet-user/JETSCAPEFIFO/RIVE/local/rivetenv.sh 
```

### Step 3: Build JETSCAPE
Proceed to build this version of JETSCAPE the same way you would for the original version [Installation Instructions](https://github.com/JETSCAPE/JETSCAPE/wiki/Doc.Installation).

```
cd /home/jetscape-rivet-user/JETSCAPEFIFO
mkdir build
cd build
cmake .. -DCMAKE_CXX_STANDARD=14
make -j4
```

