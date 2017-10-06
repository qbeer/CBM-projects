#!/usr/bin/python3

import sys
import os

for i in range(0,int(sys.argv[1])):
    os.system('./mst data '+str(sys.argv[2])+ '>> ido_'+str(sys.argv[2])+'.dat')

print('Succes.')    