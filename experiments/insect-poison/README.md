## Overview

 This directory contains scripts and datasets used for setting up the experiment 
 elaborated in this [mailing list](https://groups.google.com/forum/#!searchin/opencog/insect%7Csort:relevance/opencog/qpDwVAPkKb8/CkkzsZF_EgAJ).
 These scripts and datasets are meant to be used together with the ECAN system. If you are nto familiar
 with ECAN, you should read the [ECAN wiki page first](http://wiki.opencog.org/w/Economic_attention_allocation).

## Objective

 The major objectives of these experiments are:
  - Analyse the shifting and drifting nature of attention as topics are changed from insect to poison and vice versa.
  - Analyse the contribution of HebbianLinks in the formation of weak links which stabilize the dynamics.

## Running the experiments

Currently, only experiment 1 is fully implemented. The following steps need to be carried out inorder to set up the experiment environment and run experiment 1:

**Step 1** - Checkout to this branch by cloning this repo first or if you have already gotten opencog cloned from opencog main repo, do the following:

``` git remote add misgana  https://github.com/misgeatgit/opencog ```

``` git fetch misgana && git checkout misgana/experiment ```

**Step 2** - clone atomspace ( ``` git clone  https://github.com/opencog/atomspace ``` ) and do the following:

``` git remote add misgana  https://github.com/misgeatgit/atomspace ```

``` git fetch misgana && git checkout misgana/ecan-experiment ```

**Step 3** - Build opencog ( The build directory should be inside opencog directory i.e opencog/build)

**Step 4** - with in the build director also build the experiment by typing ```make experiments```

**Step 5** - Clone [Relex](https://github.com/opencog/relex) in the same directory you have cloned opencog since the launch script assumes Relex and opencog are installed side by side. Make sure relex is built properly and runs.

**Step 6** - In the script director of this directory, modify the ```oc_dir``` in start_exp.sh to point to the directory where relex and opencog and are cloned.

**Step 7** - in the scripts folder modify the experiments.conf by adding new ecan paramenter values as or removing the existing ones. The best way would be copying one of the lines and changing the values. Each line indicated a different experiment. So, if you have 10 new line separated parameter values, there will be 10 experiment ran by the launch script.

**Step 8** - Download wordnet and conceptnet4.scm and add it in ```opencog/experiments/insect-poison/data/kb``` directory. The files could be downloaded from [here](https://drive.google.com/open?id=1Br5PIfHt3pCIP7pZ9X6EoyaeqQ_c6b7I)

**Step 9** - Run the ```start_exp.sh``` file which is found in the scrips directory and wait for the experiments to finish.

**step 10** - Get the result of the experiment from ```opencog/experiments/insect-poison/data/log``` directory.

