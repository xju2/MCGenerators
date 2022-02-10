#!/usr/bin/env python
"""This script prepares a submission folder to submit jobs via TaskFarmer

"""

def process(py8, card, output, nevts, nodes, queue, account):
    base_cmds = [
        'shifter',
        '--image=docexoty/heptools:ubuntu20.04',
        'DelphesPythia8'
    ]


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Job defintion')
    add_arg = parser.add_argument
    add_arg('-p', '--py8', help='pythia8 configuration', required=True)
    add_arg('-c', '--card', help='Delphes card', required=True)
    add_arg('-o', '--output', help='output filename', required=True)
    add_arg('-e', '--nevts', help='number of events', required=True, type=int)
    add_arg('-n', '--nodes', help='number of nodes', type=int)
    add_arg('-q', '--queue', default='haswell', choices=['haswell', 'knl'])
    add_arg('-a', '--account', help='account to charge', required=True)
    args = parser.parse_args()
    
    if args.nodes < 2:
        process(**vars(args))