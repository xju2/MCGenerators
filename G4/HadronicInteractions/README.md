# Instructions

```
shifter --image=docexoty/heptools:ubuntu20.04-CPU bash
```

Then compile the code
```
make build && cd build && cmake .. -DCMAKE_CXX_COMPILER=`which g++` -DCMAKE_C_COMPILER=`which gcc` && make 
```

then generate events
```
./mymain -n 10 -o test.csv
```