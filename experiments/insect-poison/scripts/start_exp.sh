#!/bin/bash

rm -rf /home/misgana/OPENCOG/opencog/experiments/insect-poison/data/log/* 
pid_cogserver=$(lsof -ti :17001)
pid_relex=$(lsof -ti :4444)

if [ $pid_cogserver ]; then echo "Kill cogserser"; kill -9 $pid_cogserver;  fi
if [ $pid_relex ]; then echo "Kill relex"; kill -9 $pid_relex;  fi

python /home/misgana/OPENCOG/opencog/experiments/insect-poison/scripts/launch_exp.py 1 /home/misgana/OPENCOG
