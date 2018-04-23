for wf in $(ls *.words);do
	for w in $(cat $wf);do
		touch matching.tmp;
		cat ../sew-splitted | grep $w >> matching.tmp;
	done; 
	wc -l matching.tmp;
done;
