#!/bin/bash
#
# Batch word-pair counting script for English.
# Loop over all the files in 'beta-pages', sentence-split them
# and submit them for word-pair couting.
#
basedir=$1
execf=$basedir'/ss-one.sh'
echo $execf
time find ~/Desktop/sew-splitted -type f -exec $execf en {} localhost 17001 $basedir \;
