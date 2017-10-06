#!/usr/bin/python3

import sys
import os
import numpy as np

xx = np.linspace(float(sys.argv[1]),float(sys.argv[2]),num=17)
n = len(xx)

for i in range(0,n):
    os.system('./n_run.py 10 '+str(xx[i]))
    os.system('mv output.dat output'+str(xx[i])+'.dat')

print('Success.\n')