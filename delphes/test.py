#!/usr/bin/env python

import sys
filename = 'test.parquet'

if len(sys.argv) > 1:
    filename = sys.argv[1]

import pyarrow.parquet as pq
table = pq.read_table(filename)

df = table.to_pandas()
print(df)

def test_uproot():
    import uproot
    filename = 'test.root'
    with uproot.open(filename) as h:
        print(h)
        help(h)


test_uproot()
