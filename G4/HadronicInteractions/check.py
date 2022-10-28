#!/usr/bin/evn python
import math
import sys
a = sys.argv[1]
items = a.split(";")
vec4 = [float(x) for x in items[0].split()]
#mass = math.sqrt(vec4[1]**2 - sum([vec4[i]**2 for i in range(2, 5)]))
#print(mass)


kine_names = ['E', 'px', 'py', 'pz']
for idx,name in enumerate(kine_names):
    kine = [float(x.split()[idx+1]) for x in items]
    print(name, kine[0], sum(kine[1:]))
