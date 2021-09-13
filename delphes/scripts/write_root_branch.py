#!/usr/bin/env python
"""
This script is to write code for bookkeeping branches
via csv file with the following example.
supported data-type: int, int32, int64, float, float32, float64
<branch name, data name, data-type>
JetE, br_recoJetE, vector<float>
nJets, br_nRecoJets, int
"""

data_types = {
    'int': 'I',
    "int32": 'I',
    'int64': 'L',
    'uint32': 'i',
    'float': 'F',
    'float32': 'F',
    'float64': 'D'
}

def add_branch(tname, bname, dname, dtype):
    # booking a branch
    if "vector" in dtype:
        book_branch = '{}->Branch("{}", &{});'.format(
            tname, bname, dname)
    else:
        book_branch = '{}->Branch("{}", &{}, "{}/{}");'.format(
            tname, bname, dname, bname, data_types[dtype])

    clear_branch = None
    if "vector" in dtype:
        clear_branch = "{}.clear();".format(dname)

    return book_branch, clear_branch

def print_code(arrays):
    bookings = [x[0] for x in arrays]
    clearings = [x[1] for x in arrays if x[1]]
    print("\n".join(bookings))
    print()
    print("\n".join(clearings))

def add_branches(tname, fname):
    with open(fname, 'r') as f:
        in_vars = [x[:-1].replace(" ", '').split(',') for x in f]

    results = [add_branch(tname, *x) for x in in_vars]
    print_code(results)


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='create code for TTree')
    add_arg = parser.add_argument
    add_arg('config', help='configuration file')
    add_arg("-t", '--tname', help='tree name', default='tree')
    
    args = parser.parse_args()
    add_branches(args.tname, args.config)