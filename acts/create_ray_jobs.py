#!/usr/bin/env python

def create_jobs(njobs, job_fname, events_per_job):
    # Create jobs here
    with open(job_fname, 'w') as f:
        for ijob in range(njobs):
            seed = ijob + 1234
            #cmd = f"shifter ./full_chain_odd.py --events 1000 --seed {seed} --outdir v5 -t 2"
            cmd = f" /pscratch/sd/x/xju/LLMTracking/MCGenerators/acts/run_acts_in_shifter.sh {events_per_job} {seed} testarea"
            f.write(cmd + '\n')

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--njobs', type=int, default=10)
    parser.add_argument('-o', '--job_fname', type=str, default='jobs.txt')
    parser.add_argument('-e', '--events-per-job', type=int, default=1000)
    args = parser.parse_args()

    create_jobs(args.njobs, args.job_fname, args.events_per_job)

