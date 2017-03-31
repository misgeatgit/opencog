#/usr/bin/python
import sys

if len(sys.argv) != 2:
  print "Wrong number of params.\n"
  exit()

fl = sys.argv[1];
print fl
with open(fl, 'r') as aflog:
  read = False
  end_reading = False
  buff = ""
  cycle = 0
  for line in aflog:
    if line[:3] == "AFB" :
        read = True
        end_reading = False
        cycle = cycle + 1
        continue
    elif line[:5] == "[time":
      end_reading = True

    if read:
      if end_reading:
        with open("cycle_"+str(cycle)+".json", 'w') as f:
          f.write(buff)
        buff = ""
        read = False
        continue
      else:
        buff = buff + line;

