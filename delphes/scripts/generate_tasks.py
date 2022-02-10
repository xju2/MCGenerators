"""This will repeat the same job for N times
"""
njobs = 20
basedir = "/home/xju/ocean/projects/testarea"
config_files = ['MinBias.pileup', 'delphes_card_ATLAS_PileUp.tcl', 'qcd.cmnd']
delphe = "DelphesPythia8 delphes_card_ATLAS_PileUp.tcl qcd.cmnd {outname}"
ana_exe = "AnaDelphes"
analysis = f"{ana_exe}" + " -f {outname} -o processed_{outname}"
out_task_fname = "tasks_qcd.txt"

cmd_list = ["ln -s {}/{}".format(basedir, x) for x in config_files]
cmd_list += [delphe, analysis]

command = " && ".join(cmd_list)



outname_base = "Ntuple_qcd_PU_{:05}.root"


jobs = []
for ijob in range(njobs):
    outname = outname_base.format(ijob)
    jobs.append(command.format(outname=outname))

with open(out_task_fname, 'w') as f:
    f.write('\n'.join(jobs))
    f.write('\n')
