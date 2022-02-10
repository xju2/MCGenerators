"""This will repeat the same job for N times
"""
import os

def generate(njobs: int, py8cmnd: str, task_fname: str, shifter: bool):
    # basedir = "/global/homes/x/xju/m3443/usr/xju/TauStudies/run_QCDPU"
    basedir = os.path.abspath(os.getcwd())
    config_files = ['MinBias.pileup', 'delphes_card_ATLAS_PileUp.tcl', py8cmnd]
    delphe = "DelphesPythia8 delphes_card_ATLAS_PileUp.tcl "+py8cmnd+" {outname}"
    ana_exe = "/global/cfs/cdirs/atlas/xju/software/MCGenerators/delphes/build_heptools/bin/AnaDelphes"
    analysis = f"{ana_exe}" + " -f {outname} -o processed_{outname}"

    # check the existance of input files
    config_files = [os.path.join(basedir, x) for x in config_files]
    for x in config_files:
        if not os.path.exists(x):
            raise RuntimeError("config file", x, "is missing!")
    
    # add shifter in case of a container
    if shifter:
        delphe = "shifter " + delphe
        analysis = "shifter " + analysis

    cmd_list = ["ln -s {}".format(x) for x in config_files]
    cmd_list += [delphe, analysis]

    command = " && ".join(cmd_list)

    outname_base = "delphes_{:05}.root"

    jobs = []
    for ijob in range(njobs):
        outname = outname_base.format(ijob)
        jobs.append(command.format(outname=outname))

    with open(task_fname, 'w') as f:
        f.write('\n'.join(jobs))
        f.write('\n')

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='generate tasks for Delphes')
    add_arg = parser.add_argument
    add_arg('py8cmnd', help='Py8 command lines')
    add_arg('task_fname', help='task output name')
    add_arg('--shifter', action='store_true', help='using shifter container')
    add_arg('-n', '--njobs', help='Number of jobs', default=100, type=int)
    args = parser.parse_args()
    generate(**vars(args))
