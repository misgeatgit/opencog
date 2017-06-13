#!/usr/bin/python
import sys

if len(sys.argv) != 2:
  print "Wrong number of params.\n"
  exit()

fl = sys.argv[1];
print fl
word_freq = {}
word_cycles= {}
cycle = 0

with open(fl, 'r') as words:
  read = False
  for word in words:
    if word[:6] == "cycle_" :
      cycle = cycle + 1
      continue
    if len(word) != 0:
      if word in word_cycles:
        word_cycles[word].append(cycle)
      else:
        word_cycles[word] = [cycle]

for word, cycles in word_cycles.iteritems():
  print "Word: "+ word.rstrip('\n')
  print "Cycles: " + "".join(str(s)+" " for s in cycles)+"\n"

'''
word_graph ={}
for word in word_cycles:
  for ccl in range(1,cycle+1):
    bar = ""
    if ccl in word_cycles[word]:
      bar += "*"
    else:
      bar += " "
    word_graph[word] = bar

#for word, bar in word_graph.iteritems():
#  print word+":"+bar
'''


       

