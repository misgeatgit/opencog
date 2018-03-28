#!/bin/bash
#for word in $(cat insects.txt); do  wikit $word >> insect_paragraph.txt; done


II="insect_insecticide.words"
PI="poison_insecticide.words"

touch $II
touch $PI
cat insects.words >> $II
cat insecticide.words >> $II

cat poison.words >> $PI
cat insecticide.words >> $II

for f in $II $PI; 
do
	julia --depwarn=no insect_poison_db_from_adagram.jl ~/Desktop/ECAN/adagram-wiki-model1 $(pwd)"/"$f >> adagram_sm_links.scm; 
done 

