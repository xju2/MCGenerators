#!/bin/bash

if [ $# -lt 3 ]; then
	echo "Usage: $0 aCTS-bin-dir output-dir py8-cmnd"
	exit
fi

DIGICONF=default-smearing-config-generic.json
if [ ! -f $DIGICONF ];then
	echo "digitization configuration, itk-pixel-digitization.json, does not exist"
	exit
fi

ACTSBINDIR=$1 # acts bin directory
OUTDIR=$2	  # output directory
PY8CMD=$3
SKIPEVTS=2500

if [ ! -d $OUTDIR ];then
	mkdir -p $OUTDIR
fi


NEVTS=5000
NPU=0
CMS=13000
NWORKERS=1
BFIELD="--bf-constant-tesla 0:0:2"

# pythia configuration
PY8OPTIONS=""
echo "Pythia8 Options"
while IFS= read -r line
do
	if [ -z "$line" ] || [ ${line:0:1} == "#" ]; then
		continue
	fi
	ARG=${line%%!*}
	echo $ARG
	PY8OPTIONS="$PY8OPTIONS --gen-hard-process \"$ARG\""
done < $PY8CMD

#echo $PY8OPTIONS
# generate events
function gen() {
	$ACTSBINDIR/ActsExamplePythia8 --gen-npileup $NPU --gen-cms-energy-gev $CMS \
		--gen-vertex-xy-std-mm 2 --gen-vertex-z-std-mm 150 \
		"$PY8OPTIONS" --output-dir $OUTDIR --output-csv -j $NWORKERS -n $NEVTS
}


# simulation
function sim() {
	$ACTSBINDIR/ActsExampleFatrasGeneric --input-dir $OUTDIR \
		--output-dir $OUTDIR $BFIELD --output-csv -j $NWORKERS
}

# digitization
function digi() {
	$ACTSBINDIR/ActsExampleDigitizationGeneric $BFIELD --output-dir $OUTDIR \
		--output-csv --input-dir $OUTDIR \
		--digi-config-file $DIGICONF -j $NWORKERS
}

# measurements to spacepoints
function meas2sp() {
	$ACTSBINDIR/ActsExampleMeasurements2SPGeneric $BFIELD --output-dir $OUTDIR \
		--output-csv --input-dir $OUTDIR -j $NWORKERS
}

digi
meas2sp
