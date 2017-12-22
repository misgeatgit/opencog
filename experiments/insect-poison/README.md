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

step 1 - Load ecan module and libinsect-poison-exp module 
``` opencog>loadmodule opencog/experiments/insect-poison/libinsect-poison-exp.so```

``` opencog>loadmodule opencog/attention/libattention.so```

step 2 - Load wordnet and conceptnet data
 Import the necessary opencog modules first.
```scm>  (use-modules (opencog nlp))  ```

``` scm> (primitive-load "PATH_TO/wordnet.scm")  (primitive-load "PATH_TO/coceptnet4.scm")```

NOTE: wordnet.scm and conceptnet4.scm are not included in the dir due to their large size. 

step 3 - Start the logger agent and all or some of the ecan agents

``` opencog> agents-start opencog::LoggerAgent ```

or

``` opencog> start-logger ```

usually, we start all the ECAN agents together unless we want to try them one by one or with fewer sets of them. There is a handy command when we want to start all of them at once.

``` opencog> start-ecan ```

step 4 - Set stimulation amount for wordnodes and the start parsing the insect sentence

 ``` scm> (nlp-start-stimulation 70)  (parse-all nlp-parse PATH_TO/insect-poison/exp1_insects_sent.txt") ```   
 
step 5 - While sentence is being parsed or after finishes parsing you could dump ECAN related statistics by typing

``` opencog> dump-af-stat $A_FILE_NAME ``` 

File $A_FILE_NAME will be created under inside the dir where cogserver was lauched.

** Swithcing topic to poison**

For topic switched related statistics to work properly, the logger agent should be informed that topic is going to be changed. The steps to follow are:

- Set topic switched to true by typic ```opencog> topic-switched 1 ```

The start parsing poison senenteces
```scm> (use-modules (opencog nlp relex2logic)) (use-modules (opencog nlp chatbot)) ```

 ```scm> (parse-all nlp-parse PATH_TO/insect-poison/exp1_poison_sent.txt") ```

Now You may dump statistical information using the ***dump-af-stat** command with a different filename so that it won't overrwrithe the previous one.

**TODO***
1. ~~Write a script which automates all the above steps so that we could run the whole thing in one go.~~
2. Extend the logger so that we will have information regarding :
   * ~~How much STI has been diffused via Hebbian links~~
   * How much STI gained from direct stimulation
3. Visualize the statistical data dumped by dump-af-stat (i.e plot graphs showin various relationships) extract-items.cc tries to do this although it is incomplete and its scope is not well defined.

**Helpful opencog commands**

- *start-ecan*  - Starts all the ecan agents
- *stop-ecan*  - Stops all ECAN agents
- *list-ecan-param* - Lists ECAN paraemter-name, value pairs
- *set-ecan-param \<param_name\> \<value\>* - Sets a value for a given ECAN parameter name 
- *agents-active* - Lists currently running agents
- *agents-start <agent-name>* - Starts an agent
- *agents-stop \<agent-name\>* - Stops an agent

**Helpful scheme functions**

- *cog-af* 
- *cog-af-boundary* deprecated. 


