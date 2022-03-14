This provides an utility package to dumpy physics objects from 
[Delphes](https://github.com/delphes/delphes) into a ROOT file 
for further studies. 

# Instructions
## Use Ray Taskfarmer

Install ray in a conda environment called `ray`. 

```
source ~/miniconda3/bin/activate ray
```

Then create a list of tasks
```bash
python scripts/generate_tasks.py qcd.cmnd tasks_qcd_shifter.txt --njobs 50 --shifter --exclude-ana
```

Look at the produced output file `tasks_qcd_shifter.txt` to check 
if the command lines make sense.


Request an interactive computing node
```bash
salloc -N 2 -q interactive -C haswell -A m3443 -t 04:00:00 \
    --image=docexoty/heptools:ubuntu20.04-CPU
```

In the interactive node, setup the ray cluster
```bash
source start-ray-cluster
```
then execute the job
```bash
raytaskfarmer.py -i tasks_qcd_shifter.txt -o ntuple_qcd
```

<!-- Parquet file format, [https://arrow.apache.org/docs/cpp/parquet.html](https://arrow.apache.org/docs/cpp/parquet.html). -->