#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np

import sys

time = []
non_nlp = []
insect = []
poison = []
insecticide = []
line_no = 0
with open(sys.argv[1]) as f:
#with open('pydump-percentage-af.data') as f:
  for line in f:
    if line_no == 0:
      line_no = line_no + 1
      continue
    line = [x.strip() for x in line.split(',') ]
    time.append(float(line[0]))
    non_nlp.append(float(line[1]))
    insect.append(float(line[2]))
    poison.append(float(line[3]))
    insecticide.append(float(line[4]))


plt.plot(time, insect)
plt.plot(time, poison)
plt.plot(time, insecticide)
plt.plot(time, non_nlp)
plt.legend(['Insect', 'Poison', 'Insecticide', 'Non-nlp'], loc='best') 
#plt.legend(['Insect', 'Poison', 'Insecticide'], loc='upper left') 
plt.ylabel('Percentage in AF')
plt.show('Time in AF(sec)')
plt.savefig(sys.argv[2]+"/plot.png")



