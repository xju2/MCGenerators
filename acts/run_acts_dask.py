#!/usr/bin/env python3

import dask
from dask.distributed import Client
import os
import subprocess
from typing import List

def task(cmd: List[str]):
    out = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    return out

# dask.config.config["distributed"]["dashboard"]["link"] = "{JUPYTERHUB_SERVICE_PREFIX}proxy/{host}:{port}/status" 

scheduler_file = os.path.join(os.environ["SCRATCH"], "scheduler_file.json")
# create a client
client = Client(scheduler_file=scheduler_file)

num_jobs = 128
num_evts = 64
outdir = "dask_v5"

# cmd = ["./run_acts_in_shifter.sh", str(num_evts), outdir]
cmd = f"./run_acts_in_shifter.sh {num_evts} {outdir}"

futures = []
for i in range(num_jobs):
    seed = 756323 + i
    full_cmd = cmd + f" {seed}"
    # pp.pprint(full_cmd)
    futures.append(client.submit(task, full_cmd))

# wait for the task to finish
futures = client.gather(futures)

print("DONE")
