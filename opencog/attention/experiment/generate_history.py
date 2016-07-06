#!/usr/bin/python

import csv

# rent.data format
#uuid,sti,charged,af_boundary,time

# reward.data forma
# uuid,sti,rewarded,af_boundary,time

# diffusion.data
#src_uuid,src_sti,trgt_uuid,trgt_sti,diff_amount,af_sti,time

atom_sti = {} # {uuid: [sti,diffusion,time]}

f = open("reward.data", "r")
lines = csv.reader(f, delimiter=',')
next(lines) #delete header

for line in lines:
  uuid = line[0]
  atom_sti[uuid] = [[line[1], line[2], line[4], "Reward"]]

print atom_sti

f = open("rent.data", "r")
lines = csv.reader(f, delimiter=',')
next(lines) #delete header

for line in lines:
  uuid = line[0]
  if uuid in atom_sti.keys():
     atom_sti[uuid].append([line[1], -1*int(line[2]), line[4],"Rent"])
  #else:
  #   atom_sti[uuid]=[[line[1], -1*int(line[2]), line[4]]]

f = open("diffusion.data", "r")
lines = csv.reader(f, delimiter=',')
next(lines) #delete header

for line in lines:
  uuid = line[0]
  if uuid in atom_sti.keys():
    atom_sti[uuid].append([line[1], -1*int(line[4]), line[6], "Diffusion"])
  #else:
  #  atom_sti[uuid]=[[line[1], -1*int(line[4]), line[6]]]

for uuid in atom_sti.keys():
  print "UUID:"+str(uuid)+"\nSTI  DIFF  TIME\n"
  data = atom_sti[uuid]
  for row in data:
    print str(row[0])+"  "+str(row[1])+"  "+str(row[2])+"  "+row[3]+"\n"







