#!/bin/bash

read -p "All the results from the previous experiment is about to be deleted.
         Are you sure? [yes/no] " answer
if [ $answer = "no" ]; then exit; fi

rm -rf /home/misgana/OPENCOG/opencog/experiments/insect-poison/data/log/* 
pid_cogserver=$(lsof -ti :17001)
pid_relex=$(lsof -ti :4444)

if [ $pid_cogserver ]; then echo "Cogserver is already running.Kill cogserser."; kill -9 $pid_cogserver;  fi
if [ $pid_relex ]; then echo "Relex server is already running.Kill relex."; kill -9 $pid_relex;  fi

python /home/misgana/OPENCOG/opencog/experiments/insect-poison/scripts/launch_exp.py 1 /home/misgana/OPENCOG
