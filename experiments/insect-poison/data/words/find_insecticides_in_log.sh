#!/bin/bash

if [ -z "$1" ]; then
	echo "Please specify the launch_experiment.py output file."
	exit 1
fi

for w in $(cat insecticide.words);do
	grep -o $w  $1 | uniq
done

