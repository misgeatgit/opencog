#!/bin/sh
logf="exp1_poison.aflog"

extract="nodes.txt"
final="words_observe_at_cycle.txt"
dir=$(date +"%T")
dumpdir="trials"

mkdir $dir
mv *.json $dir
mv $logf $dir
mv $extract $dir
mv $final $dir
mv $dir $dumpdir

