#!/usr/bin/env python

# %% 
filename = '../build/pion_minus_H.csv'

data = None
with open(filename, 'r') as f:
    for line in f:
       data = line
       break 
# %%
items = data.split()
# %%
print(data)
len(items), len(items)/5
# %%
n_particles = len(items) // 5
E_tot = sum([float(items[idx*5 + 1]) for idx in range(1, n_particles)])
print(E_tot)
# %%
p = [sum([float(items[idx*5 + jdx]) for idx in range(1, n_particles)]) for jdx in range(2, 5)]
# %%
p
# %%
