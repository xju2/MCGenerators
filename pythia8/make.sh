#!/bin/bash
cmake .. -DCMAKE_C_COMPILER=/usr/local/bin/gcc \
    -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ \
    -DPYTHIA8_DIR=$CONDA_PREFIX