#!/usr/bin/env python
"""This script prepares a submission folder to submit jobs via TaskFarmer

"""
import os
import subprocess

coresMatch = {
    "haswell": 32,
    "knl": 68,
}
def process(script_name, py8, card, outdir, nevts, nodes,
    queue, account, job_time, dry_run):


    ncores = coresMatch[queue]
    batch_cfg = [
        '#!/bin/bash',
        '#SBATCH -C {}'.format(queue),
        '#SBATCH --time={}'.format(job_time),
        '#SBATCH --nodes={}'.format(nodes),
        '#SBATCH --ntasks-per-node={}'.format(ncores),
        '#SBATCH --cpus-per-task=2',
        '#SBATCH --image=docexoty/heptools:ubuntu20.04',
        '#SBATCH --volume="/dev/infiniband:/sys/class/infiniband_verbs"',
        "", "",
        '################# DON NOT CHANGE THINGS HERE UNLESS YOU KNOW WHAT YOU ARE DOING ###############',
        '# This script is a modification to the implementation suggest by gregSchwartz18 here:',
        '# https://github.com/ray-project/ray/issues/826#issuecomment-522116599',
        'redis_password=$(uuidgen)',
        'export redis_password',
        'nodes=$(scontrol show hostnames $SLURM_JOB_NODELIST) # Getting the node names',
        'nodes_array=( $nodes )', "",
        'node_1=${nodes_array[0]}',
        'ip=$(srun --nodes=1 --ntasks=1 -w $node_1 hostname --ip-address) # making redis-address',
        'port=6379',
        'ip_head=$ip:$port',
        'export ip_head',
        'echo "IP Head: $ip_head"',
        'echo "STARTING HEAD at $node_1"',
        'srun --nodes=1 --ntasks=1 -w $node_1 start-head.sh $ip $redis_password &',
        'sleep 30',
        'worker_num=$(($SLURM_JOB_NUM_NODES - 1)) #number of nodes other than the head node',
        'for ((  i=1; i<=$worker_num; i++ ))',
        'do',
        '  node_i=${nodes_array[$i]}',
        '  echo "STARTING WORKER $i at $node_i"',
        '  srun --nodes=1 --ntasks=1 -w $node_i start-worker.sh $ip_head $redis_password &',
        '  sleep 5',
        'done',
        '##############################################################################################',
        "", "",
        'shifter python {}'.format(script_name),
        'exit',
    ]
    if dry_run:
        print('\n'.join(batch_cfg))
    else:
        output = os.path.join(outdir, "submit.sh")
        with open(output, 'w') as f:
            f.write("\n".join(batch_cfg))
        

        subprocess.call(['chmod', "+x", output])
        outname = os.path.join(outdir, "delphe_ntuple")
        subprocess.call(['sbatch', output, card, py8, outname])
    # base_cmds = [
    #     'shifter',
    #     '--image=docexoty/heptools:ubuntu20.04',
    #     'DelphesPythia8'
    # ]


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Job defintion')
    add_arg = parser.add_argument
    add_arg("script_name", help='an exectutable script')
    add_arg('-p', '--py8', help='pythia8 configuration', required=True)
    add_arg('-c', '--card', help='Delphes card', required=True)
    add_arg('-o', '--output', help='output filename', required=True)
    add_arg('-e', '--nevts', help='number of events', required=True, type=int)
    add_arg('-n', '--nodes', help='number of nodes', type=int, required=True)
    add_arg('-q', '--queue', default='haswell', choices=list(coresMatch.keys()))
    add_arg('-a', '--account', help='account to charge', required=True)
    add_arg('-t', '--job-time', help='job time', default='04:00:00')
    add_arg('-d', '--dry-run', help='dry run', action='store_true')
    args = parser.parse_args()
    
    process(**vars(args))