# Introduction
This is to provide a simple interface to run [Pythia8.302](http://home.thep.lu.se/~torbjorn/pythia83html/Welcome.html) generator and write out HepMC data. One can use `Rivet` to further analyze the HepMC data.

We will compile the code with the docker image: `docexoty/heptools:rivet3.1.7`. In Cori, one can do the following
```bash
shifter --image=docker:docexoty/heptools:rivet3.1.7 bash
```
In other system, one can do the following:
```bash
docker run -it --rm -v $PWD:$PWD -w $PWD docexoty/heptools:rivet3.1.7 bash
```

Then to compile the code
```bash
mkdir build && cd build 

cmake .. && make 
```

To run the program:
```bash
./bin/pythia ../cmnds/zee.cmnd test.hepmc
```