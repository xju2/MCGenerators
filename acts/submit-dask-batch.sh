#!/bin/bash

#SBATCH -N 2
#SBATCH -n 64
#SBATCH -t 04:00:00
#SBATCH -C cpu
#SBATCH -A m3443
#SBATCH --image=docexoty/acts-triton
#SBATCH --mail-type=start,end,fail
#SBATCH -q regular


echo "Starting scheduler..."

IMAGE_NAME="docexoty/acts-triton"

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
    --nworkers 1  &

# Wait for the workers to start
sleep 20

echo "workers started"

#launch dask script using workers
shifter python run_acts_dask.py 


echo "Killing scheduler"
kill -9 $dask_pid

echo "Done"
