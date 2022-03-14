This provides an utility package to dumpy physics objects from 
[Delphes](https://github.com/delphes/delphes) into a ROOT file 
for further studies. 

# Instructions
## Use Ray Taskfarmer

```bash
python scripts/generate_tasks.py qcd.cmnd tasks_qcd_shifter.txt --njobs 50 --shifter
```

Request an interactive computing node
```bash
salloc -N 2 -q interactive -C haswell -A m3443 -t 04:00:00 \
    --image=docexoty/heptools:ubuntu20.04-CPU
```

In the interactive node, setup the ray cluster, `source start-ray-cluster`, 
then execute the job
```bash
raytaskfarmer.py -i tasks_qcd_shifter.txt -o ntuple_qcd
```

<!-- Parquet file format, [https://arrow.apache.org/docs/cpp/parquet.html](https://arrow.apache.org/docs/cpp/parquet.html). -->