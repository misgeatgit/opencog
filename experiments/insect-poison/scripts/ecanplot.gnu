#!/usr/bin/gnuplot

plot  "ecan.csv" using 1 title 'RULE_1' with lines,\
          "ecan.csv" using 2 title 'RULE_2' with lines,\
          "ecan.csv" using 3 title 'RULE_3' with lines,\
          "ecan.csv" using 4 title 'RULE_4' with lines,\
          "ecan.csv" using 5 title 'RULE_5' with lines;

# provide image params for saving it to disk.
#set terminal png size 800,600 enhanced font "Helvetica,8"

# Let's save snapshots for later analysis.
#do for [t=0:240]{
#    set output 'rulesn-'.t.'s.png'
#        pause 1 
#        replot
#}
pause 1
reread
