#!/bin/bash

if [ $# -lt 7 ]; then
	echo "Usage: $0 ACTION ACTS-BIN-DIR OUTPUT-DIR PY8-CMND NEVTS"
	echo "ACTION:       ['gen', 'sim', 'digi', 'meas2sp']"
	echo "ACTS-BIN-DIR: bin-directory created when compiling ACTS"
	echo "OUTPUT-DIR:   output directory"
	echo "PY8-CMD:      py8 commands in a text file"
	echo "NEVTS:        number of events"
	echo "NPU:          number of pileup <mu>"
	echo "SEED:         generator seed"
	exit
fi

#DIGICONF=default-smearing-config-generic.json
DIGICONF=default-geometric-config-generic.json

#DIGICONF=default-input-config-generic.json ## do not use this

if [ ! -f $DIGICONF ];then
	echo "digitization configuration, itk-pixel-digitization.json, does not exist"
	exit
fi

ACTION=$1
ACTSBINDIR=$2 # acts bin directory
OUTDIR=$3	  # output directory
PY8CMD=$4
NEVTS=$5
NPU=$6
SEED=$7

#SHIFTER="shifter --image=docexoty/heptools:ubuntu20.04"
#SHIFTER="docker run --rm -v $PWD:$PWD -w $PWD docexoty/exatrkx:tf2.5-torch1.9-cuda11.2-ubuntu20.04-rapids21.10-devel-hep bash "
SHIFTER=""

if [ ! -d $OUTDIR ];then
	mkdir -p $OUTDIR
fi

CMS=13000
NWORKERS=1
BFIELD="--bf-constant-tesla 0:0:2"

PY8OPTIONS=""
function get_py8_cmd() {
	# pythia configuration
	while IFS= read -r line
	do
		if [ -z "$line" ] || [ ${line:0:1} == "#" ]; then
			continue
		fi
		ARG=${line%%!*}
		ARG=`echo "$ARG" | xargs`
		#echo $ARG
		PY8OPTIONS="$PY8OPTIONS --gen-hard-process \"$ARG\""
	done < $PY8CMD

	echo "$PY8OPTIONS"
}

# generate events
# xy-std, 0.0125 mm
# z-std,  55.5 mm
function gen() {
	get_py8_cmd 

	$SHIFTER $ACTSBINDIR/ActsExamplePythia8 --gen-npileup $NPU --gen-cms-energy-gev $CMS \
		--gen-nhard 1 --rnd-seed $SEED  --gen-pdg-beam0 2212 --gen-pdg-beam1 2212 \
		--output-dir "$OUTDIR" --output-csv -j $NWORKERS -n $NEVTS \
		"$PY8OPTIONS"
}


# simulation
function sim() {
	$SHIFTER $ACTSBINDIR/ActsExampleFatrasGeneric --input-dir "$OUTDIR" \
		--output-dir "$OUTDIR" $BFIELD --output-csv -j $NWORKERS
}

# digitization
function digi() {
	$SHIFTER $ACTSBINDIR/ActsExampleDigitizationGeneric $BFIELD --output-dir $OUTDIR \
		--output-csv --input-dir $OUTDIR \
		--digi-config-file $DIGICONF -j $NWORKERS
}

# measurements to spacepoints
function meas2sp() {
	$SHIFTER $ACTSBINDIR/ActsExampleMeasurementsToSPGeneric $BFIELD --output-dir $OUTDIR \
		--output-csv --input-dir $OUTDIR -j $NWORKERS
}

$ACTION