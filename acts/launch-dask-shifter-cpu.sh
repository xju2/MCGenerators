#!/bin/bash

echo "Starting scheduler..."

if [ $# -lt 1 ]; then
    IMAGE_NAME="docexoty/acts-triton"
else
    IMAGE_NAME=$1
fi
echo "dask in image: $IMAGE_NAME"

scheduler_file=$SCRATCH/scheduler_file.json
rm -f $scheduler_file

#start scheduler
DASK_DISTRIBUTED__COMM__TIMEOUTS__CONNECT=3600s \
DASK_DISTRIBUTED__COMM__TIMEOUTS__TCP=3600s \
shifter --image=${IMAGE_NAME} dask-scheduler \
    --interface hsn0 \
    --scheduler-file $scheduler_file &

dask_pid=$!

# Wait for the scheduler to start
sleep 5
until [ -f $scheduler_file ]
do
     sleep 5
done

echo "Starting workers"

#start scheduler
DASK_DISTRIBUTED__COMM__TIMEOUTS__CONNECT=3600s \
DASK_DISTRIBUTED__COMM__TIMEOUTS__TCP=3600s \
srun shifter --image=${IMAGE_NAME} dask-worker \
    --scheduler-file $scheduler_file \
    --interface hsn0 \
    --nworkers 1 

echo "Killing scheduler"
kill -9 $dask_pid

echo "Done"
