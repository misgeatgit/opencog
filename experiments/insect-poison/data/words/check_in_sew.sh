#!/bin/bash

for f in $(ls *.words);do 
	for w in $(cat $f);do
		echo $w"......."$(cat ../sew-splitted | grep $w | wc -l); 
	done; 
done; 
