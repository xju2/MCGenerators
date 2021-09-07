This provides an utility package to dumpy physics objects from [Delphes](https://github.com/delphes/delphes) into a ROOT file for further studies. Using the container `docexoty/heptools:ubuntu20.04` is a good starting point.

It requires the Delphes being installed and installing direction is given to `Delphes_DIR`;
therefore, the `cmake` command looks like:
```bash
cmake .. -DDelphes_DIR=/path/to/delphes
```

Parquet file format, [https://arrow.apache.org/docs/cpp/parquet.html](https://arrow.apache.org/docs/cpp/parquet.html).