**Overview**

 This directory contains scripts and datasets used for setting up the experiment 
 elaborated in this [mailing list](https://groups.google.com/forum/#!searchin/opencog/insect%7Csort:relevance/opencog/qpDwVAPkKb8/CkkzsZF_EgAJ).
 These scripts and datasets are meant to be used together with the ECAN system. If you are nto familiar
 with ECAN, you should read the [ECAN wiki page first](http://wiki.opencog.org/w/Economic_attention_allocation).

**Objective**
 The major goals of these experiments are:
  - Analyse attention spreading as topics are changed from insect to poison and visaviz.
  - Analyse how Hebbian links change the spreading dynamics and the overall attentional focus.

**Running the experiment**
**Experiment 1**

step 1 - Checkout to this branch by cloning this repo first or if you have already gotten opencog cloned from opencog main repo, do the following:   
``` git remote add misgana  https://github.com/misgeatgit/opencog ```
``` git fetch misgana && git checkout misgana/experiment```

step 2 - Build opencog ( The build directory should be inside opencog directory i.e opencog/build)

step 3 - with in the build director also build the experiment by typing ```make experiments```

step 4 - Clone [Relex](https://github.com/opencog/relex) in the same directory you have cloned opencog since the launch script assumes Relex and opencog are installed side by side. Make sure relex is built properly and runs.

step 5 - In the script director of this directory, modify the ```oc_dir``` in start_exp.sh to point to the directory where relex and opencog and are cloned.

step 6 - in the scripts folder modify the experiments.conf by adding new ecan paramenter values as or removing the existing ones. The best way would be copying one of the lines and changing the values. Each line indicated a different experiment. So, if you have 10 new line separated parameter values, there will be 10 experiment ran by the launch script.

step 7. Download wordnet and conceptnet4.scm and add it in ```opencog/experiments/insect-poison/data/kb``` directory.

step 8. Run the start_exp.sh file and wait for the experiments to finish.

step 9. Get your result which will be stored in ```opencog/experiments/insect-poison/data/kb```.
