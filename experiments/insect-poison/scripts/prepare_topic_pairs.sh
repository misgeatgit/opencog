#for word in $(cat insects.txt); do  wikit $word >> insect_paragraph.txt; done


II="insect_insecticide.word"
PI="poison_insecticide.word"

touch $II
touch $PI
cat insect.word >> $II
cat insecticide.word >> $II

cat poison.word >> $PI
cat insecticide.word >> $II

for f in {$II, $PI}; 
do
       julia --depwarn=no insect_poison_db_from_adagram.jl ~/Desktop/ECAN/adagram-wiki-model1 $f >> kb.scm; 
done 

