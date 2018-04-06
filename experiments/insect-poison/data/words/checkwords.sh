for wf in  "insects.words" "poison.words"; do
	echo "CHECKING words in "$wf
	for w in $(cat $wf); do
		for sent in "insects.sent" "poisons.sent"; do
			matchs=$(grep -o $w "../sentences/"$sent | uniq); 
			for m in $matchs; do
				for db in "wordnet.scm" "conceptnet4.scm" ; do 
					echo "WORDS THAT MATCHED IN "$sent" in db "$db
					grep -o $m "/home/misgana/Desktop/ECAN/db/"$db | uniq;
				done;
			done;
		done;
	done;
done;
