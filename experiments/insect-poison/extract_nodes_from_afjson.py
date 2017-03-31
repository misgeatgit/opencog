#!/usr/bin/python
import json
import sys
from pprint import pprint

if len(sys.argv) != 2:
  print "Wrong number of params.\n"
  exit()
fl = sys.argv[1];

with open(fl) as jsonf:
  jsond = json.load(jsonf)
  #pprint(jsond)
  atoms = jsond["result"]["atoms"] 
  #print atoms
  for atom in atoms:
    print atom["name"]


