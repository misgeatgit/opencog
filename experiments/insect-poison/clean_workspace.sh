#!/bin/sh
logf="exp1_poison.aflog"

extract="nodes.txt"
dir=$(mkdir  $(date +"%T"))
dumpdir="trials"

mv *.json $dir
mv $logf $dir
mv $extract $dir
mv $dir $dumpdir

