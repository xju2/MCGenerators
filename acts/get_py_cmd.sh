#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 py8.cmnd"
    exit
fi

PY8CMD=$1

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

get_py8_cmd
