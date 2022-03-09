#!/usr/bin/env python

# %%
import argparse
import numpy as np

def process_file(file_path, ouput_path):
    data = None
    with open(file_path, 'r') as f:
        for line in f.readlines():
            new_data = np.array(line.split()).reshape((-1, 5)).astype(np.float16)
            if not new_data.shape[0]>2: continue
            new_data = new_data[:3,:]
            new_data = new_data.reshape((1,-1))
            if data is None: 
                data=new_data
            else:
                data = np.append(data, new_data, axis=0)
    
    with open(ouput_path, 'w') as f:
        np.savetxt(f, data)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--file')
    parser.add_argument('-o', '--output')
    args = parser.parse_args()
    process_file(args.file, args.output)

if __name__=='__main__':
    main()
# %%
