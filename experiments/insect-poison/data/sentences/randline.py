import random, sys
lines = open(sys.argv[1]).readlines()
print(lines[random.randrange(len(lines))])