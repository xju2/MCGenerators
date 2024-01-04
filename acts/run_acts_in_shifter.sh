#!/bin/bash

if [ $# -lt 3 ]; then
	echo $0 numEvts seed outDir
fi

export ACTS_PATH=/pscratch/sd/x/xju/LLMTracking/acts
export ACTS_BUILD_PATH=$ACTS_PATH/build
export DD4HEP_PATH=/usr/local/
export DD4hepINSTALL=/usr/local
source $ACTS_BUILD_PATH/python/setup.sh
source $DD4HEP_PATH/bin/thisdd4hep_only.sh
export LD_LIBRARY_PATH=$ACTS_BUILD_PATH/thirdparty/OpenDataDetector/factory:$LD_LIBRARY_PATH

/pscratch/sd/x/xju/LLMTracking/MCGenerators/acts/full_chain_odd.py --events $1 --seed $2 --outdir $3 -t 2

echo DONE
